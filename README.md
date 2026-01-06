# 🔥 BRAIINS HUD - CYBERPUNK EDITION

<div align="center">

![Cyberpunk HUD](https://img.shields.io/badge/Cyberpunk-HUD-orange?style=for-the-badge&logo=visual-studio-code&logoColor=white)
![ESP32-C3](https://img.shields.io/badge/ESP32--C3-1.28%22%20Round-blue?style=for-the-badge&logo=espressif&logoColor=white)
![Bitcoin Mining](https://img.shields.io/badge/Bitcoin-Mining-yellow?style=for-the-badge&logo=bitcoin&logoColor=white)

**🚀 Futuristic Bitcoin Mining Dashboard for ESP32-C3**

*A stunning cyberpunk-themed real-time display that transforms your mining rig into a sci-fi masterpiece*

[🌐 Web Flasher](https://silexperience210.github.io/ESP32_BraiinsHUD_Cyberpunk/) • [📖 Documentation](#-documentation) • [🛠️ Hardware](#-hardware-requirements) • [⚡ Quick Start](#-quick-start)

---

## ✨ What is Braiins HUD?

Transform your ESP32-C3 with a 1.28" round display into a mesmerizing **cyberpunk Bitcoin mining monitor**. This project combines:

- 🎨 **Stunning Visual Effects**: Floating bubbles, pulsing rings, particle trails, CRT scanlines, and glitch effects
- 📊 **Real-Time Data**: Live hashrate, BTC price, worker status, and earnings tracking
- 🔄 **Auto-Rotating Screens**: 4 different dashboards cycling every 5 seconds
- 🌐 **Easy Flashing**: Browser-based installation with our Web Flasher

Perfect for miners who want to showcase their rig with style!

---

## ⚡ Quick Start (Choose Your Path)

### 🚀 Option 1: Web Flasher (Recommended - 2 Minutes)

**No software installation required!**

1. **Visit**: [https://silexperience210.github.io/ESP32_BraiinsHUD_Cyberpunk/](https://silexperience210.github.io/ESP32_BraiinsHUD_Cyberpunk/)
2. **Connect** your ESP32-C3 via USB-C
3. **Click "Connect"** → Select your device
4. **Wait** for automatic flashing (2-3 minutes)
5. **Enjoy** your cyberpunk dashboard!

*That's it! Works on any modern browser.*

### 🔧 Option 2: Manual Installation (For Developers)

See [Manual Installation](#-manual-installation) below for VS Code + PlatformIO setup.

---

## 📋 Table of Contents

- [✨ Features](#-features)
- [🎮 Demo](#-demo)
- [🛠️ Hardware Requirements](#-hardware-requirements)
- [📦 Manual Installation](#-manual-installation)
- [🎨 Usage](#-usage)
- [🔧 Configuration](#-configuration)
- [📊 API Integration](#-api-integration)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## 🎮 Demo

*Imagine your mining rig displaying:*

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

*With floating bubbles, pulsing rings, and smooth transitions between screens!*

---

## ✨ Features

### 🎨 Cyberpunk Visual Effects
- **8 Floating Bubbles** orbiting the display edges
- **3 Pulsing Energy Rings** with breathing animation
- **20 Particle Trails** with smooth fade effects
- **CRT Scanlines** for retro gaming aesthetic
- **Random Glitch Effects** (RGB color shifts)
- **Hexagonal Corners** with neon glow
- **Breathing Border Glow** in orange/cyan
- **Data Stream Animations** simulating code flow

### 📊 4 Auto-Rotating Screens

| Screen | Description | Visual Style |
|--------|-------------|--------------|
| **NEURAL INTERFACE** | Main dashboard with hashrate, BTC price, workers | Central data hub with rings |
| **HASHRATE GRID** | Individual worker hashrates | Grid layout with progress bars |
| **24H ANALYTICS** | Performance statistics and graphs | Charts and metrics |
| **REWARDS MATRIX** | Daily earnings and projections | Vault-style with BTC symbols |

### 🚀 Technical Capabilities
- ⚡ **Real-time Braiins Pool API** integration
- 💰 **Live BTC price** from CoinGecko
- 🔄 **Auto-reconnect WiFi** handling
- 🎬 **20 FPS smooth animations**
- 🎨 **Cyberpunk color palette**: Orange/Red/Cyan on Black
- 👥 **Worker monitoring** with status indicators
- 📈 **24h hashrate tracking**
- 💵 **Earnings calculator** (daily/monthly projections)

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
git clone https://github.com/Silexperience210/ESP32_BraiinsHUD_Cyberpunk.git
cd ESP32_BraiinsHUD_Cyberpunk
```

Or download and extract the ZIP file.

### 3. Configure WiFi and Braiins

Edit `src/config.h`:

```cpp
// WiFi Configuration
#define WIFI_SSID "YourWiFiNetwork"
#define WIFI_PASSWORD "YourWiFiPassword"

// Braiins Pool Configuration
#define BRAIINS_USER "YourBraiinsUsername"  // Your Braiins Pool username
#define BRAIINS_WORKER "worker1"            // Your worker name
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
3. Test API manually: `https://pool.braiins.com/accounts/profile/json/btc/YourUsername/`
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
