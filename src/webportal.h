#ifndef WEBPORTAL_H
#define WEBPORTAL_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "settings.h"

class WebPortal {
  private:
    WebServer server;
    DNSServer dnsServer;
    SettingsManager* settingsManager;
    bool portalActive;
    bool configSaved;
    
    void handleRoot();
    void handleSave();
    void handleScan();
    void handleNotFound();
    String getConfigPage();
    String getSuccessPage();
    
  public:
    WebPortal(SettingsManager* settings);
    
    // Start captive portal (AP mode)
    void startPortal();
    
    // Stop portal
    void stopPortal();
    
    // Handle client requests (call in loop)
    void handle();
    
    // Check if config was saved
    bool isConfigSaved() { return configSaved; }
    
    // Check if portal is active
    bool isActive() { return portalActive; }
};

#endif
