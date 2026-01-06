#ifndef SCREENS_H
#define SCREENS_H

#include "display.h"
#include "config.h"
#include "api.h"
#include "animations.h"

// ============================================
// SCREEN MANAGER CLASS
// ============================================

class ScreenManager {
  private:
    LGFX* tft;
    APIManager* api;
    Animations* anim;
    
    int currentScreen;
    unsigned long lastScreenSwitch;
    
    // Helper functions for drawing UI elements
    void drawHeader(String title, uint16_t color);
    void drawFooter();
    void drawStatusBar();
    void formatHashrate(float thps, char* buffer);
    void formatBTC(float btc, char* buffer);
    void formatUSD(float usd, char* buffer);
    
  public:
    ScreenManager(LGFX* display, APIManager* apiManager, Animations* animations);
    
    // Initialize screens
    void begin();
    
    // Update and render current screen
    void update();
    
    // Individual screen renders
    void drawScreenNeural();      // Screen 0: Main dashboard
    void drawScreenWorkers();     // Screen 1: Workers list
    void drawScreenStats();       // Screen 2: 24h statistics
    void drawScreenVault();       // Screen 3: Rewards & earnings
    
    // Screen navigation
    void nextScreen();
    void setScreen(int screen);
    int getCurrentScreen() { return currentScreen; }
};

#endif
