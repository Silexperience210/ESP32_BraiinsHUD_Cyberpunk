#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "display.h"
#include "config.h"

// ============================================
// ANIMATION STATE STRUCTURES
// ============================================

struct Bubble {
  float x, y;
  float vx, vy;
  float radius;
  float phase;
  uint16_t color;
};

struct Particle {
  float x, y;
  float vx, vy;
  float life;
  uint16_t color;
  bool active;
};

struct PulsarRing {
  float radius;
  float alpha;
  float speed;
  uint16_t color;
};

// ============================================
// ANIMATION MANAGER CLASS
// ============================================

class Animations {
  private:
    LGFX* tft;
    
    // Animation states
    Bubble bubbles[NUM_BUBBLES];
    Particle particles[NUM_PARTICLES];
    PulsarRing pulsarRings[NUM_PULSAR_RINGS];
    
    float pulsePhase;
    int scanlineY;
    unsigned long glitchTimer;
    bool glitchActive;
    
    // Helper functions
    void initBubbles();
    void initParticles();
    void initPulsarRings();
    
  public:
    Animations(LGFX* display);
    
    // Initialize all animations
    void begin();
    
    // Update animation states (call every frame)
    void update();
    
    // Individual animation renders
    void drawFloatingBubbles();
    void drawPulsarRings();
    void drawParticles();
    void drawScanlines();
    void drawGlitchEffect();
    void drawHexCorners();
    void drawBreathingGlow();
    void drawDataStreams();
    
    // Transition effects
    void transitionWarp();
    void transitionSlide(int direction);
    void transitionFade();
    
    // Utility effects
    void drawCircularGauge(int x, int y, int radius, float percent, uint16_t color);
    void drawHexagon(int x, int y, int size, uint16_t color);
    void drawGlowText(int x, int y, String text, uint16_t color, uint8_t font);
    void drawPulsingIcon(int x, int y, const char* icon, uint16_t color);
};

#endif
