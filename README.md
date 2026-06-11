# Smart E-Ink Air Quality Monitor
This repository contains all the 3D models, PCB production files, and ESPHome software configurations to build the Desktop Air Quality Monitor featured on [the channel](https://www.youtube.com/@FeatureNAB). 

[![Watch the video](https://img.youtube.com/vi/DqiMmY5ppnE/0.jpg)](https://www.youtube.com/watch?v=DqiMmY5ppnE)

The device uses an ESP32 and the Sensirion SEN66 to track 9 different air quality metrics (PM1, PM2.5, PM4, PM10, VOCs, NOx, CO2, Temperature, and Humidity) and displays them on an e-ink screen. It connects natively to [Home Assistant](https://www.home-assistant.io/) and the firmware is written using [ESPHome](https://esphome.io/).
  
## What's in this Repository?

**If you just want the final ready-to-go files, check the [Releases tab](../../releases) on the right!**

All source files are organized into their respective folders: 

*   📂 **`enclosure/`** - `.step` and `.3mf` models for the custom 3D printed housing.
*   📂 **`PCB/`** - Full KiCad project files and the `production` folder containing the Gerbers, BOM, and CPL files required for manufacturing.
*   📂 **`firmware/`** - ESPHome `.yaml` configuration files, plus precompiled bin files that you can flash directly to your board (Minimal and Home Assistant versions available).

## Getting Started

This project comes in 2 flavours - the minimal "just get the sensor working" variant, and the full variant that uses a custom PCB and a 3D printed housing.

### Minimal variant 

#### 1. Hardware
To build this version, you will need:

*   [Sensirion SEN66](https://mou.sr/44c63Ji)
*   [Seeed XIAO ESP32-C6](https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbEVLbTd0dGJnZFpQeENEZVg1UGV1QXZnaUF5UXxBQ3Jtc0tuU0hGTnIzSEkyQ1NUTmktUU1jV1ZqeVBKUG5LbHlEQ3ZrbjM0MkF5VjBqczRQbVNVd1lCc0IwV2NaUm5vb1FXYU1ZSzBDdEpOTm85MUpHb0VqLWpDdVdZSXR3NWdzQ0hybEtKcmFlbVRkcVdJc2p5RQ&q=https%3A%2F%2Fwww.seeedstudio.com%2FSeeed-StudioXIAO-ESP32C6-3PCS-p-5918.html%3Fsensecap_affiliate%3DdzMqXsY%26referring_service%3Dlink&v=DqiMmY5ppnE) microcontroller
*   [A JST-GH to Dupont cable](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/jst-compatible/jst-ghr-06v-s-to-dupont-female-compatible-cable-6p-15cm) to connect them

Assemble by connecting the 3V, GND, SDA and SCL pins as shown in the video.

#### 2. Firmware

**Method A: "As Fast As Possible" (No Coding Required)**
1. If you plan to use the device with Home Assistant, download `Firmware_minimal_with_Home_Assistant_bins.zip `, if you plan to use it standalone use `Firmware_minimal_air_sensor_bins.zip ` from the Releases tab on the right side of this page.
2. Plug your ESP32 (C3, C5, C6, or S3) into your computer via USB.
3. Using a Web Serial compatible browser (like Chrome or Edge), go to [web.esphome.io](https://web.esphome.io).
4. Click "Connect", select your board's COM port, and upload the `.bin` file that matches your specific ESP32 model.
5. Connect to the device's fallback Wi-Fi network to add it to your home network!

**Method B: Do this if you already have Home Assistant (or want to customise your setup)**
1. Open the `Software` folder and grab the relevant `.yaml` config file. Copy its contents.
2. Paste it into your ESPHome Builder dashboard.
3. Edit your Wi-Fi credentials, encryption keys, and anything else you want to try.
4. "Install" to compile and flash directly to your board.

---

### Full Variant
#### 1. Hardware
To build this version, you will need:

*   [Sensirion SEN66](https://mou.sr/44c63Ji)
*   [Seeed XIAO ESP32-C6](https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbEVLbTd0dGJnZFpQeENEZVg1UGV1QXZnaUF5UXxBQ3Jtc0tuU0hGTnIzSEkyQ1NUTmktUU1jV1ZqeVBKUG5LbHlEQ3ZrbjM0MkF5VjBqczRQbVNVd1lCc0IwV2NaUm5vb1FXYU1ZSzBDdEpOTm85MUpHb0VqLWpDdVdZSXR3NWdzQ0hybEtKcmFlbVRkcVdJc2p5RQ&q=https%3A%2F%2Fwww.seeedstudio.com%2FSeeed-StudioXIAO-ESP32C6-3PCS-p-5918.html%3Fsensecap_affiliate%3DdzMqXsY%26referring_service%3Dlink&v=DqiMmY5ppnE) microcontroller
*   [2.13" e-ink Monochrome Display](https://www.seeedstudio.com/2-13-Monochrome-ePaper-Display-with-122x250-Pixels-p-5778.html?sensecap_affiliate=dzMqXsY&referring_service=link) (Note: this MUST be monochrome, the current PCB design DOES NOT WORK with colour epaper)
*   Custom epaper driver board PCB (see below)
*   4x M3 size heat set inserts. I used Ruthex.
*   4x M3 bolts for the front + 1x M3 bolt to keep the PCB in place in the enclosure. CNC Kitchen sell these.
*   A cable to connect the SEN66 to the board
    *  I used a stemma cable, cut it in half, and soldered it to a JST-GH 1.25mm pre-crimped cable.
    *  Alternatively if you will use the dupont headers on the board you can buy premade cables [like these](https://www.amazon.ie/Connectors-Pre-Crimped-Compatible-Pixhawk4-Silicone/dp/B0CLD7S5VC) (note this cable won't attach to the stemma convenience connector on the PCB!)   

#### 2. PCB
I designed the PCB to be manufactured and assembled by [JLCPCB](https://jlcpcb.com/?from=FEATURE). The ordering process is shown in the video.
1. Head to the **[Releases tab](../../releases)** and download the `PCB_fabrication_files.zip `.
2. Upload the Gerber zip to JCLPCB.
3. Enable **PCBA** (PCB Assembly) to have the tiny SMD components soldered for you.
4. Upload the BOM and positions from the zip file.
5. Order and wait for the goods to arrive.

#### 3. 3D Printing the Enclosure
Download the `3D_Files.zip` from the Releases tab and print the enclosure. 
*   The `.3mf` file is pre-configured and ready to print.
*   The `.step` file is provided if you want to modify the housing in any way. 

#### 4. Firmware
Same method as the Method B: firmware steps above. The yaml, images, and extra code for the display and air algorithm is provided for you in the `firmware/The Everything Bagel` folder, but you'll need to compile it yourself in ESPHome Builder or the ESPHome CLI. 

## Home Assistant Dashboard

If you are connecting this to Home Assistant and want the UI dashboard card shown in the video, you will need to install HACS (Home Assistant Community Store) and download the **[Air Quality Card by KadenThomp36](https://github.com/KadenThomp36/air-quality-card)**. The thresholds and algorithms in my `aqi_algo.h` match the WHO 2021 recommendations also used by this card, so the recommendations shown on the physical display on this Air Quality device should match those on this card/dashboard.

## Support the project
[Patreon](https://www.patreon.com/FEATURE418) for anyone interested.
So I can keep funding crazy R&D hardware projects like this!

Some of the links on this page are affiliate links and help support these projects at no cost to you!
