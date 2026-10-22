# ESP32 Web Server

A collection of ESP32 web server examples using Arduino IDE and the `WiFi.h` and `WebServer.h` libraries.

## Features
- Wi-Fi Station (STA) mode
- Access Point (AP) mode
- Web-based control of two LEDs via GPIO
- HTTP Basic Authentication

## Getting Started
1. Open the desired `.ino` file in Arduino IDE.
2. Configure your Wi-Fi credentials if required.
3. Upload the code to your ESP32.
4. Open the Serial Monitor at `115200` baud to find the device's IP address.
5. Enter the IP address in a browser to access the web interface.

## Hardware
- ESP32 development board
- Two LEDs and current-limiting resistors (optional, connected to GPIO 26 and GPIO 27)

**Note:** HTTP Basic Authentication over plain HTTP does not encrypt credentials. Use these examples for learning and testing on trusted networks.
