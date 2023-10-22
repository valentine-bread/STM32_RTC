#ifndef INC_MCP_H_
#define INC_MCP_H_

#include "trace.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')
#define rtcAddress 0xDF

HAL_StatusTypeDef getDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
HAL_StatusTypeDef setDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
HAL_StatusTypeDef readMCP(uint8_t *rxData, uint16_t Size, uint16_t MemAddress);
HAL_StatusTypeDef writeMCP(uint8_t *rxData, uint16_t Size, uint16_t MemAddress);
uint8_t extractHours(uint8_t registerValue);
uint8_t extractMonth(uint8_t registerValue);
uint8_t decToBcd(uint8_t val);
void formDateTimeData(uint8_t* data, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
HAL_StatusTypeDef setSTBit();
HAL_StatusTypeDef clearSTBit();
uint8_t readOSCRUNBit();
HAL_StatusTypeDef clearEXTOSCBit();



void MX_MCP_Init(void);

#endif /* INC_MCP_H_ */

