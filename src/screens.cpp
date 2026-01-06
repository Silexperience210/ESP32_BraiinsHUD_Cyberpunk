#include "screens.h"

ScreenManager::ScreenManager(LGFX* display, APIManager* apiManager, Animations* animations) {
  tft = display;
  api = apiManager;
  anim = animations;
  currentScreen = SCREEN_NEURAL;
  lastScreenSwitch = 0;
}

void ScreenManager::begin() {
  tft->fillScreen(COLOR_BG);
}

void ScreenManager::update() {
  unsigned long now = millis();
  
  // Auto-rotate screens every SCREEN_DURATION
  if (now - lastScreenSwitch >= SCREEN_DURATION) {
    nextScreen();
    lastScreenSwitch = now;
  }
  
  // Render current screen
  switch (currentScreen) {
    case SCREEN_NEURAL:
      drawScreenNeural();
      break;
    case SCREEN_WORKERS:
      drawScreenWorkers();
      break;
    case SCREEN_STATS:
      drawScreenStats();
      break;
    case SCREEN_VAULT:
      drawScreenVault();
      break;
  }
}

void ScreenManager::nextScreen() {
  // Transition effect
  anim->transitionWarp();
  
  currentScreen++;
  if (currentScreen > SCREEN_VAULT) {
    currentScreen = SCREEN_NEURAL;
  }
  
  tft->fillScreen(COLOR_BG);
}

void ScreenManager::setScreen(int screen) {
  if (screen >= SCREEN_NEURAL && screen <= SCREEN_VAULT) {
    currentScreen = screen;
    lastScreenSwitch = millis();
    tft->fillScreen(COLOR_BG);
  }
}

// ============================================
// SCREEN 1: NEURAL INTERFACE - Main Dashboard
// ============================================

void ScreenManager::drawScreenNeural() {
  BraiinsData data = api->getBraiinsData();
  BTCPrice price = api->getBTCPrice();
  
  // Draw background animations
  anim->drawPulsarRings();
  anim->drawFloatingBubbles();
  anim->drawHexCorners();
  
  // Header
  drawHeader("BRAIINS NEURAL", COLOR_PRIMARY);
  
  // Center: Large hashrate display
  tft->setTextDatum(middle_center);
  
  // Hashrate value
  char hashrateStr[20];
  formatHashrate(data.hashrate_1h, hashrateStr);
  anim->drawGlowText(CENTER_X, CENTER_Y - 20, String(hashrateStr), COLOR_PRIMARY, 2);
  
  // Hashrate label
  tft->setTextColor(COLOR_DIM, COLOR_BG);
  tft->setTextSize(1);
  tft->drawString("CURRENT HASHRATE", CENTER_X, CENTER_Y + 10);
  
  // Circular gauge around hashrate
  float percent = (data.hashrate_1h / (data.hashrate_24h + 0.1));
  if (percent > 1.0) percent = 1.0;
  anim->drawCircularGauge(CENTER_X, CENTER_Y - 20, 50, percent, COLOR_PRIMARY);
  
  // BTC Price (top section)
  if (price.valid) {
    char priceStr[20];
    formatUSD(price.usd, priceStr);
    
    tft->setTextColor(COLOR_ACCENT, COLOR_BG);
    tft->setTextSize(2);
    tft->drawString((String("BTC ") + priceStr).c_str(), CENTER_X, 50);
    
    // Price change indicator
    String changeStr = String(price.change_24h > 0 ? "+" : "") + String(price.change_24h, 1) + "%";
    uint16_t changeColor = price.change_24h >= 0 ? COLOR_SUCCESS : COLOR_SECONDARY;
    tft->setTextColor(changeColor, COLOR_BG);
    tft->setTextSize(1);
    tft->drawString(changeStr.c_str(), CENTER_X, 70);
  }
  
  // Workers status (bottom section)
  String workersStr = String(data.workers_active) + " WORKERS ACTIVE";
  anim->drawPulsingIcon(CENTER_X - 40, CENTER_Y + 50, "W", COLOR_ACCENT);
  tft->setTextColor(COLOR_TEXT, COLOR_BG);
  tft->setTextSize(1);
  tft->drawString(workersStr.c_str(), CENTER_X + 10, CENTER_Y + 53);
  
  // Status bar at bottom
  drawStatusBar();
  
  // Breathing glow around screen
  anim->drawBreathingGlow();
}

