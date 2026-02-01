/**
 * @file ArduinoCCNET.cpp
 * @brief Реализация класса для вычисления 8-битной контрольной суммы
 * @author WASH-PRO
 * @email p.masyukov@gmail.com
 * 
 * Класс предоставляет функционал для инкрементального вычисления 
 * контрольной суммы как простой суммы байтов (с накоплением в uint8_t).
 */


#include "ArduinoCCNET.h"

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

byte RESET[] = {0x02, 0x03, 0x06, 0x30, 0x41, 0xB3};
byte POLL[]  = {0x02, 0x03, 0x06, 0x33, 0xDA, 0x81};
byte NAK[]   = {0x02, 0x03, 0x06, 0x00, 0xC2, 0x82};
byte START[] = {0x02, 0x03, 0x0C, 0x34, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x08, 0xEC};

#define CashCode_RX  2 
#define CashCode_TX  3 

namespace utils {

int CashCode_BaundRate = 9600;

SoftwareSerial CashCode(CashCode_RX, CashCode_TX);

CCNET::CCNET(int BaundRate) 
{
    CashCode_BaundRate = BaundRate;
}

bool CCNET::init(void) 
{   
    CashCode.begin(CashCode_BaundRate);

    uint8_t err = 0;

    while(reset() && poll() && err < 3)
    { 
        err++; 
        delay(100); 
    }

    delay(100);
    if(!err) return false; else return true;
}

bool CCNET::reset(void) 
{
    uint8_t err = 0;
    
    while(ready() && err < 3) 
    { 
        err++; 
        delay(100); 
    }

    if(!err) return false; else return true;
}

bool CCNET::start(void)
{
   uint8_t err = 0;
    
    while(err < 3)
    {
        send (START, sizeof(START));
        delay(100);
        
        long _timeout = millis() + 100;
        while (!CashCode.available() && millis() < _timeout)  {};

        if (CashCode.available()) 
        {    
            String str = "";            
            while(CashCode.available()) 
            {
                str += CashCode.read(), HEX; str += " ";
            }
    
            str.trim();

            if(str == "2 3 6 0 194 130") 
            {
                send (NAK, sizeof(NAK));
                err = 0; 

                break;
            }
        }

        err++;
    }
    
    if(!err) return false; else return true;    
}

unsigned int CCNET::poll(void)
{
    unsigned int res = 0;
    
    send (POLL, sizeof(POLL));

    long _timeout = millis() + 100;
    while (!CashCode.available() && millis() < _timeout)  {};

    if (CashCode.available()) 
    {
        String str = "";        
        while(CashCode.available()) { str += CashCode.read(), HEX; str += " "; }
        
        str.trim();
        //Serial.println(str);
        res = coupure(str);
    }

    return res;
}

unsigned int CCNET::coupure(String str)
{
    if(str == "2 3 7 129 3 207 24")  { send (NAK, sizeof(NAK)); return 50; }
    if(str == "2 3 7 129 4 112 108") { send (NAK, sizeof(NAK)); return 100; }
    if(str == "2 3 7 129 5 249 125") { send (NAK, sizeof(NAK)); return 500; }
    if(str == "2 3 7 129 6 98 79")   { send (NAK, sizeof(NAK)); return 1000; }
    if(str == "2 3 7 129 7 235 94")  { send (NAK, sizeof(NAK)); return 5000; }
    
    return 0;
}

void CCNET::send(byte *pData, int size)
{
    uint8_t *pData2 = (uint8_t*)pData;
    for (; size > 0; size--) CashCode.write(*pData2++);
}

bool CCNET::ready(void)
{    
    send (RESET, sizeof(RESET));

    long _timeout = millis() + 100;
    while (!CashCode.available() && millis() < _timeout)  {};

    if (CashCode.available())
    {
        String response = "";        
        while(CashCode.available()) { response += CashCode.read(), HEX; response += " "; }
            
        response.trim();
        if(response == "2 3 6 0 C2 82")
        { 
            delay(500); 
            return true; 
        }
    }

    return false;
}
}