#include "animations.h"
#include <math.h>

Animations::Animations(LGFX* display) {
  tft = display;
  pulsePhase = 0;
  scanlineY = 0;
  glitchTimer = 0;
  glitchActive = false;
}

void Animations::begin() {
  initBubbles();
  initParticles();
  initPulsarRings();
}

// ============================================
// INITIALIZATION FUNCTIONS
// ============================================

void Animations::initBubbles() {
  for (int i = 0; i < NUM_BUBBLES; i++) {
    // Position bubbles around the edges
    float angle = (TWO_PI / NUM_BUBBLES) * i;
    bubbles[i].x = CENTER_X + cos(angle) * 100;
    bubbles[i].y = CENTER_Y + sin(angle) * 100;
    bubbles[i].vx = (random(-100, 100) / 100.0) * 0.5;
    bubbles[i].vy = (random(-100, 100) / 100.0) * 0.5;
    bubbles[i].radius = random(3, 8);
    bubbles[i].phase = random(0, 628) / 100.0; // 0 to 2*PI
    bubbles[i].color = (i % 2 == 0) ? COLOR_PRIMARY : COLOR_ACCENT;
  }
}

void Animations::initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].x = random(0, SCREEN_WIDTH);
    particles[i].y = random(0, SCREEN_HEIGHT);
    particles[i].vx = random(-20, 20) / 10.0;
    particles[i].vy = random(-20, 20) / 10.0;
    particles[i].life = random(50, 100) / 100.0;
    particles[i].active = true;
    particles[i].color = COLOR_PRIMARY;
  }
}

void Animations::initPulsarRings() {
  for (int i = 0; i < NUM_PULSAR_RINGS; i++) {
    pulsarRings[i].radius = 40 + (i * 20);
    pulsarRings[i].alpha = 1.0 - (i * 0.3);
    pulsarRings[i].speed = 0.5 + (i * 0.2);
    pulsarRings[i].color = (i == 0) ? COLOR_PRIMARY : (i == 1) ? COLOR_SECONDARY : COLOR_ACCENT;
  }
}

// ============================================
// UPDATE FUNCTION (CALLED EVERY FRAME)
// ============================================

void Animations::update() {
  pulsePhase += 0.05;
  if (pulsePhase > TWO_PI) pulsePhase = 0;
  
  scanlineY += SCANLINE_SPEED;
  if (scanlineY >= SCREEN_HEIGHT) scanlineY = 0;
  
  // Update bubbles
  for (int i = 0; i < NUM_BUBBLES; i++) {
    bubbles[i].x += bubbles[i].vx;
    bubbles[i].y += bubbles[i].vy;
    bubbles[i].phase += 0.03;
    
    // Bounce off edges
    if (bubbles[i].x < 10 || bubbles[i].x > SCREEN_WIDTH - 10) bubbles[i].vx *= -1;
    if (bubbles[i].y < 10 || bubbles[i].y > SCREEN_HEIGHT - 10) bubbles[i].vy *= -1;
    
    // Keep bubbles near edges
    float distFromCenter = sqrt(pow(bubbles[i].x - CENTER_X, 2) + pow(bubbles[i].y - CENTER_Y, 2));
    if (distFromCenter < 80) {
      float angle = atan2(bubbles[i].y - CENTER_Y, bubbles[i].x - CENTER_X);
      bubbles[i].vx += cos(angle) * 0.1;
      bubbles[i].vy += sin(angle) * 0.1;
    }
  }
  
  // Update particles
  for (int i = 0; i < NUM_PARTICLES; i++) {
    if (particles[i].active) {
      particles[i].x += particles[i].vx;
      particles[i].y += particles[i].vy;
      particles[i].life -= 0.01;
      
      if (particles[i].life <= 0 || particles[i].x < 0 || particles[i].x > SCREEN_WIDTH || 
          particles[i].y < 0 || particles[i].y > SCREEN_HEIGHT) {
        // Respawn particle
        particles[i].x = random(0, SCREEN_WIDTH);
        particles[i].y = random(0, SCREEN_HEIGHT);
        particles[i].vx = random(-20, 20) / 10.0;
        particles[i].vy = random(-20, 20) / 10.0;
        particles[i].life = 1.0;
      }
    }
  }
  
  // Update pulsar rings
  for (int i = 0; i < NUM_PULSAR_RINGS; i++) {
    pulsarRings[i].radius += pulsarRings[i].speed;
    if (pulsarRings[i].radius > 120) {
      pulsarRings[i].radius = 40;
    }
  }
  
  // Random glitch trigger
  if (millis() - glitchTimer > 3000) {
    if (random(0, 100) < 10) { // 10% chance every 3 seconds
      glitchActive = true;
      glitchTimer = millis();
    }
  }
  if (millis() - glitchTimer > 100) {
    glitchActive = false;
  }
}

