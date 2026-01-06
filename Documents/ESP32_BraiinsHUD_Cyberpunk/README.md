# 🔥 BRAIINS HUD - CYBERPUNK EDITION

**ESP32-C3 1.28" Round Display - Bitcoin Mining Dashboard**

![Version](https://img.shields.io/badge/version-1.0-orange)
![Platform](https://img.shields.io/badge/platform-ESP32--C3-blue)
![License](https://img.shields.io/badge/license-MIT-green)

A futuristic, cyberpunk-styled real-time Bitcoin mining dashboard for ESP32-C3 with GC9A01 240x240 round display. Monitors Braiins Pool hashrate, workers, and earnings with stunning visual effects.

Created by **Silexperience** (CyberHornet Team)

---

## ✨ Features

### 📊 4 Auto-Rotating Screens (5 seconds each)

1. **NEURAL INTERFACE** - Main dashboard with current hashrate, BTC price, and active workers
2. **HASHRATE GRID** - Detailed worker list with individual hashrates
3. **24H ANALYTICS** - Statistics and performance graphs
4. **REWARDS MATRIX** - Today's earnings and monthly projections

### 🎨 Cyberpunk Visual Effects

- **8 Floating Bubbles** on screen edges
- **3 Pulsing Rings** around the display
- **20 Particle Trails** with fade effects
- **CRT Scanlines** for retro aesthetic
- **Random Glitch Effects** (RGB shifts)
- **Hexagonal Corners** with glow
- **Breathing Border Glow**
- **Data Stream Animations**

### 🚀 Technical Features

- Real-time Braiins Pool API integration
- Live BTC price from CoinGecko
- Auto-reconnect WiFi handling
- 20 FPS smooth animations
- Color palette: Orange/Red/Cyan on Black
- Worker status monitoring
- 24h hashrate tracking
- Earnings calculator (daily/monthly)

---

## 🛠️ Hardware Requirements

### Required Components

- **ESP32-C3 SuperMini** with 1.28" GC9A01 Round Display (240x240)
  - Model: UEDX24240013-MD50E by VIEWE
  - Purchase link: [AliExpress ESP32-C3 1.28" Round Display](https://fr.aliexpress.com/item/1005008268789940.html)
- **USB-C Cable** for programming and power

### Display Specifications

- **Panel Type**: IPS TFT
- **Resolution**: 240x240 pixels
- **Driver IC**: GC9A01
- **Color Depth**: 16-bit (65K colors)
- **Interface**: SPI

---

## 📦 Installation

### 1. Software Requirements

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
- USB drivers for ESP32-C3 (usually automatic)

### 2. Clone/Download Project

```bash
git clone https://github.com/yourusername/ESP32_BraiinsHUD_Cyberpunk.git
cd ESP32_BraiinsHUD_Cyberpunk
```

Or download and extract the ZIP file.

### 3. Configure WiFi and Braiins

Edit `src/config.h`:

```cpp
// WiFi Configuration
#define WIFI_SSID "YourWiFiNetwork"
#define WIFI_PASSWORD "YourWiFiPassword"

// Braiins Pool (username is already set to "Silexperience")
// If you need to change the worker name, edit:
#define BRAIINS_WORKER "oct"  // Change to your worker name
```

### 4. Open in PlatformIO

1. Open Visual Studio Code
2. Click `File` → `Open Folder`
3. Select the `ESP32_BraiinsHUD_Cyberpunk` folder
4. PlatformIO will automatically detect the project

### 5. Build and Upload

1. Connect your ESP32-C3 via USB-C
2. Click the **Upload** button (→) in PlatformIO toolbar
3. Wait for compilation and upload (2-3 minutes first time)
4. The device will reboot and show the boot screen

### Alternative: Web Flasher (No Software Installation Required)

For users who prefer not to install development tools, you can flash the firmware directly from your web browser:

1. Visit the [Web Flasher Page](https://Silexperience210.github.io/ESP32_BraiinsHUD_Cyberpunk/webflasher/)
2. Connect your ESP32-C3 device via USB
3. Click the "Connect" button and select your device
4. The firmware will be automatically downloaded and flashed

**Note:** Web flashing requires a modern browser with Web Serial support (Chrome/Edge recommended).

---

## 🎮 Usage

### Auto-Rotation Mode

The display automatically cycles through all 4 screens every 5 seconds:

```
NEURAL → WORKERS → STATS → VAULT → (repeat)
```

### Screen Descriptions

#### 🔴 Screen 1: NEURAL INTERFACE

```
╔═══════════════════════╗
║  ⬡ BRAIINS NEURAL ⬡   ║
║                       ║
║     ⟨ 1.2 TH/s ⟩     ║
║   [████████░░] 85%    ║
║                       ║
║  ₿ $94,523  ↑ 2.3%   ║
║  ⚡ 3 WORKERS ACTIVE   ║
╚═══════════════════════╝
```

Shows:
- Current hashrate with circular gauge
- BTC price with 24h change
- Active worker count
- Pulsing rings and floating bubbles

#### 🟠 Screen 2: HASHRATE GRID

```
╔═══════════════════════╗
║  ⬢ HASHRATE GRID ⬢    ║
║                       ║
║ ⬡ OCT      1.2 TH/s   ║
║ ⬡ BITAXE-1  485 GH/s  ║
║ ⬡ BITAXE-2  502 GH/s  ║
║                       ║
║  [TOTAL: 2.18 TH/s]   ║
╚═══════════════════════╝
```

Shows:
- Individual worker names
- Per-worker hashrate
- Online/offline status
- Total combined hashrate
- Particle effects and data streams

#### 🔵 Screen 3: QUANTUM STATS

```
╔═══════════════════════╗
║  ◬ 24H ANALYTICS ◬     ║
║                       ║
║   ╱╲  ╱╲    ╱╲       ║
║  ╱  ╲╱  ╲  ╱  ╲      ║
║                       ║
║  AVG: 1.15 TH/s       ║
║  SHARES: 1,234        ║
╚═══════════════════════╝
```

Shows:
- 24h hashrate graph
- Average hashrate
- Valid shares count
- Invalid shares (if any)
- Glitch effects

#### 🟡 Screen 4: SATOSHI VAULT

```
╔═══════════════════════╗
║  ⟁ REWARDS MATRIX ⟁    ║
║                       ║
║  TODAY   0.00012 ₿    ║
║          $11.34       ║
║                       ║
║  MONTH   0.00360 ₿    ║
║  (EST)   $340.28      ║
╚═══════════════════════╝
```

Shows:
- Today's BTC earnings
- USD value of today's earnings
- Monthly projection (×30)
- Breathing glow effect

---

## 🔧 Troubleshooting

### Display Issues

**Problem**: Blank screen or garbled display

**Solutions**:
- Check USB cable connection
- Verify pinout in `platformio.ini`:
  ```ini
  -D TFT_MOSI=7
  -D TFT_SCLK=6
  -D TFT_CS=10
  -D TFT_DC=2
  -D TFT_BL=3
  ```
- Try different rotation values in `main.cpp`:
  ```cpp
  tft.setRotation(0);  // Try 0, 1, 2, or 3
  ```

### WiFi Connection Failed

**Problem**: "WIFI FAILED!" on screen

**Solutions**:
1. Double-check `WIFI_SSID` and `WIFI_PASSWORD` in `config.h`
2. Ensure 2.4GHz WiFi (ESP32-C3 doesn't support 5GHz)
3. Check Serial Monitor (115200 baud) for error messages
4. Try moving closer to router

### No Braiins Data

**Problem**: Pool status shows red/offline

**Solutions**:
1. Verify your Braiins Pool username in `config.h`
2. Check that you have active workers on Braiins Pool
3. Test API manually: `https://pool.braiins.com/accounts/profile/json/btc/Silexperience/`
4. Wait 30 seconds for first data refresh

### Slow Performance / Low FPS

**Problem**: Animations are choppy

**Solutions**:
1. Increase `ANIMATION_FRAME_RATE` in `config.h` (default 50ms)
2. Reduce `NUM_PARTICLES` or `NUM_BUBBLES`
3. Disable some animation effects in screens

---

## ⚙️ Customization

### Change Colors

Edit color definitions in `src/config.h`:

```cpp
#define COLOR_PRIMARY 0xFD20     // Orange
#define COLOR_SECONDARY 0xF800   // Red
#define COLOR_ACCENT 0x07FF      // Cyan
```

Use this color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/

### Adjust Screen Duration

In `src/config.h`:

```cpp
#define SCREEN_DURATION 5000  // 5 seconds (5000ms)
```

### Change Refresh Rates

```cpp
#define POOL_REFRESH_RATE 30000   // Braiins data (30 sec)
#define PRICE_REFRESH_RATE 60000  // BTC price (60 sec)
```

### Modify Animations

Edit `src/animations.cpp` to tweak:
- Bubble count: `NUM_BUBBLES` (default 8)
- Particle count: `NUM_PARTICLES` (default 20)
- Pulsar rings: `NUM_PULSAR_RINGS` (default 3)
- Scanline speed: `SCANLINE_SPEED` (default 2)

---

## 📡 API Documentation

### Braiins Pool API

**Endpoint**: `https://pool.braiins.com/accounts/profile/json/btc/{username}/`

**Data Retrieved**:
- Current hashrate (1h, 24h, scoring)
- Worker list with individual hashrates
- Share statistics (valid/invalid)
- Reward data (today, unpaid, total paid)

**Rate Limit**: No official limit, refreshes every 30 seconds

### CoinGecko API

**Endpoint**: `https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd`

**Data Retrieved**:
- Current BTC price in USD
- 24h price change percentage

**Rate Limit**: 10-50 calls/minute (free tier)

---

## 🗂️ Project Structure

```
ESP32_BraiinsHUD_Cyberpunk/
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp           # Main program loop
│   ├── config.h           # WiFi & API configuration
│   ├── animations.cpp     # Visual effects implementation
│   ├── animations.h       # Animation class declarations
│   ├── screens.cpp        # 4 screen implementations
│   ├── screens.h          # Screen manager class
│   ├── api.cpp           # Braiins Pool & BTC price API
│   └── api.h             # API manager class
└── README.md             # This file
```

---

## 🚀 Future Enhancements

Planned features for v2.0:

- [ ] Touch input support (if hardware allows)
- [ ] Manual screen selection
- [ ] Temperature monitoring
- [ ] Pool switching (Braiins/ViaBTC/F2Pool)
- [ ] Alert notifications (low hashrate, worker offline)
- [ ] Custom worker icons
- [ ] More animation presets
- [ ] OTA firmware updates
- [ ] Web config portal

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📜 License

This project is licensed under the MIT License - see below:

```
MIT License

Copyright (c) 2024 Silexperience

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Credits

- **Silexperience** - Main developer
- **CyberHornet Team** - Support and testing
- **Braiins Pool** - Mining pool API
- **CoinGecko** - BTC price API
- **TFT_eSPI Library** - Display driver by Bodmer
- **ArduinoJson** - JSON parsing by Benoit Blanchon

---

## 📞 Support

- **GitHub Issues**: Report bugs or request features
- **Twitter/X**: [@Silexperience](https://twitter.com/Silexperience)
- **Discord**: CyberHornet Community

---

## ⚡ Show Your Support

If you found this project useful:

- ⭐ Star the repository
- 🐦 Share on Twitter/X
- ₿ Donate: (your BTC address here)
- ⚡ Lightning: (your Lightning address here)

---

**Built with 🔥 by the Bitcoin plebs miners community**

*"Not your keys, not your coins. Not your hashrate, not your bitcoin."*
