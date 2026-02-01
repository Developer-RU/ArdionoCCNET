/**
 * @file BasicExample.ino
 * @brief Пример использования библиотеки Checksum для Arduino
 * @author WASH-PRO
 * @email p.masyukov@gmail.com
 * 
 * Демонстрирует три способа работы с контрольной суммой:
 * 1. Инкрементальное вычисление (по одному байту)
 * 2. Пакетное вычисление (массив данных)
 * 3. Верификация данных
 */

#include <Checksum.h>

void setup()
{
    // Инициализация последовательного порта для вывода результатов
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=======================================");
    Serial.println("Checksum Library Basic Example");
    Serial.println("=======================================\n");

    // Пример 1: Инкрементальное вычисление
    Serial.println("Example 1: Incremental Calculation");
    Serial.println("-----------------------------------");
    
    utils::Checksum checksum;
    checksum.start(); // Сброс к начальному значению 0xFF
    
    // Последовательно добавляем байты
    checksum.add(0x01);
    checksum.add(0x02);
    checksum.add(0x03);
    
    // Получаем результат: 0xFF + 0x01 + 0x02 + 0x03 = 0x05
    uint8_t incrementalResult = checksum.get();
    
    Serial.print("Bytes: 0x01, 0x02, 0x03\n");
    Serial.print("Initial value: 0xFF\n");
    Serial.print("Incremental checksum: 0x");
    Serial.println(incrementalResult, HEX);
    
    // Демонстрация математики для ясности
    Serial.print("Calculation: 0xFF + 0x01 + 0x02 + 0x03 = 0x");
    Serial.println(incrementalResult, HEX);
    Serial.println();

    // Пример 2: Пакетное вычисление (статический метод)
    Serial.println("Example 2: Batch Calculation");
    Serial.println("----------------------------");
    
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint16_t dataLength = sizeof(data);
    
    uint8_t batchResult = utils::Checksum::calculate(data, dataLength);
    
    Serial.print("Data array: ");
    for (uint16_t i = 0; i < dataLength; i++) {
        Serial.print("0x");
        Serial.print(data[i], HEX);
        if (i < dataLength - 1) Serial.print(", ");
    }
    Serial.println();
    
    Serial.print("Data length: ");
    Serial.print(dataLength);
    Serial.println(" bytes");
    
    Serial.print("Batch checksum: 0x");
    Serial.println(batchResult, HEX);
    
    // Проверяем вычисление вручную
    Serial.print("Manual check: 0xFF + 0x01 + 0x02 + 0x03 + 0x04 + 0x05 = 0x");
    Serial.println(batchResult, HEX);
    Serial.println();

    // Пример 3: Верификация данных
    Serial.println("Example 3: Data Verification");
    Serial.println("----------------------------");
    
    // Правильная ожидаемая сумма для массива data
    // 0xFF + 0x01 + 0x02 + 0x03 + 0x04 + 0x05 = 0x0E
    uint8_t expectedChecksum = 0x0E;
    
    Serial.print("Expected checksum: 0x");
    Serial.println(expectedChecksum, HEX);
    
    // Проверяем правильность контрольной суммы
    bool isValid = utils::Checksum::verify(data, dataLength, expectedChecksum);
    
    if (isValid) {
        Serial.println("✓ Verification: PASS - Data integrity confirmed");
    } else {
        Serial.println("✗ Verification: FAIL - Data corruption detected");
    }
    
    // Демонстрация обнаружения ошибки
    Serial.println("\nDemonstrating error detection:");
    
    // Имитируем повреждение данных (изменяем один байт)
    uint8_t corruptedData[] = {0x01, 0x02, 0x03, 0x44, 0x05}; // 0x04 изменен на 0x44
    
    Serial.print("Corrupted data: ");
    for (uint16_t i = 0; i < dataLength; i++) {
        Serial.print("0x");
        Serial.print(corruptedData[i], HEX);
        if (i < dataLength - 1) Serial.print(", ");
    }
    Serial.println();
    
    bool isCorruptedValid = utils::Checksum::verify(corruptedData, dataLength, expectedChecksum);
    
    if (isCorruptedValid) {
        Serial.println("✓ Corrupted data verification: PASS (unexpected!)");
    } else {
        Serial.println("✗ Corrupted data verification: FAIL (as expected)");
    }
    
    Serial.println("\n=======================================");
    Serial.println("Example completed successfully!");
    Serial.println("=======================================");
}

void loop()
{
    // Этот пример выполняется один раз в setup()
    // Для интерактивных примеров можно добавить код в loop()
    delay(1000);
}