// ============================================
// INDIVIDUAL ANIMATION RENDERS
// ============================================

void Animations::drawFloatingBubbles() {
  for (int i = 0; i < NUM_BUBBLES; i++) {
    float pulse = sin(bubbles[i].phase) * 0.5 + 0.5;
    int r = bubbles[i].radius * (0.7 + pulse * 0.3);
    
    // Draw bubble with fade effect
    tft->drawCircle(bubbles[i].x, bubbles[i].y, r, bubbles[i].color);
    tft->drawCircle(bubbles[i].x, bubbles[i].y, r - 1, tft->color565(
      (bubbles[i].color >> 11) * pulse * 8,
      ((bubbles[i].color >> 5) & 0x3F) * pulse * 4,
      (bubbles[i].color & 0x1F) * pulse * 8
    ));
  }
}

void Animations::drawPulsarRings() {
  for (int i = 0; i < NUM_PULSAR_RINGS; i++) {
    int alpha = 255 * (1.0 - (pulsarRings[i].radius - 40) / 80.0);
    if (alpha > 0) {
      uint16_t color = tft->color565(
        ((pulsarRings[i].color >> 11) * alpha) >> 8,
        (((pulsarRings[i].color >> 5) & 0x3F) * alpha) >> 8,
        ((pulsarRings[i].color & 0x1F) * alpha) >> 8
      );
      tft->drawCircle(CENTER_X, CENTER_Y, pulsarRings[i].radius, color);
    }
  }
}

void Animations::drawParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    if (particles[i].active && particles[i].life > 0) {
      int alpha = particles[i].life * 255;
      uint16_t color = tft->color565(
        ((particles[i].color >> 11) * alpha) >> 8,
        (((particles[i].color >> 5) & 0x3F) * alpha) >> 8,
        ((particles[i].color & 0x1F) * alpha) >> 8
      );
      
      // Draw particle with trail
      tft->drawPixel(particles[i].x, particles[i].y, color);
      tft->drawPixel(particles[i].x - particles[i].vx, particles[i].y - particles[i].vy, 
                     tft->color565(
                       ((particles[i].color >> 11) * alpha / 2) >> 8,
                       (((particles[i].color >> 5) & 0x3F) * alpha / 2) >> 8,
                       ((particles[i].color & 0x1F) * alpha / 2) >> 8
                     ));
    }
  }
}

void Animations::drawScanlines() {
  for (int y = scanlineY; y < SCREEN_HEIGHT; y += 4) {
    tft->drawFastHLine(0, y, SCREEN_WIDTH, COLOR_DIM);
  }
}

void Animations::drawGlitchEffect() {
  if (glitchActive) {
    // Random horizontal shifts
    for (int i = 0; i < 5; i++) {
      int y = random(0, SCREEN_HEIGHT);
      // Simplified to just drawing colored lines
      tft->drawFastHLine(0, y, SCREEN_WIDTH, random(0, 2) ? COLOR_SECONDARY : COLOR_ACCENT);
    }
  }
}

void Animations::drawHexCorners() {
  int size = 15;
  int offset = 10;
  
  // Top-left
  drawHexagon(offset + size, offset + size, size, COLOR_PRIMARY);
  // Top-right
  drawHexagon(SCREEN_WIDTH - offset - size, offset + size, size, COLOR_ACCENT);
  // Bottom-left
  drawHexagon(offset + size, SCREEN_HEIGHT - offset - size, size, COLOR_SECONDARY);
  // Bottom-right
  drawHexagon(SCREEN_WIDTH - offset - size, SCREEN_HEIGHT - offset - size, size, COLOR_ACCENT);
}

