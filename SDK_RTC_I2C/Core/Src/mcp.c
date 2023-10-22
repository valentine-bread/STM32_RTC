#include "mcp.h"
#include "i2c.h"

void MX_MCP_Init(void){

}

HAL_StatusTypeDef readMCP(uint8_t *rxData, uint16_t Size, uint16_t MemAddress){
	return HAL_I2C_Mem_Read(&hi2c1, rtcAddress | 1, MemAddress, I2C_MEMADD_SIZE_8BIT, rxData, Size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef writeMCP(uint8_t *rxData, uint16_t Size, uint16_t MemAddress){
	return HAL_I2C_Mem_Write(&hi2c1,rtcAddress & 0xFFFE, MemAddress, I2C_MEMADD_SIZE_8BIT, rxData, Size, HAL_MAX_DELAY);
}


uint8_t extractHours(uint8_t registerValue) {
    // Извлекаем значения битов 12/24, AM/PM, HRTEN0, HRTEN<1:0> и HRONE<3:0>
    uint8_t format12_24 = (registerValue >> 6) & 0x01;  // 12/24 (бит 6)
    uint8_t am_pm = (registerValue >> 5) & 0x01;       // AM/PM (бит 5)
    uint8_t tens = 0;
    uint8_t ones = 0;
    if (format12_24 == 1) { // 12-часовой формат
        tens = (registerValue >> 4) & 0x01;  // HRTEN0 (бит 4)
        ones = registerValue & 0x0F;          // HRONE<3:0> (биты 3-0)
    } else { // 24-часовой формат
        tens = (registerValue >> 4) & 0x03;  // HRTEN<1:0> (биты 5-4)
        ones = registerValue & 0x0F;          // HRONE<3:0> (биты 3-0)
    }

    // Преобразуем бинарно-десятичные значения в целое число
    uint8_t hours = tens * 10 + ones;

    if (format12_24 == 1 && am_pm == 1) { // Если 12-часовой формат и PM
        hours += 12;
    }

    return hours;
}


uint8_t extractMonth(uint8_t registerValue) {
    // Извлекаем значения битов LPYR, MTHTEN0 и MTHONE3-0
    uint8_t isLeapYear = (registerValue >> 5) & 0x01;  // LPYR (бит 5)
    uint8_t tens = (registerValue >> 4) & 0x01;       // MTHTEN0 (бит 4)
    uint8_t ones = registerValue & 0x0F;             // MTHONE3-0 (биты 3-0)
    // Преобразуем бинарно-десятичные значения в целое число
    uint8_t month = tens * 10 + ones;
    // Учитываем високосный год
    if (isLeapYear && month > 2) {
        month--; // В феврале у нас один день больше в високосный год
    }
    return month;
}


HAL_StatusTypeDef getDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate){
	uint8_t rxData[7];  // MCP79411 RTC возвращает 7 байтов данных.
	HAL_StatusTypeDef i2cStatus;
	i2cStatus = readMCP(rxData, 7, 0x00);
	if (i2cStatus == HAL_OK) {
	 // Вывод данных времени и даты в консоль.
	 sTime->Seconds = ((rxData[0] >> 4) & 0x07)* 10 + (rxData[0] & 0x0F);
	 sTime->Minutes = ((rxData[1] >> 4) & 0x07)* 10 + (rxData[1] & 0x0F);
	 sTime->Hours = extractHours(rxData[2]);
	 sDate->WeekDay = (rxData[3] & 0x07);
	 sDate->Date = ((rxData[4] >> 4) & 0x03) * 10 + (rxData[4] & 0x0F);
	 sDate->Month = extractMonth(rxData[5]);
	 sDate->Year = ((rxData[6] >> 4) & 0x0F) * 10 + (rxData[6] & 0x0F);
	}
	return i2cStatus;
}


uint8_t decToBcd(uint8_t val){
  return (val / 10) << 4 | (val % 10);
//	return (uint8_t)( (val/10*16) + (val%10) );
}

void formDateTimeData(uint8_t* data, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate) {
    // Формирование данных времени
    data[0] = decToBcd(sTime->Seconds);
    data[1] = decToBcd(sTime->Minutes);
    // Формирование данных часов
    data[2] = decToBcd(sTime->Hours);
    if (sTime->TimeFormat == RTC_HOURFORMAT_12) {
        data[2] |= (1 << 6); // Установка бита AM/PM в 1 для PM
    } else {
        data[2] &= ~(1 << 6); // Сброс бита AM/PM для AM
    }
    // Формирование данных дня недели
    data[3] = decToBcd(sDate->WeekDay);
    // Формирование данных даты
    data[4] =decToBcd(sDate->Date);
    // Формирование данных месяца
    data[5] = decToBcd(sDate->Month);
	if (sDate->Year % 4 == 0) {
		data[5] |= (1 << 5); // Установка бита LPYR для високосного года
	} else {
		data[5] &= ~(1 << 5); // Сброс бита LPYR для невисокосного года
	}
    // Формирование данных года
    data[6] = decToBcd(sDate->Year);
}

HAL_StatusTypeDef setSTBit() {
    uint8_t rxData[1];
    HAL_StatusTypeDef i2cStatus;
    i2cStatus = readMCP(rxData, 1, 0x00);    // Чтение текущего значения регистра секунд
    if (i2cStatus != HAL_OK) {
        return i2cStatus; // Обработка ошибки чтения
    }
    rxData[0] |= (1 << 7);    // Установка бита ST
    return writeMCP(rxData, 1, 0x00); // Запись обновленного значения обратно в устройство
}

HAL_StatusTypeDef clearSTBit() {
    uint8_t rxData[1];
    HAL_StatusTypeDef i2cStatus;
    i2cStatus = readMCP(rxData, 1, 0x00);
    if (i2cStatus != HAL_OK) {
        return i2cStatus; // Обработка ошибки чтения
    }
    rxData[0] &= ~(1 << 7);// Сброс бита ST
    return writeMCP(rxData, 1, 0x00);    // Запись обновленного значения обратно в устройство
}

uint8_t readOSCRUNBit() {
    uint8_t rxData[1];
    if (readMCP(rxData, 1, 0x03) != HAL_OK) {    // Чтение значения регистра RTCWKDAY
        return 0; // Возвращаем 0, если не удалось прочитать значение
    }
    uint8_t OSCRUNBit = (rxData[0] >> 5) & 0x01;
    return OSCRUNBit;
}


HAL_StatusTypeDef clearEXTOSCBit() {
	uint8_t rxData[1];
	HAL_StatusTypeDef i2cStatus;
	i2cStatus = readMCP(rxData, 1, 0x07);
	if (i2cStatus != HAL_OK) {
		return i2cStatus; // Обработка ошибки чтения
	}
	rxData[0] &= ~(1 << 3);// Сброс бита EXTOSC
	return writeMCP(rxData, 1, 0x07);    // Запись обновленного значения обратно в устройство
}


HAL_StatusTypeDef setDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate){
	HAL_StatusTypeDef i2cStatus = clearSTBit();
	if(i2cStatus != HAL_OK){
		return i2cStatus;
	}
	i2cStatus = clearEXTOSCBit();
	if(i2cStatus != HAL_OK){
		return i2cStatus;
	}
	for(int i = 0; i < 11 && readOSCRUNBit() != 0; i++){
		HAL_Delay(10);
		if(i == 10){
//			return HAL_ERROR;
		}
	}
	uint8_t rxData[7];
	formDateTimeData(rxData, sTime, sDate);
	i2cStatus = writeMCP(rxData, 7, 0x00);
	if(i2cStatus != HAL_OK){
		return i2cStatus;
	}
	HAL_Delay(10);
	return setSTBit();
}
