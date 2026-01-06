ESP32 BraiinsHUD — Web Flasher

This repository contains the minimal files required to host a browser-based flasher using ESP Web Tools.

Files included (root):
- `index.html` — Web page with the ESP Web Tools install button.
- `manifest.json` — Firmware manifest. Contains both a merged-binary build and an alternative parts-based build.
- `merged-firmware.bin` — Merged firmware binary (can be replaced by a real binary or a release asset).
- `bootloader.bin`, `partitions.bin`, `firmware.bin` — Individual parts (optional). If present, manifest lists them with offsets.

How to prepare a real merged firmware (locally):
1. Build your project with PlatformIO (or Arduino/ESP-IDF) and collect the `bootloader.bin`, `partitions.bin`, and `firmware.bin` files.
2. Use `esptool` to merge them into a single binary for ESP32-C3 (example):

```bash
python -m esptool --chip esp32c3 merge-bin -o merged-firmware.bin \
  --flash-mode dio --flash-freq 40m --flash-size 4MB \
  0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin
```

3. Replace the `merged-firmware.bin` in the repo (commit & push) or upload it as a release and update `manifest.json` to point to the release asset URL.

Deploy on GitHub Pages:
1. Create a new GitHub repo and push this project.
2. In the repo Settings → Pages: select `main` branch and folder `/` (root).
3. Wait a few minutes; the site will be available at `https://<yourusername>.github.io/<repo>/`.

Updating firmware versions:
Options for publishing updates:

- Overwrite `merged-firmware.bin` in the repo and push — immediate, but no version history.
- Publish release assets (recommended): upload the separate binaries or the merged binary to a GitHub Release and update `manifest.json` to reference the release URL. This preserves version history and lets users pick releases.

Manifest notes:

- The provided `manifest.json` contains two builds for `ESP32-C3`:
  1) `merged-firmware.bin` (offset 0)
  2) individual parts (`bootloader.bin`, `partitions.bin`, `boot_app0.bin`, `firmware.bin`) with common offsets used by PlatformIO builds.

If your build uses different offsets, update the `offset` values in `manifest.json` accordingly.

Security notes:
- ESP Web Tools requires HTTPS and Web Serial permissions.
- Do NOT host untrusted firmware binaries.

