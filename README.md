# 🪴 Automated Plant System

An ESP32-based smart gardening assistant featuring automated irrigation, real-time sensor monitoring, a local web dashboard, and an interactive OLED face expression system.

---

## 🚀 Features
* **Smart Irrigation:** Monitors soil moisture and toggles a water pump relay automatically.
* **Live Dashboard:** Hosts a local, responsive HTML dashboard that auto-refreshes every 2 seconds.
* **Interactive OLED Face:** Changes digital facial expressions (Neutral, Happy, Wink, Sad) via a touch sensor.
* **Safety Alert:** Uses an ultrasonic sensor to detect close objects and sound a buzzer alarm.
* **Object Detection:** Integrates an IR sensor to monitor immediate surroundings.

---

## 📌 Pin Mapping

| Component | Pin Type | ESP32 Pin |
| :--- | :--- | :--- |
| **DHT11 (Temp/Humid)** | Data | GPIO 13 |
| **Soil Moisture** | Analog | GPIO 34 |
| **Touch Sensor** | Digital | GPIO 26 |
| **IR Sensor** | Digital | GPIO 35 |
| **Ultrasonic Trigger** | Digital | GPIO 5 |
| **Ultrasonic Echo** | Digital | GPIO 18 |
| **Buzzer** | Digital | GPIO 4 |
| **Relay (Water Pump)** | Digital | GPIO 15 |
| **OLED (I2C)** | SDA/SCL | Standard I2C (`0x3C`) |



## 📐 Hardware & Enclosure Design

### PCB Schematic & Layout
The custom circuit architecture bridges the ESP32 controller with all monitoring modules:
* **Schematic Design:** Full EasyEDA wiring diagram routing power rails and logic lines.
* **PCB Layout:** Double-layer compact traces optimized to reduce cross-signal noise.
* <img width="996" height="597" alt="image" src="https://github.com/user-attachments/assets/e28968e3-a916-4f6f-ab46-01d165a7b05d" />
<img width="660" height="562" alt="image" src="https://github.com/user-attachments/assets/dbb55466-9b10-4d65-b02f-be4a3ed39a3c" />

<img width="366" height="270" alt="image" src="https://github.com/user-attachments/assets/db4ff1bf-3499-436a-aff1-439e41162d90" />

<img width="813" height="715" alt="image" src="https://github.com/user-attachments/assets/57169b9b-d15b-4ae1-9ad7-3733182aaa72" />

<img width="1107" height="792" alt="image" src="https://github.com/user-attachments/assets/f3168c96-ca26-45dc-9959-55d1fab18ae2" />


### 3D CAD Mechanical Enclosure
A modular structure handles water and electronics separation safely:
* **Top Module:** Integrated hopper container for plant or system alignment.
* **Bottom Module:** Enclosed basement unit safely housing the PCB assembly, pump, and fluid components.
  <img width="800" height="433" alt="image" src="https://github.com/user-attachments/assets/6aac0fc6-1b78-41cc-a7a2-32b8120a6254" />

---

