#ifndef INC_MCP_H_
#define INC_MCP_H_

#include "i2c.h"
#include "rtc.h"

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
#define rtcAddress 0x6F

HAL_StatusTypeDef getDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
HAL_StatusTypeDef setDateTime(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
HAL_StatusTypeDef getTime(RTC_TimeTypeDef *sTime);
HAL_StatusTypeDef getDate(RTC_DateTypeDef *sDate);
//HAL_StatusTypeDef startClock (void);
//HAL_StatusTypeDef stopClock (void);

void MX_MCP_Init(void);

#endif /* INC_MCP_H_ */

