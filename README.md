# 🏠 Industrial ESP32-S3 Home Automation Engine

Welcome to my open-source home automation project! This repository contains production-ready, highly optimized firmware modules designed to build a reliable, local-first room automation system. 

The architecture focuses on **industrial reliability**, featuring robust input debouncing, hardware-level communication protocols (RS485/Modbus, CAN Bus, SPI), and low-latency network telemetry (MQTT).

---

## ⚠️ Disclaimer & Affiliation
* **Non-Affiliation:** This project is entirely independent. I am **not affiliated with, endorsed by, or partnered with Waveshare** in any official capacity. 
* **Sponsorship Status:** I am **not currently sponsored** by any hardware manufacturer. All code and designs contained here are part of my personal development journey as a student maker.

---

## 🛠️ Repository Architecture & Project Files

This codebase is split into modular components tailored for the ESP32-S3 platform. **Note for Arduino IDE Users:** Each `.ino` file is located within an identically named directory to comply with standard compilation toolchains.

### 🔌 Core Automation Modules
*   📁 **`Relay_Input_Controller/`**
    *   `Relay_Input_Controller.ino` — Features asynchronous debounce algorithms filtering isolated optocoupled digital input lines to toggle high-current relays cleanly.
*   📁 **`PLC_Climate_Sequencer/`**
    *   `PLC_Climate_Sequencer.ino` — Emulates an industrial programmable logic controller (PLC) finite-state machine to monitor climate variables and safely toggle exhaust fans or trigger safety alarms.
*   📁 **`PLC_Failsafe_Storage/`**
    *   `PLC_Failsafe_Storage.ino` — Utilizes Non-Volatile Storage (NVS) memory partitions to create a state backup recovery engine capable of surviving sudden room brownouts.

### 🌐 Industrial Protocols & Networking
*   📁 **`ESP32_MQTT_HomeAssistant/`**
    *   `ESP32_MQTT_HomeAssistant.ino` — Native dual-core asynchronous MQTT client that maps network commands from a Home Assistant broker directly onto hardware physical pins.
*   📁 **`Modbus_RS485_Master/`**
    *   `Modbus_RS485_Master.ino` — Low-level UART implementation of the industry-standard Modbus RTU protocol for reading heavy-duty environment sensors over a shielded twisted pair.
*   📁 **`ESP32_S3_CAN_Bus_Node/`**
    *   `ESP32_S3_CAN_Bus_Node.ino` — Harnesses the native TWAI/CAN bus peripheral drivers inside the ESP32-S3 to build a secure, automotive-grade multi-node mesh networking backbone.

### 📺 Waveshare Ecosystem Interfaces
*   📁 **`Waveshare_Modbus_Relay_Control/`**
    *   `Waveshare_Modbus_Relay_Control.ino` — Explicit hex array packet generator utilizing CRC-16 checksum math to control the dedicated **Waveshare 8-Channel Modbus RTU Relay Module**.
*   📁 **`Waveshare_RS485_Sensor_Hub/`**
    *   `Waveshare_RS485_Sensor_Hub.ino` — Direct integration sketch designed to poll telemetry variables from **Waveshare RS485 environmental transmitters**.
*   📁 **`Waveshare_EPD_Room_Display/`**
    *   `Waveshare_EPD_Room_Display.ino` — Low-refresh SPI canvas rendering program providing a status dashboard on a **Waveshare E-Ink/e-Paper display**.
*   📁 **`Waveshare_Round_LCD_Gauge/`**
    *   `Waveshare_Round_LCD_Gauge.ino` — Double-buffered sprite engine rendering smooth graphic gauges on the circular **Waveshare GC9A01 LCD screen**.

---

## ⚡ Hardware Recommendation

While this is an independent repository, **all code here works best with Waveshare hardware**. I highly recommend checking out their equipment for serious hobbyist or industrial projects:

*   **Built-in Chips:** Integrated communication transceivers minimize loose breadboard wires.
*   **I/O Protection:** Industrial optocoupled interfaces protect sensitive microcontrollers from power surges.
*   **Build Quality:** Reliable power delivery setups ensure long-term, stable operations.

🛒 Explore their complete catalog at the official **[Waveshare Store](https://www.waveshare.com/)**.

---

## 🚀 Getting Started

### 📋 Prerequisites
To build and flash these sketches to an ESP32-S3, make sure you have the following libraries installed via the **Arduino IDE Library Manager**:
*   `ModbusMaster` by Doc Walker
*   `PubSubClient` by Nick O'Leary
*   `GxEPD2` by Jean-Marc Zingg
*   `TFT_eSPI` by Bodmer *(Configuration required in `User_Setup.h` to select the GC9A01 driver panel)*

### 💻 Installation
1. Clone this repository down to your local directory setup:
   ```bash
   git clone https://github.com
   ```
2. Open your chosen module directory in the Arduino IDE or PlatformIO.
3. Select your ESP32-S3 development profile variant from the board dropdown list.
4. Verify compiling variables and flash the target code over native USB.

---

## 🤝 Contribution & License
This project is open-source. Feel free to open issues, submit pull requests, or modify the code layout for your own custom home automation setups!
