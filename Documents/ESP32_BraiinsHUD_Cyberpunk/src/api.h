#ifndef API_H
#define API_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "config.h"

// ============================================
// DATA STRUCTURES
// ============================================

struct BraiinsData {
  float hashrate_1h;           // Current hashrate (TH/s)
  float hashrate_24h;          // 24h average hashrate (TH/s)
  float hashrate_scoring;      // Scoring hashrate (TH/s)
  int workers_active;          // Number of active workers
  int workers_offline;         // Number of offline workers
  int workers_disabled;        // Number of disabled workers
  int workers_total;           // Total workers
  float btc_rewards_today;     // BTC mined today
  float btc_unpaid;            // Unpaid balance
  float btc_total_paid;        // Total paid
  int shares_valid;            // Valid shares
  int shares_invalid;          // Invalid/stale shares
  unsigned long last_update;   // Last successful update timestamp
  bool connected;              // Connection status
};

struct WorkerData {
  String name;
  float hashrate;
  bool online;
  unsigned long last_share;
};

struct BTCPrice {
  float usd;
  float change_24h;
  unsigned long last_update;
  bool valid;
};

// ============================================
// API MANAGER CLASS
// ============================================

class APIManager {
  private:
    // Note: WiFiClientSecure is created locally in each fetch function
    // to avoid socket reuse issues
    BraiinsData braiinsData;
    BTCPrice btcPrice;
    WorkerData workers[10];  // Support up to 10 workers
    int workerCount;
    
    unsigned long lastBraiinsUpdate;
    unsigned long lastPriceUpdate;
    
    // Dynamic credentials
    String wifiSSID;
    String wifiPassword;
    String apiToken;
    
    bool fetchBraiinsData();
    bool fetchBTCPrice();
    bool parseJsonResponse(String json);
    
  public:
    APIManager();
    
    // Set credentials before begin()
    void setCredentials(const char* ssid, const char* password, const char* token);
    
    // Initialize WiFi and API client
    bool begin();
    
    // Connect to WiFi
    bool connectWiFi();
    
    // Update data (call regularly in loop)
    void update();
    
    // Force immediate update
    void forceUpdate();
    
    // Getters for data
    BraiinsData getBraiinsData();
    BTCPrice getBTCPrice();
    WorkerData* getWorkers();
    int getWorkerCount();
    
    // Connection status
    bool isWiFiConnected();
    bool isBraiinsConnected();
    
    // WiFi signal strength
    int getWiFiRSSI() { return WiFi.RSSI(); }
};

#endif
