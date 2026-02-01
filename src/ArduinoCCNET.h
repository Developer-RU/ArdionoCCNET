/**
 * @file Checksum.h
 * @brief Заголовочный файл класса для вычисления 8-битной контрольной суммы
 * @author WASH-PRO
 * @email p.masyukov@gmail.com
 * 
 * Класс предоставляет функционал для инкрементального вычисления 
 * контрольной суммы как простой суммы байтов (с накоплением в uint8_t).
 * 
 * @note Использует начальное значение 0xFF для контрольной суммы
 * @warning Из-за использования 8-битного переполнения контрольная сумма 
 *          не является криптографически стойкой и предназначена только 
 *          для проверки целостности данных
 */

/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once


#include "Arduino.h"
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>


namespace utils {

class CCNET
{
    public:
        CCNET::CCNET(int CashCode_BaundRate);

    public:
        int CashCode_BaundRate;

        bool init(void);
        bool reset(void);
        bool start(void);
        unsigned int poll(void);
        unsigned int coupure(String str); 
    private:
        void send(byte *pData, int size);
        bool ready(void);
};

}