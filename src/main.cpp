#include <Arduino.h>
#include <Wire.h>
#include <CST816S.h>

#include "display.h"
#include "config.h"
#include "api.h"
#include "settings.h"
#include "webportal.h"

// ============================================
// Global Objects
// ============================================

LGFX tft;
APIManager api;
CST816S touch(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
SettingsManager settings;
WebPortal portal(&settings);

// Mode flags
bool configMode = false;       // True when in config portal mode
unsigned long touchStartTime = 0;  // For long press detection
bool longPressTriggered = false;

// Timing
unsigned long lastApiUpdate = 0;
unsigned long lastScreenSwitch = 0;
unsigned long lastTouch = 0;
unsigned long lastActivity = 0;  // For sleep mode
int currentScreen = 0;
int lastDrawnScreen = -1;  // Track which screen was last drawn
const int NUM_SCREENS = 4; // Added Workers screen
bool forceRedraw = false;   // Force redraw flag
bool autoRotate = true;     // Auto screen rotation toggle
bool sleepMode = false;     // Sleep mode flag
float previousHashrate = 0; // For hashrate drop alert

// Cache for avoiding flicker
float lastHashrate = -1;
float lastBTCPrice = -1;
float lastRewards = -1;
int lastWorkers = -1;

// ============================================
// Helper Functions
// ============================================

void formatHashrate(float thps, char* buffer) {
  if (thps >= 1.0) {
    sprintf(buffer, "%.2f TH/s", thps);
  } else if (thps >= 0.001) {
    sprintf(buffer, "%.0f GH/s", thps * 1000);
  } else {
    sprintf(buffer, "%.0f MH/s", thps * 1000000);
  }
}

void formatBTC(float btc, char* buffer) {
  if (btc >= 0.01) {
    sprintf(buffer, "%.4f", btc);
  } else if (btc >= 0.0001) {
    sprintf(buffer, "%.6f", btc);
  } else {
    sprintf(buffer, "%.8f", btc);
  }
}

// Get alert color based on system status
uint16_t getAlertBorderColor() {
  // Single-worker mode: alert is based on that specific worker
  if (api.hasTargetWorker()) {
    SingleWorkerData sw = api.getTargetWorker();
    if (sw.found && !sw.online) {
      return COLOR_SECONDARY;  // Red: target worker is offline
    }
    // Hashrate drop detection for single worker
    if (sw.found && previousHashrate > 0 && sw.hashrate_5m > 0) {
      float drop = (previousHashrate - sw.hashrate_5m) / previousHashrate;
      if (drop > 0.2) return COLOR_PRIMARY;  // Orange: >20% drop
    }
    return 0;  // Target worker is fine
  }

  // Aggregate mode: alert if any worker offline
  BraiinsData data = api.getBraiinsData();
  if (data.workers_offline > 0) {
    return COLOR_SECONDARY;  // Red
  }
  if (previousHashrate > 0 && data.hashrate_1h > 0) {
    float drop = (previousHashrate - data.hashrate_1h) / previousHashrate;
    if (drop > 0.2) return COLOR_PRIMARY;  // Orange
  }
  return 0;  // No alert
}

// Alert blink state (global for persistence)
static bool alertToggle = false;
static unsigned long lastAlertToggle = 0;

// Update alert blink state - returns true if state changed
bool updateAlertBlink() {
  if (millis() - lastAlertToggle > 500) {
    alertToggle = !alertToggle;
    lastAlertToggle = millis();
    return true;
  }
  return false;
}

// Draw just the border (for alert blinking without redrawing everything)
void drawBorderOnly(uint16_t alertColor) {
  // Clear border area first (draw black circles)
  tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_BG);
  tft.drawCircle(CENTER_X, CENTER_Y, 116, COLOR_BG);
  tft.drawCircle(CENTER_X, CENTER_Y, 117, COLOR_BG);
  
  // Draw alert or normal border
  if (alertColor != 0 && alertToggle) {
    tft.drawCircle(CENTER_X, CENTER_Y, 115, alertColor);
    tft.drawCircle(CENTER_X, CENTER_Y, 116, alertColor);
    tft.drawCircle(CENTER_X, CENTER_Y, 117, alertColor);
  } else {
    tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_PRIMARY);
  }
}

