#include "api.h"
#include <HTTPClient.h>

APIManager::APIManager() {
  lastBraiinsUpdate = 0;
  lastPriceUpdate = 0;
  workerCount = 0;
  
  // Initialize credentials empty (will be set via setCredentials)
  wifiSSID = "";
  wifiPassword = "";
  apiToken = "";
  
  braiinsData.hashrate_1h = 0;
  braiinsData.hashrate_24h = 0;
  braiinsData.hashrate_scoring = 0;
  braiinsData.workers_active = 0;
  braiinsData.workers_offline = 0;
  braiinsData.workers_disabled = 0;
  braiinsData.workers_total = 0;
  braiinsData.btc_rewards_today = 0;
  braiinsData.btc_unpaid = 0;
  braiinsData.btc_total_paid = 0;
  braiinsData.shares_valid = 0;
  braiinsData.shares_invalid = 0;
  braiinsData.connected = false;
  
  btcPrice.usd = 0;
  btcPrice.change_24h = 0;
  btcPrice.valid = false;
}

void APIManager::setCredentials(const char* ssid, const char* password, const char* token) {
  wifiSSID = String(ssid);
  wifiPassword = String(password);
  apiToken = String(token);
  Serial.printf("[API] Credentials set - SSID: %s\n", ssid);
}

bool APIManager::begin() {
  return connectWiFi();
}

bool APIManager::connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nWiFi connection failed!");
    return false;
  }
}

void APIManager::update() {
  unsigned long now = millis();
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    connectWiFi();
    return;
  }
  
  if (now - lastBraiinsUpdate >= POOL_REFRESH_RATE || lastBraiinsUpdate == 0) {
    Serial.println("Updating Braiins data...");
    if (fetchBraiinsData()) {
      lastBraiinsUpdate = now;
      braiinsData.last_update = now;
      braiinsData.connected = true;
      Serial.println("Braiins update SUCCESS!");
    } else {
      braiinsData.connected = false;
      Serial.println("Braiins update FAILED!");
    }
  }
  
  if (now - lastPriceUpdate >= PRICE_REFRESH_RATE || lastPriceUpdate == 0) {
    Serial.println("Updating BTC price...");
    if (fetchBTCPrice()) {
      lastPriceUpdate = now;
      btcPrice.last_update = now;
      btcPrice.valid = true;
      Serial.println("BTC price update SUCCESS!");
    } else {
      btcPrice.valid = false;
      Serial.println("BTC price update FAILED!");
    }
  }
}

void APIManager::forceUpdate() {
  lastBraiinsUpdate = 0;
  lastPriceUpdate = 0;
  update();
}

bool APIManager::fetchBraiinsData() {
  HTTPClient http;
  // Build the authenticated API URL: https://pool.braiins.com/accounts/profile/json/btc/
  String url = String("https://") + BRAIINS_API_HOST + BRAIINS_API_PATH;
  
  Serial.print("Fetching: ");
  Serial.println(url);
  
  http.begin(url);
  http.setTimeout(15000);
  http.addHeader("User-Agent", "ESP32-BraiinsHUD/1.0");
  http.addHeader("SlushPool-Auth-Token", apiToken.c_str());
  
  int httpCode = http.GET();
  Serial.print("HTTP code: ");
  Serial.println(httpCode);
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("Got ");
    Serial.print(payload.length());
    Serial.println(" bytes");
    http.end();
    return parseJsonResponse(payload);
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(httpCode));
    http.end();
    return false;
  }
}

bool APIManager::parseJsonResponse(String json) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);
  
  if (error) {
    Serial.print("JSON error: ");
    Serial.println(error.c_str());
    Serial.println(json.substring(0, 200));
    return false;
  }
  
  // Debug: print JSON structure
  // Serial.println("=== JSON DEBUG ===");
  // Serial.println(json);
  // Serial.println("==================");
  
  braiinsData.workers_active = 0;
  
  if (doc["btc"].is<JsonObject>()) {
    JsonObject btc = doc["btc"];
    
    // Get unit and calculate multiplier (API returns Gh/s)
    String unit = btc["hash_rate_unit"] | "Gh/s";
    float mult = 1.0;
    if (unit == "Gh/s") mult = 0.001;      // Convert Gh/s to Th/s
    else if (unit == "Ph/s") mult = 1000.0; // Convert Ph/s to Th/s
    else if (unit == "Th/s") mult = 1.0;
    
    // Use 5m rate as current (total of all workers)
    braiinsData.hashrate_1h = (btc["hash_rate_5m"] | 0.0f) * mult;
    braiinsData.hashrate_24h = (btc["hash_rate_24h"] | 0.0f) * mult;
    braiinsData.hashrate_scoring = (btc["hash_rate_60m"] | 0.0f) * mult;
    
    // Workers count from new API format
    braiinsData.workers_active = btc["ok_workers"] | 0;
    braiinsData.workers_offline = btc["off_workers"] | 0;
    braiinsData.workers_disabled = btc["dis_workers"] | 0;
    braiinsData.workers_total = braiinsData.workers_active + braiinsData.workers_offline + braiinsData.workers_disabled;
    
    // Rewards
    String todayReward = btc["today_reward"] | "0";
    String currentBalance = btc["current_balance"] | "0";
    String allTimeReward = btc["all_time_reward"] | "0";
    
    braiinsData.btc_rewards_today = todayReward.toFloat();
    braiinsData.btc_unpaid = currentBalance.toFloat();
    braiinsData.btc_total_paid = allTimeReward.toFloat();
    
    // Shares
    braiinsData.shares_valid = btc["shares_24h"] | 0;
    braiinsData.shares_invalid = 0;
  }
  
  Serial.print("Hashrate: ");
  Serial.print(braiinsData.hashrate_1h);
  Serial.print(" TH/s, Workers: ");
  Serial.println(braiinsData.workers_active);
  return true;
}

bool APIManager::fetchBTCPrice() {
  HTTPClient http;
  String url = String("https://") + BTC_PRICE_HOST + BTC_PRICE_PATH;
  
  Serial.print("Fetching: ");
  Serial.println(url);
  
  http.begin(url);
  http.setTimeout(15000);
  
  int httpCode = http.GET();
  Serial.print("HTTP code: ");
  Serial.println(httpCode);
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    JsonDocument doc;
    
    if (deserializeJson(doc, payload)) {
      http.end();
      return false;
    }
    
    btcPrice.usd = doc["bitcoin"]["usd"] | 0.0f;
    btcPrice.change_24h = doc["bitcoin"]["usd_24h_change"] | 0.0f;
    
    Serial.print("BTC: $");
    Serial.println(btcPrice.usd);
    http.end();
    return true;
  }
  
  http.end();
  return false;
}

BraiinsData APIManager::getBraiinsData() { return braiinsData; }
BTCPrice APIManager::getBTCPrice() { return btcPrice; }
WorkerData* APIManager::getWorkers() { return workers; }
int APIManager::getWorkerCount() { return workerCount; }
bool APIManager::isWiFiConnected() { return WiFi.status() == WL_CONNECTED; }
bool APIManager::isBraiinsConnected() { return braiinsData.connected; }
