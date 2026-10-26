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
