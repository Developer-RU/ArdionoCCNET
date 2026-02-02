# ArduinoCCNET Library

<!-- [![Release](https://img.shields.io/github/v/release/Developer-RU/ArduinoCCNET)](https://github.com/Developer-RU/ArduinoCCNET/releases) -->
[![Version: 1.0.0](images/version.svg)](https://github.com/Developer-RU/ArdionoCCNET/releases/tag/1.0.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE.md)

A simple Arduino library for communicating with cash acceptors using the CCNET protocol. This library provides a straightforward interface to initialize, enable, and poll for accepted bills from CCNET-compatible devices.

## Features

*   Simple interface for core CCNET commands (`RESET`, `POLL`, `ENABLE BILLS`).
*   Uses `SoftwareSerial` to communicate, leaving the hardware serial port free for debugging.
*   Handles response validation with built-in 16-bit CRC checks.
*   Provides a non-blocking `poll()` method for easy integration into the main loop.
*   Lightweight and has no external dependencies besides the Arduino core libraries.

## Installation

### Using Arduino IDE

1.  Download the latest release from the Releases page as a `.zip` file.
2.  In the Arduino IDE, go to `Sketch` → `Include Library` → `Add .ZIP Library...`
3.  Select the downloaded `.zip` file.

### Using PlatformIO

Add the following line to your `platformio.ini` file under `lib_deps`:

```ini
[env:your_board]
platform = atmelavr ; or your platform
framework = arduino
lib_deps =
    https://github.com/Developer-RU/ArduinoCCNET.git
```

## Usage

Here is a basic example of how to use the library. For a complete sketch, see the `Simple.ino` example included with the library.

```cpp
#include <Arduino.h>
#include <ArduinoCCNET.h>

// Define the RX and TX pins for the cash acceptor.
#define CCNET_RX_PIN 2
#define CCNET_TX_PIN 3

// Create an instance of the CCNET class.
utils::CCNET cashAcceptor(CCNET_RX_PIN, CCNET_TX_PIN, 9600);

unsigned long lastPollTime = 0;
const unsigned long POLL_INTERVAL_MS = 200;

void setup() {
    Serial.begin(115200);
    Serial.println("CCNET Example Sketch");

    if (cashAcceptor.init()) {
        Serial.println("Cash acceptor initialized successfully.");
        if (cashAcceptor.start()) {
            Serial.println("Cash acceptor is now accepting bills.");
        } else {
            Serial.println("Failed to start cash acceptor!");
        }
    } else {
        Serial.println("Failed to initialize cash acceptor! Check wiring.");
    }
}

void loop() {
    // Poll the device periodically.
    if (millis() - lastPollTime > POLL_INTERVAL_MS) {
        lastPollTime = millis();

        unsigned int billType = cashAcceptor.poll();

        if (billType > 0) {
            Serial.print("Accepted a bill of type: ");
            Serial.println(billType);
        }
    }
}
```

## API Reference 

### Class: `utils::CCNET`

`CCNET(uint8_t rxPin, uint8_t txPin, long baudRate = 9600)` - Constructor.

`bool init()` - Initializes the serial connection and resets the device. Returns `true` on success.

`bool reset()` - Sends a `RESET` command to the device. Returns `true` if the device acknowledges.

`bool start()` - Sends a command to enable bill acceptance. Returns `true` on success.

`unsigned int poll()` - Polls the device for events. Returns the bill type number if a bill is accepted, otherwise returns `0`.

## Examples

The library includes a `Simple.ino` example sketch that can be accessed from the Arduino IDE via `File` → `Examples` → `ArduinoCCNET`.

## Platform Compatibility

This library is compatible with most Arduino boards that support `SoftwareSerial`, including:
*   Arduino Uno/Nano/Mega (AVR)
*   ESP8266 / ESP32 (though hardware serial is recommended on these platforms)

## Library Structure

```
ArduinoCCNET/
├── examples/
├── src/
│   ├── ArduinoCCNET.cpp   # Implementation
│   └── ArduinoCCNET.h     # Header file
├── library.properties
├── keywords.txt
├── LICENSE
└── README.md
```

## Contributing

Contributions are welcome! Please:

Fork the repository

Create a feature branch

Make your changes

Submit a pull request

Report issues at: https://github.com/Developer-RU/ArduinoCCNET/issues

## Changelog

### v1.0.0

*   Initial release.
*   Implementation of core CCNET commands: `RESET`, `POLL`, `ENABLE BILLS`.
*   Response validation with 16-bit CRC.
*   Added `Simple.ino` example.

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.
