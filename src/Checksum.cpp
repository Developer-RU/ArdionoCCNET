/**
 * @file Checksum.cpp
 * @brief Реализация класса для вычисления 8-битной контрольной суммы
 * @author WASH-PRO
 * @email p.masyukov@gmail.com
 * 
 * Класс предоставляет функционал для инкрементального вычисления 
 * контрольной суммы как простой суммы байтов (с накоплением в uint8_t).
 */

#include "Checksum.h"

namespace utils {

/**
 * @brief Сбрасывает контрольную сумму до начального значения
 */
void Checksum::start() {
    m_checksum = INITIAL_VALUE;
}

/**
 * @brief Добавляет один байт к текущей контрольной сумме
 * @param byte Байт данных для добавления
 */
void Checksum::add(uint8_t byte) {
    m_checksum += byte;
}

/**
 * @brief Добавляет массив байтов к текущей контрольной сумме
 * @param data Указатель на массив данных
 * @param dataLen Количество байтов для добавления
 * 
 * @note Если data == nullptr, функция не выполняет никаких действий
 */
void Checksum::add(const uint8_t* data, uint16_t dataLen) {
    if (data == nullptr) {
        return;
    }
    
    for (uint16_t i = 0; i < dataLen; ++i) {
        add(data[i]);
    }
}

/**
 * @brief Возвращает текущее значение контрольной суммы
 * @return Вычисленная контрольная сумма
 */
uint8_t Checksum::get() const {
    return m_checksum;
}

/**
 * @brief Вычисляет контрольную сумму для заданного массива данных
 * @param data Указатель на массив данных
 * @param dataLen Количество байтов для обработки
 * @return Контрольная сумма для переданных данных
 * 
 * @note Удобный статический метод для разового вычисления
 */
uint8_t Checksum::calculate(const uint8_t* data, uint16_t dataLen) {
    Checksum checksum;
    checksum.start();
    checksum.add(data, dataLen);
    return checksum.get();
}

/**
 * @brief Проверяет корректность контрольной суммы для данных
 * @param data Указатель на массив данных
 * @param dataLen Количество байтов для обработки
 * @param expected Ожидаемое значение контрольной суммы
 * @return true если вычисленная контрольная сумма совпадает с ожидаемой
 */
bool Checksum::verify(const uint8_t* data, uint16_t dataLen, uint8_t expected) {
    return calculate(data, dataLen) == expected;
}

} // namespace utils
