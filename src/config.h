#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// WIFI CONFIGURATION
// ============================================
#define WIFI_SSID "Freebox-66E1B2"        // Change this
#define WIFI_PASSWORD "Alexandre" // Change this

// ============================================
// BRAIINS POOL CONFIGURATION
// ============================================
#define BRAIINS_USERNAME "Silexperience"
#define BRAIINS_WORKER "oct"
#define BRAIINS_API_TOKEN "FmUIuSYMTI3kt9VK"
// Authenticated API endpoint
#define BRAIINS_API_HOST "pool.braiins.com"
#define BRAIINS_API_PATH "/accounts/profile/json/btc/"
// Note: Full URL will be: https://pool.braiins.com/accounts/profile/json/btc/

// ============================================
// BTC PRICE API
// ============================================
#define BTC_PRICE_HOST "api.coingecko.com"
#define BTC_PRICE_PATH "/api/v3/simple/price?ids=bitcoin&vs_currencies=usd"

// ============================================
// REFRESH RATES (milliseconds)
// ============================================
#define POOL_REFRESH_RATE 30000   // 30 seconds - Braiins Pool data
#define PRICE_REFRESH_RATE 60000  // 60 seconds - BTC price
#define ANIMATION_FRAME_RATE 50   // 50ms = 20 FPS
#define SCREEN_DURATION 30000     // 30 seconds per screen
#define ANIMATION_INTERVAL 33     // 33ms = ~30 FPS
#define API_UPDATE_INTERVAL 30000 // 30 seconds

// ============================================
// CYBERPUNK COLOR PALETTE
// ============================================
#define COLOR_BG 0x0000           // Black background
#define COLOR_PRIMARY 0xFD20      // Orange (#FF6600)
#define COLOR_SECONDARY 0xF800    // Red (#FF0000)
#define COLOR_ACCENT 0x07FF       // Cyan (#00FFFF)
#define COLOR_TEXT 0xFFFF         // White
#define COLOR_DIM 0x4208          // Dark gray
#define COLOR_GLOW 0xFDA0         // Orange glow
#define COLOR_SUCCESS 0x07E0      // Green
#define COLOR_WARNING 0xFFE0      // Yellow

// ============================================
// SCREEN DEFINITIONS
// ============================================
#define SCREEN_NEURAL 0    // Main dashboard
#define SCREEN_WORKERS 1   // Workers list
#define SCREEN_STATS 2     // 24h statistics
#define SCREEN_VAULT 3     // Rewards & earnings

// ============================================
// ANIMATION SETTINGS
// ============================================
#define NUM_BUBBLES 8         // Floating bubbles on edges
#define NUM_PARTICLES 20      // Particle effect count
#define NUM_PULSAR_RINGS 3    // Concentric pulsing rings
#define SCANLINE_SPEED 2      // CRT scanline animation speed

// ============================================
// DISPLAY SETTINGS
// ============================================
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define CENTER_X 120
#define CENTER_Y 120

// ============================================
// TOUCH SETTINGS (CST816S)
// ============================================
#define TOUCH_SDA 4
#define TOUCH_SCL 5
#define TOUCH_INT 0
#define TOUCH_RST 1

// ============================================
// SLEEP MODE SETTINGS
// ============================================
#define SLEEP_TIMEOUT 60000       // 60 seconds before dimming
#define BRIGHTNESS_NORMAL 255     // Normal brightness
#define BRIGHTNESS_DIM 30         // Dimmed brightness (sleep mode)

#endif
