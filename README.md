# Checksum Library for Arduino

A lightweight 8-bit checksum library for Arduino, ESP8266, ESP32 and other AVR/ARM platforms.
Provides efficient data integrity verification through simple byte summation.

## Features

* 8-bit checksum calculation with overflow
* Incremental calculation mode
* Batch calculation mode
* Initial value: 0xFF (configurable in code)
* Support for both single bytes and byte arrays
* Static methods for one-time calculations
* Memory efficient (minimal footprint)
* No external dependencies

## Installation

### Using Arduino IDE

1. Download the library as ZIP
2. In Arduino IDE: Sketch → Include Library → Add .ZIP Library...
3. Select the downloaded ZIP file

### Using PlatformIO

Add to your `platformio.ini`:

[env:your_board]
platform = espressif32  ; or your platform
framework = arduino
lib_deps = 
    https://github.com/Developer-RU/ArduinoCRC.git

## Usage

```cpp
#include <Checksum.h>

void setup() {
    Serial.begin(115200);
    
    // Example 1: Incremental calculation
    utils::Checksum checksum;
    checksum.start();
    checksum.add(0x01);
    checksum.add(0x02);
    checksum.add(0x03);
    uint8_t result = checksum.get(); // 0xFF + 0x01 + 0x02 + 0x03 = 0x05
    
    Serial.print("Incremental checksum: 0x");
    Serial.println(result, HEX);
    
    // Example 2: Batch calculation
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t batchResult = utils::Checksum::calculate(data, sizeof(data));
    
    Serial.print("Batch checksum: 0x");
    Serial.println(batchResult, HEX);
    
    // Example 3: Verification
    uint8_t expected = 0x09;
    bool isValid = utils::Checksum::verify(data, sizeof(data), expected);
    
    Serial.print("Verification: ");
    Serial.println(isValid ? "PASS" : "FAIL");
}

void loop() {
    // Empty
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
