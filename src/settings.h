#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>
#include <Arduino.h>

// ============================================
// Default values (used if not configured)
// ============================================
#define DEFAULT_AP_SSID "BraiinsHUD-Setup"
#define DEFAULT_AP_PASS "braiins123"
#define DEFAULT_BRIGHTNESS 255
#define DEFAULT_SCREEN_DURATION 30

// ============================================
// Settings structure
// ============================================
struct DeviceSettings {
  char wifi_ssid[64];
  char wifi_password[64];
  char braiins_token[64];
  char braiins_username[32];
  char worker_name[32];      // Target worker to monitor (empty = show aggregate stats)
  uint8_t brightness;
  uint16_t screen_duration;  // seconds
  bool configured;           // true if first setup is done
};

// ============================================
// Settings Manager Class
// ============================================
class SettingsManager {
  private:
    Preferences prefs;
    DeviceSettings settings;
    
  public:
    SettingsManager();
    
    // Load settings from flash
    bool load();
    
    // Save settings to flash
    bool save();
    
    // Reset to factory defaults
    void reset();
    
    // Check if device is configured
    bool isConfigured();
    
    // Getters
    const char* getWifiSSID() { return settings.wifi_ssid; }
    const char* getWifiPassword() { return settings.wifi_password; }
    const char* getBraiinsToken() { return settings.braiins_token; }
    const char* getBraiinsUsername() { return settings.braiins_username; }
    const char* getWorkerName() { return settings.worker_name; }
    uint8_t getBrightness() { return settings.brightness; }
    uint16_t getScreenDuration() { return settings.screen_duration; }

    // Setters
    void setWifiSSID(const char* ssid);
    void setWifiPassword(const char* password);
    void setBraiinsToken(const char* token);
    void setBraiinsUsername(const char* username);
    void setWorkerName(const char* name);
    void setBrightness(uint8_t brightness);
    void setScreenDuration(uint16_t duration);
    void setConfigured(bool configured);
    
    // Get full settings struct
    DeviceSettings* getSettings() { return &settings; }
};

#endif
