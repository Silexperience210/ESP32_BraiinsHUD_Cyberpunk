#include "settings.h"

SettingsManager::SettingsManager() {
  // Initialize with defaults
  memset(&settings, 0, sizeof(settings));
  settings.brightness = DEFAULT_BRIGHTNESS;
  settings.screen_duration = DEFAULT_SCREEN_DURATION;
  settings.configured = false;
}

bool SettingsManager::load() {
  prefs.begin("braiins-hud", true);  // Read-only mode
  
  settings.configured = prefs.getBool("configured", false);
  
  if (settings.configured) {
    String ssid = prefs.getString("wifi_ssid", "");
    String pass = prefs.getString("wifi_pass", "");
    String token = prefs.getString("api_token", "");
    String user = prefs.getString("username", "Miner");
    
    strncpy(settings.wifi_ssid, ssid.c_str(), sizeof(settings.wifi_ssid) - 1);
    strncpy(settings.wifi_password, pass.c_str(), sizeof(settings.wifi_password) - 1);
    strncpy(settings.braiins_token, token.c_str(), sizeof(settings.braiins_token) - 1);
    strncpy(settings.braiins_username, user.c_str(), sizeof(settings.braiins_username) - 1);
    
    settings.brightness = prefs.getUChar("brightness", DEFAULT_BRIGHTNESS);
    settings.screen_duration = prefs.getUShort("screen_dur", DEFAULT_SCREEN_DURATION);
  }
  
  prefs.end();
  
  Serial.printf("[SETTINGS] Loaded - Configured: %s\n", settings.configured ? "YES" : "NO");
  if (settings.configured) {
    Serial.printf("[SETTINGS] SSID: %s\n", settings.wifi_ssid);
    Serial.printf("[SETTINGS] User: %s\n", settings.braiins_username);
  }
  
  return settings.configured;
}

bool SettingsManager::save() {
  prefs.begin("braiins-hud", false);  // Read-write mode
  
  prefs.putBool("configured", settings.configured);
  prefs.putString("wifi_ssid", settings.wifi_ssid);
  prefs.putString("wifi_pass", settings.wifi_password);
  prefs.putString("api_token", settings.braiins_token);
  prefs.putString("username", settings.braiins_username);
  prefs.putUChar("brightness", settings.brightness);
  prefs.putUShort("screen_dur", settings.screen_duration);
  
  prefs.end();
  
  Serial.println("[SETTINGS] Saved to flash!");
  return true;
}

void SettingsManager::reset() {
  prefs.begin("braiins-hud", false);
  prefs.clear();
  prefs.end();
  
  memset(&settings, 0, sizeof(settings));
  settings.brightness = DEFAULT_BRIGHTNESS;
  settings.screen_duration = DEFAULT_SCREEN_DURATION;
  settings.configured = false;
  
  Serial.println("[SETTINGS] Factory reset complete!");
}

bool SettingsManager::isConfigured() {
  return settings.configured;
}

void SettingsManager::setWifiSSID(const char* ssid) {
  strncpy(settings.wifi_ssid, ssid, sizeof(settings.wifi_ssid) - 1);
}

void SettingsManager::setWifiPassword(const char* password) {
  strncpy(settings.wifi_password, password, sizeof(settings.wifi_password) - 1);
}

void SettingsManager::setBraiinsToken(const char* token) {
  strncpy(settings.braiins_token, token, sizeof(settings.braiins_token) - 1);
}

void SettingsManager::setBraiinsUsername(const char* username) {
  strncpy(settings.braiins_username, username, sizeof(settings.braiins_username) - 1);
}

void SettingsManager::setBrightness(uint8_t brightness) {
  settings.brightness = brightness;
}

void SettingsManager::setScreenDuration(uint16_t duration) {
  settings.screen_duration = duration;
}

void SettingsManager::setConfigured(bool configured) {
  settings.configured = configured;
}
