# Smart Soil Moisture Monitor with Automatic Irrigation

[![Arduino](https://img.shields.io/badge/Arduino-UNO%20%7C%20Nano%20%7C%20Mega-00979D?logo=arduino&logoColor=white)](https://arduino.cc)
[![Modbus](https://img.shields.io/badge/Modbus-RTU-485C3A)](https://modbus.org)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

An automated irrigation controller that reads soil moisture and temperature from an RS485 Modbus RTU sensor, controls a water pump relay, and displays real-time data on a 16×2 I2C LCD. Designed for greenhouse, garden, or agricultural automation.

## Features

- 🌱 **Real‑time soil moisture** (%) and temperature (°C) via RS485 Modbus sensor
- 💧 **Automatic pump control** – activates below 30%, turns off above 35% (hysteresis prevents rapid switching)
- 🖥️ **I2C LCD (16×2)** – shows moisture, temperature, and pump status
- ⏱️ **Sensor timeout handling** – displays error on LCD and serial
- 📟 **Serial debug output** – view readings and pump state over USB

## Hardware Requirements

| Component                     | Example / Notes                                  |
|-------------------------------|--------------------------------------------------|
| Arduino board                 | Uno, Nano, Mega (tested with Uno)               |
| RS485 soil sensor (Modbus RTU)| Supports function code 0x03, registers 0x00–0x01|
| RS485 to TTL converter        | MAX485 or similar module                        |
| 5V relay module               | Active Low (or modify code for Active High)     |
| 16×2 I2C LCD (address 0x27)   | Common address; use I2C scanner if different    |
| 12V/24V water pump            | External supply controlled by relay             |
| 5V power supply               | For Arduino and relay module                    |

## Wiring Diagram

| Arduino Pin | Connected To                  |
|-------------|--------------------------------|
| 2           | RS485 module RX (to sensor TX)|
| 3           | RS485 module TX (to sensor RX)|
| 7           | RS485 RE (receive enable)      |
| 8           | RS485 DE (driver enable)       |
| 9           | Relay IN (LOW = pump ON)       |
| A4 (SDA)    | LCD SDA                        |
| A5 (SCL)    | LCD SCL                        |
| 5V          | RS485 VCC, LCD VCC, Relay VCC  |
| GND         | Common ground                  |

> **Note:** The RS485 module’s VCC, RE, and DE are connected as shown. If your relay is active‑HIGH, change `LOW`/`HIGH` in the code.

## Installation & Setup

1. **Install required libraries** (Arduino IDE → Sketch → Include Library → Manage Libraries):
   - `LiquidCrystal I2C` by Frank de Brabander
   - `SoftwareSerial` (built‑in)

2. **Connect hardware** according to the wiring table.

3. **Set sensor Modbus parameters** in the code (if different from default):
   ```cpp
   const byte soilSensorRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
   // Slave ID=1, function=3, start register=0, 2 registers, CRC=C40B

4. Upload soil_monitor.ino to your Arduino.

5. Open Serial Monitor (9600 baud) to see live readings and pump status.
