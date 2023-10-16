/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
//#include "rtc.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trace.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
//  MX_RTC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* Do not remove this code below */
  MX_TRACE_Init();
  SDK_TRACE_Start();
  /* Do not remove this code from above */

  SDK_TRACE_Print("%s","Test string");
//  __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
//  HAL_RTC_WaitForSynchro(&hrtc);
  SDK_TRACE_Timestamp(PRINT, 0);
////  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
////  SDK_TRACE_Print("Current Date: %02d-%02d-%02d\n", sDate.Date, sDate.Month, sDate.Year);
//  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//  	  SDK_TRACE_Print("Current Date: %02d-%02d-%02d\n", sDate.Date, sDate.Month, sDate.Year);

  for (int i = 0; i < 5;i++) {
//	 SDK_TRACE_Print("%d", sTime.Seconds);
	 uint8_t rxData[7];  // MCP79411 RTC возвращает 7 байтов данных.
	 HAL_StatusTypeDef i2cStatus;

	 // Адрес MCP79411 RTC на шине I2C.
	 uint8_t rtcAddress = 0xDF; // MCP79411 RTC адрес в режиме чтения.

	 // Установка адреса чтения и начало чтения данных.
	 i2cStatus = HAL_I2C_Mem_Read(&hi2c1, rtcAddress, 0x00, I2C_MEMADD_SIZE_8BIT, rxData, sizeof(rxData), HAL_MAX_DELAY);

	 if (i2cStatus == HAL_OK) {
		 // Вывод данных времени и даты в консоль.
		 uint8_t seconds = (rxData[0] & 0x0F);
		 uint8_t minutes = (rxData[1] & 0x0F);
		 uint8_t hours = (rxData[2] & 0x0F);
		 uint8_t day = (rxData[3] & 0x07);
		 uint8_t date = (rxData[4] & 0x0F);
		 uint8_t month = (rxData[5] & 0x0F);
		 uint16_t year = (2000 + ((rxData[6] & 0x0F)));
		 SDK_TRACE_Print("Дата и время: %02d:%02d:%02d %02d/%02d/%04d\n", hours, minutes, seconds, date, month, year);
	 }
	 else{
		Error_Handler();
	 }
	 SDK_TRACE_Timestamp(PRINT, 0);
//	 SDK_TRACE_Print("Current Time: %02d:%02d:%02d.%03d\n", sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds);
	 HAL_Delay(1000);

  }
//  SDK_TRACE_Timestamp(PRINT, 0);
  /* Place your code before here */
  /* Do not remove this code below */
  SDK_TRACE_Stop();
  /* Do not remove this code from above */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	SDK_TRACE_Print("%s","ERROR");
	SDK_TRACE_Timestamp(PRINT, 0);
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