// ============================================
// SCREEN 2: WORKERS MATRIX - Workers List
// ============================================

void ScreenManager::drawScreenWorkers() {
  BraiinsData data = api->getBraiinsData();
  WorkerData* workers = api->getWorkers();
  int workerCount = api->getWorkerCount();
  
  // Draw background animations
  anim->drawParticles();
  anim->drawScanlines();
  anim->drawDataStreams();
  
  // Header
  drawHeader("HASHRATE GRID", COLOR_ACCENT);
  
  // Worker list
  tft->setTextDatum(top_left);
  int yPos = 50;
  
  if (workerCount == 0) {
    tft->setTextColor(COLOR_DIM, COLOR_BG);
    tft->setTextSize(1);
    tft->drawString("NO WORKERS FOUND", 20, CENTER_Y);
    tft->drawString("Check your Braiins", 20, CENTER_Y + 20);
    tft->drawString("Pool configuration", 20, CENTER_Y + 40);
  } else {
    // Draw main worker (oct)
    tft->setTextColor(COLOR_PRIMARY, COLOR_BG);
    anim->drawHexagon(25, yPos + 10, 8, COLOR_PRIMARY);
    tft->setTextSize(1);
    tft->drawString(BRAIINS_WORKER, 40, yPos);
    
    char hashrateStr[20];
    formatHashrate(data.hashrate_1h, hashrateStr);
    tft->setTextColor(COLOR_TEXT, COLOR_BG);
    tft->drawString(hashrateStr, 140, yPos);
    
    yPos += 30;
    
    // Draw individual workers
    for (int i = 0; i < workerCount && i < 5; i++) {
      uint16_t color = workers[i].online ? COLOR_SUCCESS : COLOR_DIM;
      
      anim->drawHexagon(25, yPos + 10, 6, color);
      
      tft->setTextColor(color, COLOR_BG);
      tft->drawString(workers[i].name, 40, yPos);
      
      char workerHashStr[20];
      if (workers[i].hashrate >= 1.0) {
        sprintf(workerHashStr, "%.2f TH/s", workers[i].hashrate);
      } else {
        sprintf(workerHashStr, "%.0f GH/s", workers[i].hashrate * 1000);
      }
      
      tft->setTextColor(COLOR_TEXT, COLOR_BG);
      tft->drawString(workerHashStr, 140, yPos);
      
      yPos += 25;
    }
  }
  
  // Total at bottom
  tft->setTextDatum(middle_center);
  char totalStr[30];
  formatHashrate(data.hashrate_1h, totalStr);
  String totalDisplay = String("TOTAL: ") + totalStr;
  
  tft->fillRect(20, 200, 200, 25, COLOR_DIM);
  anim->drawGlowText(CENTER_X, 212, totalDisplay, COLOR_PRIMARY, 1);
  
  // Status bar
  drawStatusBar();
}

// ============================================
// SCREEN 3: QUANTUM STATS - 24h Analytics
// ============================================

