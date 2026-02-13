🌾 CrowPanel 5.0 – Smart Agriculture Dashboard (LVGL)

A fully working LVGL 8.3.10 + LovyanGFX setup for:

CrowPanel ESP32 Display 5.0" V3 (800×480 RGB)
Running on ESP32-S3-WROOM-1-N4R8 (4MB Flash / 8MB PSRAM)

-------------------------------------------------------------------
📦 Hardware Overview
| Component | Value                           |
| --------- | ------------------------------- |
| Board     | CrowPanel ESP32 Display 5.0" V3 |
| MCU       | ESP32-S3-WROOM-1-N4R8           |
| Flash     | **4MB (32Mb)**                  |
| PSRAM     | 8MB                             |
| Display   | 800 × 480 RGB                   |
| Touch     | Capacitive (GT911)              |
| Power     | USB-C                           |


-------------------------------------------------------------------
🛠 Software Setup Guide (Arduino IDE)
✅ Arduino IDE
Recommended:
Arduino IDE 2.x (tested with 2.3.7)

-------------------------------------------------------------------
✅ Install ESP32 Board Package
Step 1 — Add Board Manager URL
Go to:
File → Preferences

Add to Additional Boards Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Click OK.

Step 2 — Install ESP32 Package
Go to:
Tools → Board → Boards Manager

Search:
esp32

Install:
esp32 by Espressif Systems
🎯 Recommended Core Version
Tested and confirmed working: 3.3.7

Alternative stable version: 2.0.14

⚠ Do NOT randomly mix versions.
LVGL + LovyanGFX are sensitive to core versions.

-----------------------------------------------------
📚 Required Libraries
Go to:
Sketch → Include Library → Manage Libraries

Install the following:
1️⃣ LovyanGFX
Tested Version: 1.2.19

2️⃣ LVGL
lvgl
Install: 8.3.10

⚠ IMPORTANT:
DO NOT use LVGL 9.x.

This project is built for LVGL 8.3.x only.

-------------------------------------------------------------------
⚙️ Board Configuration (CRITICAL)

Select:
Tools → Board → ESP32 Arduino → ESP32S3 Dev Module

🔧 Core Settings
USB CDC On Boot → Enabled
CPU Frequency → 240MHz (WiFi)
Core Debug Level → None

💾 Flash Settings (VERY IMPORTANT)
Flash Mode → QIO 80MHz
Flash Size → 4MB (32Mb)
Partition Scheme → Huge APP (3MB No OTA / 1MB SPIFFS)

If incorrect, you will get:
Detected size(4096k) smaller than the size in the binary image header(16384k)

🧠 PSRAM Settings (REQUIRED)
PSRAM → OPI PSRAM
⚠ REQUIRED for 800×480 LVGL projects.

Without PSRAM → crash / freeze / bootloop.

🔌 USB Settings
USB Mode → Hardware CDC and JTAG
Upload Mode → USB-OTG CDC (TinyUSB)
Upload Speed → 921600

🧹 First Upload Recommendation
Tools → Erase All Flash Before Sketch Upload → Enabled

After successful upload, you may disable it.

-------------------------------------------------------------------
📁 Project Folder Structure
Your Arduino sketch folder MUST look like this:

final-crowpanel/
│
├── final-crowpanel.ino
├── gfx_conf.h
└── lv_conf.h

❗ Important Rules (Based on Real Testing)
🔹 lv_conf.h

MUST be inside your sketch folder
NOT inside Arduino/libraries/lvgl/
Must match LVGL 8 configuration

🔹 gfx_conf.h

Must NOT contain setup()
Must NOT contain loop()
Must define RGB panel configuration
Must initialize GT911 correctly
🔹 Fonts

If you use fonts like:

lv_font_montserrat_36
lv_font_montserrat_22

Make sure they are enabled in:

lv_conf.h

🚀 Features in This Demo

✔ RGB 800×480 Display
✔ LVGL 8.3.10
✔ LovyanGFX RGB Driver
✔ GT911 Touch Support
✔ Circular Gauge Cards
✔ Live Updating Sensor Simulation
✔ Scrollable Layout
✔ Temperature History Bar Chart

🧠 Common Problems & Fixes
❌ UI Freezing / Crashing

Usually caused by:

Wrong PSRAM setting
LVGL 9 installed instead of 8.3
Wrong Flash Size
Wrong ESP32 core version
❌ "Touch_GT911.hpp not found"

Use:
#include <lgfx/v1/platforms/esp32/Touch_GT911.hpp>

NOT:
esp32s3/Touch_GT911.hpp
❌ Flash Size Error

Fix:

Flash Size → 4MB (32Mb)
Partition → Huge APP

-------------------------------------------------------------------

🧪 Tested Configuration
| Component   | Version   |
| ----------- | --------- |
| Arduino IDE | 2.3.7     |
| ESP32 Core  | 3.3.7     |
| LovyanGFX   | 1.2.19    |
| LVGL        | 8.3.10    |
| Flash       | 4MB       |
| PSRAM       | OPI PSRAM |

-------------------------------------------------------------------
🌱 Next Improvements

Real sensor integration (I2C / RS485 / Modbus)
Multi-page UI with swipe
WiFi dashboard sync
ESP-NOW remote nodes
Data logging to SD card
OTA firmware update

-------------------------------------------------------------------
📜 License
Open-source demo project for CrowPanel 5.0 + LVGL.