void Animations::drawBreathingGlow() {
  float pulse = sin(pulsePhase) * 0.5 + 0.5;
  int alpha = 50 + (pulse * 100);
  
  uint16_t glowColor = tft->color565(
    ((COLOR_GLOW >> 11) * alpha) >> 8,
    (((COLOR_GLOW >> 5) & 0x3F) * alpha) >> 8,
    ((COLOR_GLOW & 0x1F) * alpha) >> 8
  );
  
  tft->drawRect(5, 5, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10, glowColor);
  tft->drawRect(6, 6, SCREEN_WIDTH - 12, SCREEN_HEIGHT - 12, glowColor);
}

void Animations::drawDataStreams() {
  static int streamY[5] = {0, 50, 100, 150, 200};
  
  for (int i = 0; i < 5; i++) {
    streamY[i] += 2;
    if (streamY[i] > SCREEN_HEIGHT) streamY[i] = 0;
    
    tft->setTextColor(COLOR_ACCENT, COLOR_BG);
    tft->setTextSize(1);
    tft->drawChar(random(33, 126), 5, streamY[i]);
    tft->drawChar(random(33, 126), SCREEN_WIDTH - 15, streamY[i]);
  }
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

void Animations::drawCircularGauge(int x, int y, int radius, float percent, uint16_t color) {
  // Background circle
  tft->drawCircle(x, y, radius, COLOR_DIM);
  tft->drawCircle(x, y, radius - 1, COLOR_DIM);
  
  // Arc for percentage
  int segments = percent * 360;
  for (int i = 0; i < segments; i += 2) {
    float angle = (i - 90) * DEG_TO_RAD;
    int x1 = x + cos(angle) * (radius - 3);
    int y1 = y + sin(angle) * (radius - 3);
    int x2 = x + cos(angle) * radius;
    int y2 = y + sin(angle) * radius;
    tft->drawLine(x1, y1, x2, y2, color);
  }
}

void Animations::drawHexagon(int x, int y, int size, uint16_t color) {
  for (int i = 0; i < 6; i++) {
    float angle1 = (TWO_PI / 6) * i;
    float angle2 = (TWO_PI / 6) * ((i + 1) % 6);
    
    int x1 = x + cos(angle1) * size;
    int y1 = y + sin(angle1) * size;
    int x2 = x + cos(angle2) * size;
    int y2 = y + sin(angle2) * size;
    
    tft->drawLine(x1, y1, x2, y2, color);
  }
}

void Animations::drawGlowText(int x, int y, String text, uint16_t color, uint8_t textSize) {
  // Draw glow effect (darker copies offset)
  uint16_t glowColor = tft->color565(
    ((color >> 11) * 100) >> 8,
    (((color >> 5) & 0x3F) * 100) >> 8,
    ((color & 0x1F) * 100) >> 8
  );
  
  tft->setTextColor(glowColor, COLOR_BG);
  tft->setTextSize(textSize);
  
  tft->drawString(text.c_str(), x - 1, y);
  tft->drawString(text.c_str(), x + 1, y);
  tft->drawString(text.c_str(), x, y - 1);
  tft->drawString(text.c_str(), x, y + 1);
  
  // Draw main text
  tft->setTextColor(color, COLOR_BG);
  tft->drawString(text.c_str(), x, y);
}

void Animations::drawPulsingIcon(int x, int y, const char* icon, uint16_t color) {
  float pulse = sin(pulsePhase * 2) * 0.3 + 0.7;
  uint16_t pulsedColor = tft->color565(
    ((color >> 11) * pulse * 8),
    (((color >> 5) & 0x3F) * pulse * 4),
    ((color & 0x1F) * pulse * 8)
  );
  
  tft->setTextColor(pulsedColor, COLOR_BG);
  tft->setTextSize(2);
  tft->drawString(icon, x, y);
}

// ============================================
// TRANSITION EFFECTS
// ============================================

void Animations::transitionWarp() {
  // Simple fade to black and back
  for (int i = 0; i < 10; i++) {
    tft->fillScreen(tft->color565(i * 2, i * 2, i * 2));
    delay(20);
  }
}

void Animations::transitionSlide(int direction) {
  // Simplified slide effect
  tft->fillScreen(COLOR_BG);
}

void Animations::transitionFade() {
  tft->fillScreen(COLOR_BG);
}