void ScreenManager::drawScreenStats() {
  BraiinsData data = api->getBraiinsData();
  
  // Draw background animations
  anim->drawFloatingBubbles();
  anim->drawGlitchEffect();
  anim->drawHexCorners();
  
  // Header
  drawHeader("24H ANALYTICS", COLOR_SECONDARY);
  
  // Simple bar graph area (simplified visualization)
  int graphY = 70;
  int graphHeight = 80;
  int graphWidth = 200;
  int graphX = 20;
  
  // Draw grid
  tft->setTextColor(COLOR_DIM, COLOR_BG);
  for (int i = 0; i < 5; i++) {
    int y = graphY + (i * graphHeight / 4);
    tft->drawFastHLine(graphX, y, graphWidth, COLOR_DIM);
  }
  
  // Draw simplified hashrate bars (simulated data points)
  float avgHashrate = data.hashrate_24h;
  float currentHashrate = data.hashrate_1h;
  
  // Draw 8 bars representing last 24h (simplified)
  for (int i = 0; i < 8; i++) {
    int barX = graphX + (i * 25);
    float variance = (random(-10, 10) / 100.0);
    float barHashrate = avgHashrate * (1.0 + variance);
    
    int barHeight = (barHashrate / (avgHashrate * 1.2)) * graphHeight;
    if (barHeight > graphHeight) barHeight = graphHeight;
    if (barHeight < 0) barHeight = 0;
    
    uint16_t barColor = (i == 7) ? COLOR_PRIMARY : COLOR_ACCENT;
    tft->fillRect(barX, graphY + graphHeight - barHeight, 20, barHeight, barColor);
  }
  
  // Stats below graph
  tft->setTextDatum(top_left);
  int statsY = 165;
  
  // Average hashrate
  tft->setTextColor(COLOR_TEXT, COLOR_BG);
  tft->setTextSize(1);
  tft->drawString("AVG 24H:", 20, statsY);
  char avgStr[20];
  formatHashrate(data.hashrate_24h, avgStr);
  tft->setTextColor(COLOR_ACCENT, COLOR_BG);
  tft->drawString(avgStr, 100, statsY);
  
  statsY += 20;
  
  // Shares
  tft->setTextColor(COLOR_TEXT, COLOR_BG);
  tft->drawString("SHARES:", 20, statsY);
  String sharesStr = String(data.shares_valid);
  tft->setTextColor(COLOR_SUCCESS, COLOR_BG);
  tft->drawString(sharesStr.c_str(), 100, statsY);
  
  statsY += 20;
  
  // Invalid shares
  if (data.shares_invalid > 0) {
    tft->setTextColor(COLOR_TEXT, COLOR_BG);
    tft->drawString("INVALID:", 20, statsY);
    tft->setTextColor(COLOR_SECONDARY, COLOR_BG);
    tft->drawString(String(data.shares_invalid).c_str(), 100, statsY);
  }
  
  // Status bar
  drawStatusBar();
  
  // Pulsing border
  anim->drawBreathingGlow();
}

// ============================================
// SCREEN 4: SATOSHI VAULT - Rewards
// ============================================

