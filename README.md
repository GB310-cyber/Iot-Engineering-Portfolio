# Iot-Engineering-Portfolio
ESP32-S3 Environmental Monitoring System
# ESP32-S3 Environmental Monitoring System

## Project Overview

This project is an environmental monitoring prototype developed using C++ and the ESP32-S3 microcontroller. It demonstrates how embedded systems can collect environmental data, display sensor readings, and identify conditions that may require attention.

The project is developed and tested using the Wokwi simulator.

## Hardware Components

- **ESP32-S3:** The main microcontroller responsible for processing sensor readings and controlling the display.
- **DHT22 Sensor:** Measures ambient temperature and relative humidity.
- **MQ-2 Sensor:** Provides analog readings associated with smoke and combustible gases.
- **2.8-Inch TFT Touchscreen:** Displays environmental readings, system status, and the monitoring interface.

## 1. ESP32 + MQ-2 GAS/SMOKE SENSOR

MQ-2 AO → ESP32 GPIO 4
MQ-2 DO → ESP32 GPIO 5
MQ-2 VCC → 5V (if required by the module)
MQ-2 GND → ESP32 GND

Note: Protect ESP32 inputs from voltages above 3.3V. Use a voltage divider or level shifter where necessary.

## 2. ESP32 + DHT22 TEMPERATURE/HUMIDITY SENSOR

DHT22 DATA → ESP32 GPIO 6
DHT22 VCC → ESP32 3V3
DHT22 GND → ESP32 GND

Add a 4.7kΩ–10kΩ pull-up resistor between DATA and 3V3 if the sensor module does not already have one.

## 3. ESP32 + ILI9341 TFT DISPLAY (SPI)

TFT CS → ESP32 GPIO 10
TFT DC → ESP32 GPIO 9
TFT MOSI → ESP32 GPIO 11
TFT SCK/CLK → ESP32 GPIO 12
TFT GND → ESP32 GND
TFT VCC → According to the display module's specifications
TFT RST → According to the display module's specifications
TFT MISO → Not used by the current code

## 4. ESP32 + FT6206 TOUCH CONTROLLER (I²C)

Touch SDA → ESP32 GPIO 15
Touch SCL → ESP32 GPIO 14
Touch VCC → 3V3 (if specified for the module)
Touch GND → ESP32 GND

## 5. WI-FI

Wi-Fi name → Wokwi-GUEST
Wi-Fi password → Leave blank
Time zone → UTC+1 (Nigeria) change to your current location 

## 6. IMPORTANT NOTES

1. Connect all component grounds together.
2. Check the exact ESP32 board model before using these GPIO assignments.
3. GPIO 6, 9, 10 and 11 are connected to internal flash on the classic ESP32-WROOM-32 and must not be used as ordinary GPIOs on that board.
4. The MQ-2 readings and alarm thresholds must be calibrated for real hardware.
5. This prototype is for demonstration and must not be relied on as a certified fire or gas safety system.
## Features

- Real-time temperature and humidity monitoring.
- Analog gas and smoke sensor readings.
- Graphical sensor monitoring interface.
- Configurable high-temperature warning logic.
- Threshold-based gas and smoke detection.
- Event timestamps where network time synchronization is implemented and available.

## Technologies Used

- C++
- ESP32-S3
- Arduino framework
- Wokwi simulation platform
- Sensor interfacing and GPIO programming
- TFT display integration

## How It Works

The ESP32-S3 receives data from the DHT22 and MQ-2 sensors. The software processes the readings and displays the available information on the TFT touchscreen. When configured warning conditions are met, the program can update the system status and record event information.

## How to Run

1. Open the project in Wokwi.
2. Check the circuit connections and required libraries.
3. Start the simulation.
4. Observe the sensor readings and display interface.
5. Adjust simulated sensor conditions to test the implemented warning logic.

## Skills Demonstrated

This project demonstrates embedded C++ programming, ESP32-S3 development, analog and digital sensor interfacing, touchscreen integration, environmental data processing, and simulation-based testing.

## Future Improvements

Planned improvements include physical hardware testing, sensor calibration, improved touchscreen interaction, fault detection, and secure IoT communication.

## Project Status

Simulation-based prototype. Physical hardware validation and sensor calibration remain future tasks.

**Safety Notice:** This educational prototype is not a certified fire alarm or gas safety system.

## Author

**[ ABDUL-AZIM TIJANI ]**

IoT Engineering | Embedded Systems | C++ Development