// ============================================
// Clean Minimal Screens
// ============================================

void drawScreenMain() {
  BraiinsData data = api.getBraiinsData();
  BTCPrice price = api.getBTCPrice();
  
  // Check for alerts
  uint16_t alertColor = getAlertBorderColor();
  
  // Handle alert blinking separately (only redraw border)
  if (alertColor != 0) {
    if (updateAlertBlink()) {
      drawBorderOnly(alertColor);
    }
  }
  
  // Only full redraw if data changed
  bool needsRedraw = (data.hashrate_1h != lastHashrate) || 
                     (price.usd != lastBTCPrice) ||
                     (data.workers_active != lastWorkers);
  
  if (!needsRedraw) return;
  
  lastHashrate = data.hashrate_1h;
  lastBTCPrice = price.usd;
  lastWorkers = data.workers_active;
  
  tft.fillScreen(COLOR_BG);
  
  // Border color based on alert status
  if (alertColor != 0 && alertToggle) {
    tft.drawCircle(CENTER_X, CENTER_Y, 115, alertColor);
    tft.drawCircle(CENTER_X, CENTER_Y, 116, alertColor);
    tft.drawCircle(CENTER_X, CENTER_Y, 117, alertColor);
  } else {
    tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_PRIMARY);
  }
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);
  
  // Title
  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("BRAIINS POOL", CENTER_X, 35);
  
  // Hashrate - main focus
  char hashrateStr[20];
  formatHashrate(data.hashrate_1h, hashrateStr);
  
  // Label above value
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("HASHRATE", CENTER_X, CENTER_Y - 35);
  
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.setTextSize(3);
  tft.drawString(hashrateStr, CENTER_X, CENTER_Y - 5);
  
  // BTC Price
  if (price.valid) {
    // Label above value
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("BTC/USD", CENTER_X, CENTER_Y + 35);
    
    char priceStr[20];
    sprintf(priceStr, "$%.0f", price.usd);
    tft.setTextColor(COLOR_ACCENT, COLOR_BG);
    tft.setTextSize(2);
    tft.drawString(priceStr, CENTER_X, CENTER_Y + 60);
  }
  
  // Status indicators at bottom
  tft.setTextSize(1);
  
  // Workers count
  tft.setTextColor(data.workers_active > 0 ? COLOR_SUCCESS : COLOR_DIM, COLOR_BG);
  String workersStr = String(data.workers_active) + " worker";
  if (data.workers_active != 1) workersStr += "s";
  tft.drawString(workersStr.c_str(), CENTER_X, 195);
  
  // Connection status & auto-rotate indicator
  tft.fillCircle(CENTER_X - 15, 215, 4, api.isBraiinsConnected() ? COLOR_SUCCESS : COLOR_SECONDARY);
  
  // Auto-rotate indicator (pause icon if disabled)
  if (!autoRotate) {
    tft.fillRect(CENTER_X + 10, 211, 3, 8, COLOR_WARNING);
    tft.fillRect(CENTER_X + 16, 211, 3, 8, COLOR_WARNING);
  } else {
    // Play icon (triangle) when auto-rotate is on
    tft.fillTriangle(CENTER_X + 10, 211, CENTER_X + 10, 219, CENTER_X + 18, 215, COLOR_DIM);
  }
}

