/**
 * @file ArduinoCCNET.h
 * @author Masyukov Pavel
 * @brief Header file for the CCNET protocol implementation for Arduino.
 * @version 1.0.0
 * 
 * This class provides an interface to communicate with a CCNET-based
 * cash acceptor device using SoftwareSerial.
 */

#pragma once


#include "Arduino.h"
#include <SoftwareSerial.h>

namespace utils {

class CCNET
{
public:
    /**
     * @brief Construct a new CCNET object.
     * @param rxPin The Arduino pin connected to the cash acceptor's TX line.
     * @param txPin The Arduino pin connected to the cash acceptor's RX line.
     * @param baudRate The communication speed (baud rate). Default is 9600.
     */
    CCNET(uint8_t rxPin, uint8_t txPin, long baudRate = 9600);

    /**
     * @brief Destroy the CCNET object and free allocated resources.
     */
    ~CCNET();

    /**
     * @brief Initializes the cash acceptor by setting up the serial connection and resetting the device.
     * @return true if initialization is successful, false otherwise.
     */
    bool init(void);

    /**
     * @brief Sends a RESET command to the cash acceptor.
     * @return true if the device acknowledges the reset, false otherwise.
     */
    bool reset(void);

    /**
     * @brief Enables bill acceptance on the device.
     * @return true if the device acknowledges the command, false otherwise.
     */
    bool start(void);

    /**
     * @brief Polls the cash acceptor for events, such as an inserted bill.
     * @return The bill type number (e.g., 3, 4, 5) if a bill is accepted, or 0 for no event.
     */
    unsigned int poll(void);

private:
    // Private helper methods for internal library operation.
    bool ready(void);
    void send(byte *pData, int size);
    int receive(byte* buffer, int bufferSize, unsigned long timeout = 100);
    uint8_t getBillType(const byte* buffer, int len);
    bool compareResponses(const byte* buffer, int len, const byte* expected, int expectedLen);
    bool isResponseValid(const byte* buffer, int len);
    uint16_t calculateCRC(const byte* data, int len);

    // Member variables
    SoftwareSerial* _serial; // Pointer to the SoftwareSerial object for communication.
    long _baudRate;          // Stores the baud rate for the serial connection.

    // Maximum number of times to retry a command if it fails.
    static const unsigned int MAX_RETRY = 3;
};

}