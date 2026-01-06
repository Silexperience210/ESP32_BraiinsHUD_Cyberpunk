ESP32 BraiinsHUD — Web Flasher

This folder contains the minimal files required to host a browser-based flasher using ESP Web Tools.

Files included:
- `index.html` — Web page with the ESP Web Tools install button.
- `manifest.json` — Firmware manifest used by ESP Web Tools (points to `merged-firmware.bin`).
- `merged-firmware.bin` — Placeholder firmware file. Replace with the real merged firmware binary.

How to prepare a real merged firmware (locally):
1. Build your project with PlatformIO (or Arduino/ESP-IDF) and collect the `bootloader.bin`, `partitions.bin`, and `firmware.bin` files.
2. Use `esptool` to merge them into a single binary for ESP32-C3 (example):

```bash
python -m esptool --chip esp32c3 merge-bin -o merged-firmware.bin \
  --flash-mode dio --flash-freq 40m --flash-size 4MB \
  0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin
```

3. Replace the `merged-firmware.bin` in the repo (commit & push) or upload it as a release and update `manifest.json` to point to the release URL.

Deploy on GitHub Pages:
1. Create a new GitHub repo and push this project.
2. In the repo Settings → Pages: select `main` branch and folder `/` (root).
3. Wait a few minutes; the site will be available at `https://<yourusername>.github.io/<repo>/`.

Updating firmware versions:
- Option A: Overwrite `merged-firmware.bin` in the repo and push — users will get the new firmware when they visit.
- Option B: Create releases and update `manifest.json` to point to the release asset URL (recommended for versioning).

Security notes:
- ESP Web Tools requires HTTPS and Web Serial permissions.
- Do NOT host untrusted firmware binaries.

