/**
 * @file ArduinoCCNET.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the CCNET protocol class for Arduino.
 * @version 1.0.0
 * 
 * This class provides an interface to communicate with a CCNET-based 
 * cash acceptor device using SoftwareSerial.
 */


#include "ArduinoCCNET.h"

namespace utils {

// --- CCNET Protocol Commands ---
// These arrays represent the full command packets sent to the cash acceptor.
static const byte CMD_RESET[] = {0x02, 0x03, 0x06, 0x30, 0x41, 0xB3};
static const byte CMD_POLL[]  = {0x02, 0x03, 0x06, 0x33, 0xDA, 0x81};
static const byte CMD_ACK[]   = {0x02, 0x03, 0x06, 0x00, 0xC2, 0x82}; // Acknowledge command
static const byte CMD_START[] = {0x02, 0x03, 0x0C, 0x34, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x08, 0xEC};

/**
 * @brief Constructor: Initializes the CCNET object and creates a SoftwareSerial instance.
 */
CCNET::CCNET(uint8_t rxPin, uint8_t txPin, long baudRate)
    : _baudRate(baudRate)
{
    // Dynamically allocate SoftwareSerial to avoid issues with global constructors.
    _serial = new SoftwareSerial(rxPin, txPin);
}

/**
 * @brief Destructor: Cleans up by deleting the allocated SoftwareSerial instance.
 */
CCNET::~CCNET()
{
    // Free the memory to prevent a memory leak.
    delete _serial;
    _serial = nullptr;
}

/**
 * @brief Initializes the serial port and resets the cash acceptor.
 */
bool CCNET::init(void) 
{   
    // Start the serial communication at the specified baud rate.
    _serial->begin(_baudRate);

    // Attempt to reset the device, retrying on failure.
    for (unsigned int i = 0; i < MAX_RETRY; ++i) {
        if (reset()) {
            delay(200); // Give device time to initialize after reset
            return true; // Success
        }
        delay(100); // Wait before retrying
    }
    return false; // Failed to reset after retries
}

/**
 * @brief Sends a RESET command and waits for a valid ACK response.
 */
bool CCNET::reset(void) 
{
    // The 'ready()' helper function handles sending RESET and checking the response.
    for (unsigned int i = 0; i < MAX_RETRY; ++i) {
        if (ready()) {
            return true; // Success
        }
        delay(100); // Wait before retrying
    }
    return false; // Failed after retries
}

/**
 * @brief Sends a command to enable bill acceptance.
 */
bool CCNET::start(void)
{
    byte buffer[16];
    for (unsigned int i = 0; i < MAX_RETRY; ++i) {
        // Send the command to enable bill acceptance.
        send((byte*)CMD_START, sizeof(CMD_START));
        // Wait for a response.
        int len = receive(buffer, sizeof(buffer));

        // Check if the response is a valid ACK (command code 0x00).
        if (isResponseValid(buffer, len) && buffer[3] == 0x00) { // 0x00 is ACK
            // Acknowledge the device's response.
            send((byte*)CMD_ACK, sizeof(CMD_ACK));
            return true;
        }
        delay(100); // Wait before retrying
    }
    return false;
}

/**
 * @brief Polls the device for status and accepted bills.
 */
unsigned int CCNET::poll(void)
{
    byte buffer[16];
    // Send the POLL command.
    send((byte*)CMD_POLL, sizeof(CMD_POLL));

    // Read the response from the device.
    int len = receive(buffer, sizeof(buffer));
    // Validate the response (CRC check, etc.).
    if (isResponseValid(buffer, len)) {
        // Extract the bill type from the response.
        uint8_t billType = getBillType(buffer, len);
        if (billType > 0) {
            // If a bill was detected, send an ACK to confirm.
            send((byte*)CMD_ACK, sizeof(CMD_ACK));
            return billType;
        }
    }
    return 0; // No bill or unknown response
}

/**
 * @brief Parses a response buffer to find the bill type.
 */
uint8_t CCNET::getBillType(const byte* buffer, int len)
{
    // Example response for a bill: 02 03 07 81 03 CF 18
    // 0x81 indicates a bill-related event. The 5th byte (index 4) is the bill type.
    if (buffer[3] == 0x81) {
        return buffer[4]; // Return the bill type code (e.g., 3, 4, 5)
    }
    return 0; // Not a bill-related message
}

/**
 * @brief Low-level function to send a byte array over serial.
 */
void CCNET::send(byte *pData, int size)
{
    _serial->write(pData, size);
}

/**
 * @brief Low-level function to receive a byte array from serial with a timeout.
 */
int CCNET::receive(byte* buffer, int bufferSize, unsigned long timeout)
{
    int count = 0;
    unsigned long startTime = millis();
    // Loop until the timeout is reached.
    while (millis() - startTime < timeout) {
        if (_serial->available()) {
            if (count < bufferSize) {
                buffer[count++] = _serial->read();
            } else {
                // If the buffer is full, discard the rest of the incoming data.
                while(_serial->available()) _serial->read();
                break;
            }
        }
    }
    return count; // Return the number of bytes read.
}

/**
 * @brief Sends a RESET command and checks for a valid ACK response.
 */
bool CCNET::ready(void)
{    
    byte buffer[16];
    send((byte*)CMD_RESET, sizeof(CMD_RESET));

    // Use a longer timeout for reset, as the device may take time to restart.
    int len = receive(buffer, sizeof(buffer), 500); // Reset can take longer

    // Check if the response is a valid ACK.
    if (isResponseValid(buffer, len) && buffer[3] == 0x00) { // 0x00 is ACK
        delay(500); // Wait for device to be ready after ACK
        return true;
    }
    return false;
}

/**
 * @brief Compares a received buffer with an expected byte array.
 */
bool CCNET::compareResponses(const byte* buffer, int len, const byte* expected, int expectedLen) {
    if (len != expectedLen) {
        return false;
    }
    return memcmp(buffer, expected, len) == 0;
}

/**
 * @brief Validates an incoming message by checking its structure and CRC.
 */
bool CCNET::isResponseValid(const byte* buffer, int len) {
    // A valid message must have at least SYNC, ADDR, LEN, and 2 CRC bytes.
    if (len < 5) return false;

    // The first byte must be the SYNC byte (0x02).
    if (buffer[0] != 0x02) return false;

    // The third byte (index 2) contains the message length. It must match the received length.
    uint8_t messageLen = buffer[2];
    if (messageLen != len) return false;

    // Extract the 16-bit CRC from the last two bytes of the message.
    uint16_t receivedCRC = (uint16_t)buffer[len - 1] << 8 | buffer[len - 2];

    // Calculate the CRC on the message payload (from ADDR byte to data before CRC).
    uint16_t calculatedCRC = calculateCRC(buffer + 1, len - 3);

    // The received CRC must match the calculated one.
    return receivedCRC == calculatedCRC;
}

/**
 * @brief Calculates the 16-bit CRC for a given data buffer.
 */
uint16_t CCNET::calculateCRC(const byte* data, int len) {
    const uint16_t polynomial = 0x08408;
    uint16_t crc = 0;

    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

}