void drawScreenStats() {
  // Only draw once per screen switch
  if (currentScreen == lastDrawnScreen && !forceRedraw) return;
  
  BraiinsData data = api.getBraiinsData();
  
  tft.fillScreen(COLOR_BG);
  
  // Simple circular border
  tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_ACCENT);
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);
  
  // Title
  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("STATISTICS", CENTER_X, 35);
  
  // Stats list
  tft.setTextDatum(middle_left);
  int y = 70;
  int labelX = 40;
  int valueX = 200;
  
  // 1h Hashrate
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("1H AVG", labelX, y);
  char hr1[20];
  formatHashrate(data.hashrate_1h, hr1);
  tft.setTextDatum(middle_right);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.drawString(hr1, valueX, y);
  
  y += 30;
  
  // 24h Hashrate
  tft.setTextDatum(middle_left);
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("24H AVG", labelX, y);
  char hr24[20];
  formatHashrate(data.hashrate_24h, hr24);
  tft.setTextDatum(middle_right);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.drawString(hr24, valueX, y);
  
  y += 30;
  
  // Scoring Hashrate
  tft.setTextDatum(middle_left);
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("SCORING", labelX, y);
  char hrScore[20];
  formatHashrate(data.hashrate_scoring, hrScore);
  tft.setTextDatum(middle_right);
  tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
  tft.drawString(hrScore, valueX, y);
  
  y += 30;
  
  // Valid shares
  tft.setTextDatum(middle_left);
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("SHARES", labelX, y);
  tft.setTextDatum(middle_right);
  tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
  tft.drawString(String(data.shares_valid).c_str(), valueX, y);
  
  y += 30;
  
  // Workers
  tft.setTextDatum(middle_left);
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("WORKERS", labelX, y);
  tft.setTextDatum(middle_right);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  String wStr = String(data.workers_active) + "/" + String(data.workers_total);
  tft.drawString(wStr.c_str(), valueX, y);
}

void drawScreenRewards() {
  // Only draw once per screen switch
  if (currentScreen == lastDrawnScreen && !forceRedraw) return;
  
  BraiinsData data = api.getBraiinsData();
  BTCPrice price = api.getBTCPrice();
  
  tft.fillScreen(COLOR_BG);
  
  // Simple circular border  
  tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_SUCCESS);
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);
  
  // Title
  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("REWARDS", CENTER_X, 35);
  
  // Today's rewards
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.drawString("TODAY", CENTER_X, 60);
  
  char todayBTC[20];
  formatBTC(data.btc_rewards_today, todayBTC);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.setTextSize(2);
  tft.drawString((String(todayBTC) + " BTC").c_str(), CENTER_X, 85);
  
  if (price.valid) {
    char todayUSD[20];
    sprintf(todayUSD, "$%.2f", data.btc_rewards_today * price.usd);
    tft.setTextColor(COLOR_ACCENT, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString(todayUSD, CENTER_X, 110);
  }
  
  // Unpaid balance
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("UNPAID", CENTER_X, 140);
  
  char unpaidBTC[20];
  formatBTC(data.btc_unpaid, unpaidBTC);
  tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
  tft.setTextSize(2);
  tft.drawString((String(unpaidBTC) + " BTC").c_str(), CENTER_X, 165);
  
  if (price.valid) {
    char unpaidUSD[20];
    sprintf(unpaidUSD, "$%.2f", data.btc_unpaid * price.usd);
    tft.setTextColor(COLOR_ACCENT, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString(unpaidUSD, CENTER_X, 190);
  }
  
  // Total paid (small at bottom)
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  char totalStr[30];
  sprintf(totalStr, "Total: %.6f BTC", data.btc_total_paid);
  tft.drawString(totalStr, CENTER_X, 215);
}

// ---- Single-Worker focused screen ----
void drawScreenSingleWorker() {
  SingleWorkerData sw = api.getTargetWorker();

  tft.fillScreen(COLOR_BG);

  // Border: green = online, red = offline, dim = not found yet
  uint16_t borderColor = sw.found ? (sw.online ? COLOR_SUCCESS : COLOR_SECONDARY) : COLOR_DIM;
  tft.drawCircle(CENTER_X, CENTER_Y, 115, borderColor);
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);

  tft.setTextDatum(middle_center);

  // Worker name (truncated to fit display width)
  // Extract short name from "username.workername" if dot present
  String fullName = String(sw.name);
  int dotIdx = fullName.lastIndexOf('.');
  String shortName = (dotIdx >= 0) ? fullName.substring(dotIdx + 1) : fullName;
  if (shortName.length() == 0) shortName = "WORKER";

  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("TARGET WORKER", CENTER_X, 30);

  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.setTextSize(2);
  tft.drawString(shortName.c_str(), CENTER_X, 55);

  if (!sw.found) {
    // Worker not found yet
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("Waiting for data...", CENTER_X, CENTER_Y);
    tft.drawString("Check worker name", CENTER_X, CENTER_Y + 20);
  } else {
    // Status badge
    uint16_t statusColor = sw.online ? COLOR_SUCCESS : COLOR_SECONDARY;
    const char* statusLabel = sw.online ? "ONLINE" : "OFFLINE";
    tft.setTextColor(statusColor, COLOR_BG);
    tft.setTextSize(2);
    tft.drawString(statusLabel, CENTER_X, 85);

    // Current hashrate (5m) — main focus
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("HASHRATE (5m)", CENTER_X, CENTER_Y - 30);

    char hashrateStr[20];
    formatHashrate(sw.hashrate_5m, hashrateStr);
    tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
    tft.setTextSize(3);
    tft.drawString(hashrateStr, CENTER_X, CENTER_Y);

    // 1h average
    tft.setTextDatum(middle_left);
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("1H AVG", 30, CENTER_Y + 40);
    char hr1h[20];
    formatHashrate(sw.hashrate_1h, hr1h);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString(hr1h, 30, CENTER_Y + 55);

    // Scoring hashrate
    tft.setTextDatum(middle_right);
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.drawString("SCORING", 210, CENTER_Y + 40);
    char hrScore[20];
    formatHashrate(sw.hashrate_scoring, hrScore);
    tft.setTextColor(COLOR_ACCENT, COLOR_BG);
    tft.drawString(hrScore, 210, CENTER_Y + 55);
  }

  // Connection dot bottom
  tft.setTextDatum(middle_center);
  tft.fillCircle(CENTER_X, 215, 4, api.isBraiinsConnected() ? COLOR_SUCCESS : COLOR_SECONDARY);
}

