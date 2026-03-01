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

// Individual worker monitoring (single-worker mode)
struct SingleWorkerData {
  char name[48];            // Full name from API: "username.workername"
  float hashrate_5m;        // 5-min hashrate (TH/s)
  float hashrate_1h;        // 60-min hashrate (TH/s)
  float hashrate_scoring;   // Scoring hashrate (TH/s)
  bool online;              // true = active, false = offline/disabled
  bool found;               // true if target worker was found in API response
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
    String targetWorkerName;  // Worker to monitor (empty = aggregate mode)

    // Per-worker data (single-worker mode)
    SingleWorkerData targetWorker;
    unsigned long lastWorkerUpdate;

    bool fetchBraiinsData();
    bool fetchBTCPrice();
    bool fetchWorkerData();
    bool parseJsonResponse(String& json);
    bool parseWorkerResponse(String& json);
    
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
    
    // Single-worker mode
    void setTargetWorker(const char* name);
    bool hasTargetWorker() { return targetWorkerName.length() > 0; }
    SingleWorkerData getTargetWorker() { return targetWorker; }

    // Connection status
    bool isWiFiConnected();
    bool isBraiinsConnected();

    // WiFi signal strength
    int getWiFiRSSI() { return WiFi.RSSI(); }
};

#endif
