# ⚡ QUICK START GUIDE

Get your ESP32-C3 Braiins HUD running in **5 minutes**!

---

## 🎯 Prerequisites

✅ ESP32-C3 with 1.28" GC9A01 display  
✅ USB-C cable  
✅ VS Code + PlatformIO installed  
✅ WiFi network (2.4GHz)  
✅ Braiins Pool account with active workers  

---

## 📝 Step 1: Configure WiFi

Open `src/config.h` and edit:

```cpp
#define WIFI_SSID "YourWiFiName"        // ← Change this
#define WIFI_PASSWORD "YourPassword"     // ← Change this
```

**That's it!** The username is already set to `Silexperience` and worker `oct`.

---

## 🔌 Step 2: Connect ESP32

1. Plug USB-C cable into ESP32-C3
2. Connect other end to your computer
3. Wait for drivers to install (Windows may take 1 minute)

---

## 🚀 Step 3: Upload Code

1. Open VS Code
2. Click **File** → **Open Folder**
3. Select `ESP32_BraiinsHUD_Cyberpunk` folder
4. Wait for PlatformIO to load (bottom bar will show "Ready")
5. Click the **→** (Upload) button in bottom toolbar
6. Wait 2-3 minutes for first compile + upload

---

## ✅ Step 4: Verify

You should see:

1. **Boot screen**: "BRAIINS HUD CYBERPUNK EDITION"
2. **WiFi connection**: Indicator shows green
3. **4 screens auto-rotating** every 5 seconds:
   - Neural Interface (hashrate + BTC price)
   - Hashrate Grid (workers list)
   - Quantum Stats (24h graph)
   - Satoshi Vault (earnings)

---

## 🆘 Troubleshooting

### "WiFi Failed" Error

- Check SSID and password in `config.h`
- Make sure it's 2.4GHz WiFi (not 5GHz)
- Try moving closer to router

### Blank Screen

- Check USB cable is data-capable (not charge-only)
- Try different USB port
- Press RESET button on ESP32

### "Upload Failed"

- Hold BOOT button while clicking Upload
- Try different USB cable
- Install CH340 drivers (Google "CH340 driver")

### No Braiins Data

- Wait 30 seconds after boot
- Check Serial Monitor (115200 baud) for errors
- Verify your workers are active on Braiins Pool

---

## 🎨 Customization (Optional)

### Change Colors

Edit `src/config.h`:

```cpp
#define COLOR_PRIMARY 0xFD20    // Orange → change to your color
#define COLOR_ACCENT 0x07FF     // Cyan → change to your color
```

### Change Screen Duration

```cpp
#define SCREEN_DURATION 5000    // 5 seconds → change to 3000 for 3 sec
```

### Change Your Braiins Worker

```cpp
#define BRAIINS_USERNAME "Silexperience"  // Your username
#define BRAIINS_WORKER "oct"              // Your worker name
```

---

## 🔥 You're Done!

Your cyberpunk mining dashboard is now live!

**Enjoy the show!** 🚀

---

Need more help? Check the full **README.md** for detailed documentation.
