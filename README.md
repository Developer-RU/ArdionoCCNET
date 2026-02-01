# ArduinoCCNET Library

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

Add to your `platformio.ini`:

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

### Class: utils::Checksum 

void start() - Reset checksum to initial value (0xFF)

void add(uint8_t byte) - Add single byte to checksum

void add(const uint8_t* data, uint16_t dataLen) - Add byte array to checksum

uint8_t get() const - Get current checksum value

### Static Methods

static uint8_t calculate(const uint8_t* data, uint16_t dataLen) - Calculate checksum for byte array

static bool verify(const uint8_t* data, uint16_t dataLen, uint8_t expected) - Verify checksum

## Limitations

8-bit checksum provides basic error detection only

Not cryptographically secure

Susceptible to certain error patterns

Maximum overflow: 255 bytes without loss of precision

## Examples

The library includes the following examples (File → Examples → ArduinoCRC):

BasicUsage - Simple checksum calculation and verification

Incremental - Demonstrates incremental checksum building

DataVerification - Complete data integrity check example

SerialMonitor - Interactive checksum calculator via Serial

## Platform Compatibility

✅ Arduino Uno/Nano/Mega (AVR)
✅ ESP8266
✅ ESP32
✅ Arduino Due (SAM)
✅ Teensy 3.x/4.x
✅ Raspberry Pi Pico (RP2040)

## Library Structure

ArduinoCRC/
├── examples/           # Example sketches
├── src/
│   ├── Checksum.cpp   # Implementation
│   └── Checksum.h     # Header file
├── library.properties # Library metadata
├── keywords.txt       # Arduino IDE syntax highlighting
├── LICENSE           # MIT License
└── README.adoc       # This file

## Contributing

Contributions are welcome! Please:

Fork the repository

Create a feature branch

Make your changes

Submit a pull request

Report issues at: https://github.com/Developer-RU/ArduinoCRC/issues

## Changelog ==

### v2.0.0 (2026-01-03)

Initial release

8-bit checksum implementation

Incremental and batch modes

MIT license

## License

MIT License

Copyright (c) 2022-2026 WASH-PRO p.masyukov@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Links

Repository: https://github.com/Developer-RU/ArduinoCRC

Issues: https://github.com/Developer-RU/ArduinoCRC/issues

Releases: https://github.com/Developer-RU/ArduinoCRC/releases