void ScreenManager::drawScreenVault() {
  BraiinsData data = api->getBraiinsData();
  BTCPrice price = api->getBTCPrice();
  
  // Draw background animations
  anim->drawPulsarRings();
  anim->drawParticles();
  anim->drawHexCorners();
  
  // Header
  drawHeader("REWARDS MATRIX", COLOR_PRIMARY);
  
  tft->setTextDatum(middle_center);
  
  // Today's rewards section
  int sectionY = 70;
  
  tft->setTextColor(COLOR_DIM, COLOR_BG);
  tft->setTextSize(1);
  tft->drawString("TODAY", CENTER_X, sectionY);
  
  sectionY += 20;
  
  // BTC amount
  char btcStr[20];
  formatBTC(data.btc_rewards_today, btcStr);
  anim->drawGlowText(CENTER_X, sectionY, String(btcStr) + " BTC", COLOR_PRIMARY, 2);
  
  sectionY += 30;
  
  // USD value
  if (price.valid) {
    char usdStr[20];
    formatUSD(data.btc_rewards_today * price.usd, usdStr);
    tft->setTextColor(COLOR_ACCENT, COLOR_BG);
    tft->setTextSize(1);
    tft->drawString(usdStr, CENTER_X, sectionY);
  }
  
  // Monthly estimate section
  sectionY = 140;
  
  tft->setTextColor(COLOR_DIM, COLOR_BG);
  tft->drawString("MONTH (EST)", CENTER_X, sectionY);
  
  sectionY += 20;
  
  // Estimated monthly BTC (today's rate * 30)
  float monthlyBTC = data.btc_rewards_today * 30;
  char monthlyBTCStr[20];
  formatBTC(monthlyBTC, monthlyBTCStr);
  tft->setTextColor(COLOR_TEXT, COLOR_BG);
  tft->drawString((String(monthlyBTCStr) + " BTC").c_str(), CENTER_X, sectionY);
  
  sectionY += 20;
  
  // Monthly USD value
  if (price.valid) {
    char monthlyUSDStr[20];
    formatUSD(monthlyBTC * price.usd, monthlyUSDStr);
    tft->setTextColor(COLOR_SUCCESS, COLOR_BG);
    tft->drawString(monthlyUSDStr, CENTER_X, sectionY);
  }
  
  // Status bar
  drawStatusBar();
  
  // Breathing glow
  anim->drawBreathingGlow();
}

// ============================================
// HELPER FUNCTIONS
// ============================================

void ScreenManager::drawHeader(String title, uint16_t color) {
  tft->setTextDatum(middle_center);
  
  // Draw hexagon borders
  anim->drawHexagon(30, 20, 8, color);
  anim->drawHexagon(210, 20, 8, color);
  
  // Draw title with glow
  anim->drawGlowText(CENTER_X, 20, title, color, 1);
}

void ScreenManager::drawStatusBar() {
  int barY = 230;
  
  tft->setTextDatum(top_left);
  tft->setTextColor(COLOR_DIM, COLOR_BG);
  tft->setTextSize(1);
  
  // WiFi status
  if (api->isWiFiConnected()) {
    tft->drawString("WiFi", 5, barY);
    tft->fillCircle(35, barY + 3, 2, COLOR_SUCCESS);
  } else {
    tft->drawString("WiFi", 5, barY);
    tft->fillCircle(35, barY + 3, 2, COLOR_SECONDARY);
  }
  
  // Braiins status
  if (api->isBraiinsConnected()) {
    tft->drawString("Pool", 50, barY);
    tft->fillCircle(75, barY + 3, 2, COLOR_SUCCESS);
  } else {
    tft->drawString("Pool", 50, barY);
    tft->fillCircle(75, barY + 3, 2, COLOR_SECONDARY);
  }
  
  // Screen indicator
  tft->setTextDatum(top_right);
  String screenStr = String(currentScreen + 1) + "/4";
  tft->drawString(screenStr.c_str(), 235, barY);
}

void ScreenManager::formatHashrate(float thps, char* buffer) {
  if (thps >= 1.0) {
    sprintf(buffer, "%.2f TH/s", thps);
  } else if (thps >= 0.001) {
    sprintf(buffer, "%.0f GH/s", thps * 1000);
  } else {
    sprintf(buffer, "%.0f MH/s", thps * 1000000);
  }
}

void ScreenManager::formatBTC(float btc, char* buffer) {
  if (btc >= 0.01) {
    sprintf(buffer, "%.4f", btc);
  } else if (btc >= 0.0001) {
    sprintf(buffer, "%.6f", btc);
  } else {
    sprintf(buffer, "%.8f", btc);
  }
}

void ScreenManager::formatUSD(float usd, char* buffer) {
  if (usd >= 1000) {
    sprintf(buffer, "$%.0f", usd);
  } else if (usd >= 10) {
    sprintf(buffer, "$%.2f", usd);
  } else {
    sprintf(buffer, "$%.4f", usd);
  }
}

void ScreenManager::drawFooter() {
  // Reserved for future use
}