// ---- Aggregate workers screen (original) ----
void drawScreenWorkersAggregate() {
  BraiinsData data = api.getBraiinsData();

  tft.fillScreen(COLOR_BG);

  uint16_t borderColor = (data.workers_offline > 0) ? COLOR_SECONDARY : COLOR_ACCENT;
  tft.drawCircle(CENTER_X, CENTER_Y, 115, borderColor);
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);

  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("WORKERS", CENTER_X, 35);

  int y = 65;

  tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
  tft.setTextSize(3);
  tft.drawString(String(data.workers_active).c_str(), CENTER_X, y);

  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("ONLINE", CENTER_X, y + 25);

  y += 55;
  tft.drawFastHLine(60, y, 120, COLOR_DIM);
  y += 20;

  int boxWidth = 80;
  int boxHeight = 50;
  int spacing = 10;
  int startX = CENTER_X - boxWidth - spacing/2;

  // Offline box — only show if workers are actually offline
  if (data.workers_offline > 0) {
    tft.drawRect(startX, y, boxWidth, boxHeight, COLOR_SECONDARY);
    tft.setTextColor(COLOR_SECONDARY, COLOR_BG);
    tft.setTextSize(2);
    tft.setTextDatum(middle_center);
    tft.drawString(String(data.workers_offline).c_str(), startX + boxWidth/2, y + 18);
    tft.setTextSize(1);
    tft.drawString("OFFLINE", startX + boxWidth/2, y + 38);
  }

  // Disabled box — only show if workers are disabled
  startX = CENTER_X + spacing/2;
  if (data.workers_disabled > 0) {
    tft.drawRect(startX, y, boxWidth, boxHeight, COLOR_WARNING);
    tft.setTextColor(COLOR_WARNING, COLOR_BG);
    tft.setTextSize(2);
    tft.drawString(String(data.workers_disabled).c_str(), startX + boxWidth/2, y + 18);
    tft.setTextSize(1);
    tft.drawString("DISABLED", startX + boxWidth/2, y + 38);
  }

  // If everything is fine: show a clean "all clear" message
  if (data.workers_offline == 0 && data.workers_disabled == 0) {
    tft.setTextDatum(middle_center);
    tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("ALL WORKERS ONLINE", CENTER_X, y + boxHeight / 2);
  }

  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  char totalStr[30];
  sprintf(totalStr, "Total: %d workers", data.workers_total);
  tft.drawString(totalStr, CENTER_X, 210);
}

// ---- Dispatcher: single or aggregate depending on config ----
void drawScreenWorkers() {
  if (currentScreen == lastDrawnScreen && !forceRedraw) return;

  if (api.hasTargetWorker()) {
    drawScreenSingleWorker();
  } else {
    drawScreenWorkersAggregate();
  }
}

