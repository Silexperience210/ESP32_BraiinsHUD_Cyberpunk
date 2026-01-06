// User_Setup.h pour ESP32-2424S012C
// ESP32-C3 avec écran rond GC9A01 240x240

#define USER_SETUP_LOADED

// Driver GC9A01 pour écran rond
#define GC9A01_DRIVER

// Taille écran
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// Pins ESP32-C3 pour ESP32-2424S012
#define TFT_MOSI 7
#define TFT_SCLK 6
#define TFT_CS   10
#define TFT_DC   2
#define TFT_RST  -1   // Connecté au reset de l'ESP32
#define TFT_BL   3    // Backlight

// Fréquence SPI (plus basse pour stabilité)
#define SPI_FREQUENCY  40000000

// Autres options
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// Pas d'utilisation de DMA sur ESP32-C3
// #define USE_DMA
