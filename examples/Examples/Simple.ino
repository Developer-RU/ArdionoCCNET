/**
 * @file Simple.ino
 * @author Masyukov Pavel
 * @brief Example of using the ArduinoCCNET library.
 * @version 1.0.0
 * 
 * This sketch demonstrates how to initialize a CCNET cash acceptor
 * and poll it for incoming bills.
 */

#include <Arduino.h>
#include <ArduinoCCNET.h>

// Define the RX and TX pins for the cash acceptor.
#define CCNET_RX_PIN 2
#define CCNET_TX_PIN 3

// Create an instance of the CCNET class from the library.
// Pass the pins and baud rate to the constructor.
utils::CCNET cashAcceptor(CCNET_RX_PIN, CCNET_TX_PIN, 9600);

// Used to control the polling frequency.
unsigned long lastPollTime = 0;
const unsigned long POLL_INTERVAL_MS = 200;

void setup()
{
    // Initialize the hardware serial port for debugging output.
    Serial.begin(115200);
    // Wait for the serial port to connect. This is only necessary for boards
    // with native USB, like the Arduino Leonardo or Micro.
    while (!Serial) { ; }

    Serial.println("CCNET Example Sketch");
    Serial.println("Initializing Cash Acceptor...");
    // Initialize the cash acceptor. This will reset it and get it ready.
    if (cashAcceptor.init()) {
        Serial.println("Cash acceptor initialized successfully.");
        // Enable bill acceptance.
        if (cashAcceptor.start()) {
            Serial.println("Cash acceptor started and is accepting bills.");
        } else {
            Serial.println("Failed to start cash acceptor!");
        }
    } else {
        Serial.println("Failed to initialize cash acceptor! Check wiring and power.");
    }
}

void loop()
{
    // Poll the device periodically, but not too fast.
    if (millis() - lastPollTime > POLL_INTERVAL_MS) {
        lastPollTime = millis();

        unsigned int billType = cashAcceptor.poll();

        if (billType > 0) {
            Serial.print("Accepted a bill of type: ");
            Serial.println(billType);
        }
    }
}