// ============================================
// Boot Screen
// ============================================

void drawBootScreen() {
  tft.fillScreen(COLOR_BG);
  
  // Simple border
  tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_PRIMARY);
  
  // Title
  tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
  tft.setTextDatum(middle_center);
  tft.setTextSize(2);
  tft.drawString("BRAIINS", CENTER_X, CENTER_Y - 20);
  tft.drawString("HUD", CENTER_X, CENTER_Y + 10);
  
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("v2.1 - Single Worker", CENTER_X, CENTER_Y + 40);
  
  delay(1500);
}

void drawWiFiScreen(bool connecting, const char* ssid) {
  tft.fillScreen(COLOR_BG);
  
  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(1);
  
  if (connecting) {
    tft.drawString("Connecting to WiFi...", CENTER_X, CENTER_Y - 20);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString(ssid, CENTER_X, CENTER_Y + 10);
  } else {
    tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
    tft.drawString("Connected!", CENTER_X, CENTER_Y - 20);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString(WiFi.localIP().toString().c_str(), CENTER_X, CENTER_Y + 10);
  }
}

void drawConfigPortalScreen() {
  tft.fillScreen(COLOR_BG);
  
  // Circle border (static)
  tft.drawCircle(CENTER_X, CENTER_Y, 118, COLOR_DIM);
  
  tft.setTextDatum(middle_center);
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("SETUP MODE", CENTER_X, 40);
  
  // WiFi icon
  tft.setTextSize(2);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.drawString("Connect to WiFi:", CENTER_X, 80);
  
  tft.setTextColor(COLOR_PRIMARY, COLOR_BG);
  tft.drawString(DEFAULT_AP_SSID, CENTER_X, 115);
  
  tft.setTextColor(COLOR_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.drawString("Password:", CENTER_X, 145);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.drawString(DEFAULT_AP_PASS, CENTER_X, 165);
  
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.drawString("Then open browser", CENTER_X, 195);
  tft.drawString("192.168.4.1", CENTER_X, 215);
}

// Orbiting dot animation variables
float orbitAngle = 0;
const float ORBIT_RADIUS = 115;
const float ORBIT_SPEED = 0.05;  // radians per frame
const int DOT_SIZE = 4;
const int TRAIL_LENGTH = 8;

void drawOrbitingDot() {
  // Erase previous positions (draw trail fading out)
  for (int i = TRAIL_LENGTH; i >= 0; i--) {
    float trailAngle = orbitAngle - (i * ORBIT_SPEED * 3);
    int trailX = CENTER_X + (int)(cos(trailAngle) * ORBIT_RADIUS);
    int trailY = CENTER_Y + (int)(sin(trailAngle) * ORBIT_RADIUS);
    
    if (i == TRAIL_LENGTH) {
      // Erase oldest position
      tft.fillCircle(trailX, trailY, DOT_SIZE + 1, COLOR_BG);
      // Redraw base circle at that position
      tft.drawCircle(CENTER_X, CENTER_Y, 115, COLOR_DIM);
    }
  }
  
  // Draw trail (fading)
  for (int i = TRAIL_LENGTH - 1; i >= 0; i--) {
    float trailAngle = orbitAngle - (i * ORBIT_SPEED * 3);
    int trailX = CENTER_X + (int)(cos(trailAngle) * ORBIT_RADIUS);
    int trailY = CENTER_Y + (int)(sin(trailAngle) * ORBIT_RADIUS);
    
    // Fade color based on position in trail
    uint8_t brightness = 255 - (i * 25);
    uint16_t trailColor = tft.color565(brightness, brightness * 0.4, 0);  // Orange fading
    
    int dotSize = DOT_SIZE - (i / 3);
    if (dotSize < 1) dotSize = 1;
    tft.fillCircle(trailX, trailY, dotSize, trailColor);
  }
  
  // Draw main dot (bright orange)
  int dotX = CENTER_X + (int)(cos(orbitAngle) * ORBIT_RADIUS);
  int dotY = CENTER_Y + (int)(sin(orbitAngle) * ORBIT_RADIUS);
  tft.fillCircle(dotX, dotY, DOT_SIZE, COLOR_PRIMARY);
  
  // Update angle
  orbitAngle += ORBIT_SPEED;
  if (orbitAngle > 2 * PI) {
    orbitAngle -= 2 * PI;
  }
}

// ============================================
// Setup
// ============================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=========================");
  Serial.println("BRAIINS HUD v2.1");
  Serial.println("Single-Worker Edition");
  Serial.println("=========================\n");
  
  // Initialize display
  Serial.println("[BOOT] Display init...");
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(255);
  
  // Initialize touch
  Serial.println("[BOOT] Touch init...");
  touch.begin();
  Serial.println("[BOOT] Touch ready!");
  
  // Boot screen
  drawBootScreen();
  
  // Load settings from flash
  Serial.println("[BOOT] Loading settings...");
  bool configured = settings.load();
  
  if (!configured) {
    // First boot - start config portal
    Serial.println("[BOOT] Not configured - starting setup portal...");
    configMode = true;
    portal.startPortal();
    drawConfigPortalScreen();
    return;  // Don't continue setup, run portal in loop
  }
  
  // Apply saved brightness
  tft.setBrightness(settings.getBrightness());
  
  // Set API credentials from settings
  api.setCredentials(
    settings.getWifiSSID(),
    settings.getWifiPassword(),
    settings.getBraiinsToken()
  );

  // Configure single-worker mode if a target was set in the portal
  if (strlen(settings.getWorkerName()) > 0) {
    api.setTargetWorker(settings.getWorkerName());
  }

  // Connect WiFi
  Serial.println("[BOOT] WiFi connecting...");
  drawWiFiScreen(true, settings.getWifiSSID());
  api.begin();
  
  if (WiFi.status() == WL_CONNECTED) {
    drawWiFiScreen(false, settings.getWifiSSID());
    delay(1000);
    
    // Fetch initial data
    tft.fillScreen(COLOR_BG);
    tft.setTextDatum(middle_center);
    tft.setTextColor(COLOR_DIM, COLOR_BG);
    tft.setTextSize(1);
    tft.drawString("Loading data...", CENTER_X, CENTER_Y);
    
    api.update();
  } else {
    // WiFi failed - enter config mode
    Serial.println("[BOOT] WiFi failed - starting setup portal...");
    configMode = true;
    portal.startPortal();
    drawConfigPortalScreen();
    return;
  }
  
  Serial.println("[BOOT] Ready!\n");
  
  lastApiUpdate = millis();
  lastScreenSwitch = millis();
  lastActivity = millis();  // Initialize activity timer
  
  // Draw first screen
  tft.fillScreen(COLOR_BG);
  drawScreenMain();
}

// ============================================
// Main Loop
// ============================================

void loop() {
  unsigned long now = millis();
  
  // ========== CONFIG MODE ==========
  if (configMode) {
    portal.handle();
    
    // Check if config was saved
    if (portal.isConfigSaved()) {
      Serial.println("[CONFIG] Settings saved! Restarting...");
      tft.fillScreen(COLOR_BG);
      tft.setTextDatum(middle_center);
      tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
      tft.setTextSize(1);
      tft.drawString("Configuration saved!", CENTER_X, CENTER_Y - 10);
      tft.drawString("Restarting...", CENTER_X, CENTER_Y + 10);
      delay(2000);
      ESP.restart();
    }
    
    // Animate orbiting dot on config screen
    drawOrbitingDot();
    
    delay(30);  // ~33 FPS for smooth animation
    return;  // Don't run normal loop in config mode
  }
  
  // ========== NORMAL MODE ==========
  
  // Handle sleep mode
  if (!sleepMode && (now - lastActivity >= SLEEP_TIMEOUT)) {
    sleepMode = true;
    tft.setBrightness(BRIGHTNESS_DIM);
    Serial.println("[SLEEP] Entering sleep mode");
  }
  
  // Handle touch
  if (touch.available()) {
    // Wake from sleep on any touch
    if (sleepMode) {
      sleepMode = false;
      tft.setBrightness(settings.getBrightness());
      lastActivity = now;
      Serial.println("[SLEEP] Waking up!");
      forceRedraw = true;
      lastHashrate = -1;
      lastBTCPrice = -1;
    }
    
    // Track touch for long press (10s = factory reset)
    int gesture = touch.data.gestureID;
    
    if (gesture == 5) {  // LONG_PRESS gesture detected
      if (touchStartTime == 0) {
        touchStartTime = now;
        longPressTriggered = false;
      } else if (!longPressTriggered && (now - touchStartTime >= 10000)) {
        // 10 second long press = factory reset
        longPressTriggered = true;
        Serial.println("[RESET] Factory reset triggered!");
        
        tft.fillScreen(COLOR_SECONDARY);
        tft.setTextDatum(middle_center);
        tft.setTextColor(COLOR_TEXT, COLOR_SECONDARY);
        tft.setTextSize(1);
        tft.drawString("FACTORY RESET", CENTER_X, CENTER_Y - 10);
        tft.drawString("Restarting...", CENTER_X, CENTER_Y + 10);
        
        settings.reset();
        delay(2000);
        ESP.restart();
      }
    } else {
      touchStartTime = 0;  // Reset if gesture changed
    }
    
    // Normal touch handling (debounce 300ms)
    if (now - lastTouch > 300) {
      lastTouch = now;
      lastActivity = now;  // Reset activity timer
      
      int x = touch.data.x;
      int y = touch.data.y;
      
      Serial.printf("[TOUCH] x=%d y=%d gesture=%d\n", x, y, gesture);
      
      // Swipe left - next screen
      if (gesture == 3) {  // SWIPE_LEFT
        currentScreen = (currentScreen + 1) % NUM_SCREENS;
        lastScreenSwitch = now;
        lastHashrate = -1;
        lastBTCPrice = -1;
        Serial.printf("[TOUCH] Swipe Left -> Screen %d\n", currentScreen);
      }
      // Swipe right - previous screen  
      else if (gesture == 4) {  // SWIPE_RIGHT
        currentScreen = (currentScreen - 1 + NUM_SCREENS) % NUM_SCREENS;
        lastScreenSwitch = now;
        lastHashrate = -1;
        lastBTCPrice = -1;
        Serial.printf("[TOUCH] Swipe Right -> Screen %d\n", currentScreen);
      }
      // Single tap - toggle auto-rotate
      else if (gesture == 1) {  // SINGLE_CLICK
        autoRotate = !autoRotate;
        Serial.printf("[TOUCH] Auto-rotate: %s\n", autoRotate ? "ON" : "OFF");
        forceRedraw = true;
        lastHashrate = -1;
        lastBTCPrice = -1;
      }
    }
  } else {
    touchStartTime = 0;  // Reset if no touch
  }
  
  // Get screen duration from settings
  unsigned long screenDuration = settings.getScreenDuration() * 1000;
  
  // Update API data
  if (now - lastApiUpdate >= API_UPDATE_INTERVAL) {
    // Save previous hashrate for drop detection (worker-specific or aggregate)
    if (api.hasTargetWorker()) {
      SingleWorkerData sw = api.getTargetWorker();
      if (sw.found && sw.hashrate_5m > 0) previousHashrate = sw.hashrate_5m;
    } else {
      BraiinsData oldData = api.getBraiinsData();
      if (oldData.hashrate_1h > 0) previousHashrate = oldData.hashrate_1h;
    }
    
    Serial.println("[API] Updating...");
    api.update();
    lastApiUpdate = now;
    Serial.println("[API] Done!");
    
    // Force redraw after API update
    forceRedraw = true;
    lastHashrate = -1;
    lastBTCPrice = -1;
    lastRewards = -1;
    lastWorkers = -1;
  }
  
  // Auto-switch screens (only if autoRotate is enabled)
  if (autoRotate && now - lastScreenSwitch >= screenDuration) {
    currentScreen = (currentScreen + 1) % NUM_SCREENS;
    lastScreenSwitch = now;
    
    // Force full redraw on screen change
    lastHashrate = -1;
    lastBTCPrice = -1;
  }
  
  // Draw current screen
  switch (currentScreen) {
    case 0:
      drawScreenMain();
      break;
    case 1:
      drawScreenStats();
      break;
    case 2:
      drawScreenRewards();
      break;
    case 3:
      drawScreenWorkers();
      break;
  }
  
  // Mark current screen as drawn
  lastDrawnScreen = currentScreen;
  forceRedraw = false;
  
  delay(100);
}
