/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_THERMISTOR (0)
#define DATA_POTENTIOMETER (1)
#define DATA_LIGHT_ALL (2)
#define DATA_LIGHT_IR (3)
#define DATA_TEMPERATURE (4)
#define DATA_HUMIDITY (5)

#define MODE_STANDBY (0)
#define MODE_RECORD_DATA (1)
#define MODE_READ_DATA (2)
#define MODE_LIVE_DATA (3)
#define MODE_SET_SAMPLE_TIME (4)

#define UTF_N (0x4E)
#define UTF_Y (0x59)
#define UTF_SPACE (0x20)
#define UTF_RETURN (0xD)

//*LIGHT SENSOR */
//Slave addresses
#define LIGHT_ADDR (0x29 << 1)

//Control
#define LIGHT_CONTR (0x80)
#define LIGHT_CONTR_ACTIVE (0x1)

#define LIGHT_MEAS_RATE (0x85)
#define LIGHT_MEAS_RATE_100ms_50ms (0x00)

#define LIGHT_INTERRUPT (0x8F)
#define LIGHT_INTERRUPT_INTERRUPT_ON_MEASURE (0b0001010)

#define LIGHT_STATUS (0x8C)
#define LIGHT_STATUS_NEW_DATA (0x04)

//Data
#define LIGHT_DATA_1_L (0x88)
#define LIGHT_DATA_1_H (0x89)
#define LIGHT_DATA_0_L (0x8A)
#define LIGHT_DATA_0_H (0x8B)
//*LIGHT SENSOR END */

//*TEMPERATURE HUMIDITY SENSOR*/
//Slave address
#define TH_ADDR (0x70 << 1)

//Control
#define TH_SLEEP (0xB098)
#define TH_WAKE (0x3517)
#define TH_READ_DATA (0x7866)

//Data
#define TH_DATA_TEMP_H (0)
#define TH_DATA_TEMP_L (1)
#define TH_DATA_HUM_H (3)
#define TH_DATA_HUM_L (4)

//*TEMPERATURE HUMIDITY SENSOR END*/

//*EEPROM*/
#define EEPROM_ADDR (0b1010000 << 1)
#define EEPROM_ENTRY_SIZE (18) //bytes

#define PAGE_YEAR (0)
#define PAGE_MONTH (1)
#define PAGE_DATE (2)
#define PAGE_HOUR (3)
#define PAGE_MINUTE (4)
#define PAGE_SECOND (5)
#define PAGE_THERMISTOR_H  (6)
#define PAGE_THERMISTOR_L  (7)
#define PAGE_POTENTIOMETER_H (8)
#define PAGE_POTENTIOMETER_L (9)
#define PAGE_LIGHT_ALL_H (10)
#define PAGE_LIGHT_ALL_L (11)
#define PAGE_LIGHT_IR_H (12)
#define PAGE_LIGHT_IR_L (13)
#define PAGE_TEMPERATURE_H (14)
#define PAGE_TEMPERATURE_L (15)
#define PAGE_HUMIDITY_H (16)
#define PAGE_HUMIDITY_L (17)
//#define PAGE_ ()
//*EEPROM END*/

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint16_t data[6];
uint16_t eeprom_page_addr = 0x00;
uint8_t plg_dtcted = 0, mode = 0, second_elapsed = 0, sample_time = 0;

#define SAMPLE_TIMES_DIVISIONS (455)
const uint8_t SAMPLE_TIMES[] = {1, 2, 5, 10, 15, 30, 45, 60, 120};
const uint8_t UART_SAMPLE_TIME_SET[] = "Sample time set to ";
const uint8_t UART_SAMPLE_TIMES[][3] = {"  1", "  2", "  5", " 10", " 15", " 30", " 45", " 60", "120"};
const uint8_t UART_SECONDS[] = " s\r\n";

const uint8_t UART_MODE_START[][25] = {"Standby mode...\r\n", "Recording data mode...\r\r\n", "Reading data mode...\r\n", "Live data mode...\r\n", "Setting sample time...\r\n"};

const uint8_t UART_START_PACKET[] = "<Packet Start>\r\n";
const uint8_t UART_END_PACKET[] = "<Packet End>\r\r\n";
const uint8_t UART_SENSOR_PREFIX[][20] = {"Thermistor:\t", "Potentiometer:\t", "Light (Visible):\t", "Light (Infrared):\t", "Temperature:\t", "Humidity:\t\t"};

const uint8_t UART_NO_DATA[] = "There is no data stored to transmit\r\r\n";
const uint8_t UART_LOGON[] = "Device Logged On\r\r\n";
const uint8_t UART_TRANSMISSION_START[] = "Data transmission from EEPROM:\r\n";
const uint8_t UART_TABLE_HEADER[] = "CSV format: DD.MM.YY,HH.MM.SS,Thermistor,Potentiometer,Light(Visible),Light(Infrared),Temperature,Humidity\r\n";
const uint8_t UART_TABLE_LINE[] = "---------------------------------------------------\r\n";
const uint8_t UART_TRANSMISSION_END[] = "Data transmission complete\r\r\n";
const uint8_t UART_DATA_DELETE[] = "Would you like to erase the EEPROM? (Y/N)\r\n";

const uint8_t UART_DATA_ERASED[] = "\r\nData has been erased\r\r\n";
const uint8_t UART_DATA_NOT_ERASED[] = "\r\nData has not been erased\r\r\n";
const uint8_t UART_NO_REPLY_DETECTED[] = "\r\nNo reply detected, please reply with 'Y' or 'N'\r\n";
const uint8_t UART_INVALID_REPLY_DETECTED[] = "\r\nInvalid reply detected, please reply with 'Y' or 'N'\r\n";

const uint8_t UART_NEWLINE[] = "\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void changeMode(int MODE_CODE);
void setSampleTime(void);
void readData(void);
void storeData(void);
void retrieveData(void);
void liveData(void);
void transmitFromIndex(int index, int newline);
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
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
//  HAL_Delay(10000);

  //setup light sensor
  uint8_t buf[1];
  HAL_Delay(200);
  HAL_I2C_IsDeviceReady(&hi2c1, LIGHT_ADDR, 2, 1000);
  buf[0] = LIGHT_MEAS_RATE_100ms_50ms;
  HAL_I2C_Mem_Write(&hi2c1, LIGHT_ADDR, LIGHT_MEAS_RATE, 1, buf, 1, 1000);
  HAL_Delay(10);

  //get EEPROM address to write to
  HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 2, 1000);
  HAL_Delay(10);
  HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, 0x0000, 2, buf, 1, 1000);
  eeprom_page_addr = buf[0];
  HAL_Delay(10);

  HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t SW2_down = 0x00, SW3_down = 0x00, SW4_down = 0x00;
  while (1)
  {
	  //handle plug detect
	  if ((GPIOB->IDR & GPIO_IDR_14) == GPIO_IDR_14) {
		  if (!plg_dtcted) {
			  HAL_Delay(5000);
			  HAL_UART_Transmit(&huart1, UART_LOGON, sizeof(UART_LOGON), 10);
			  plg_dtcted = 1;
		  }
	  } else if (plg_dtcted) plg_dtcted = 0;

	  //select mode
	  if ((GPIOB->IDR & GPIO_IDR_3) == 0b0) {
		  //SW2 pressed
		  if (!SW2_down) {
			  SW2_down = 0b1;
			  if (mode == MODE_SET_SAMPLE_TIME) {
				  changeMode(MODE_RECORD_DATA);
			  } else {
				  sample_time = 0;
				  changeMode(MODE_SET_SAMPLE_TIME);
			  }
		  }
	  }	else SW2_down = 0b0;

	  if ((GPIOA->IDR & GPIO_IDR_15) == 0b0){
		  //SW3 pressed
		  if (!SW3_down) {
			  SW3_down = 0b1;
			  changeMode(MODE_READ_DATA);
		  }
	  }	else SW3_down = 0b0;

	  if ((GPIOB->IDR & GPIO_IDR_15) == 0b0){
		  //SW4 pressed
		  if (!SW4_down) {
			  SW4_down = 0b1;
		  changeMode(MODE_LIVE_DATA);
		  }
	  } else SW4_down = 0b0;


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = ENABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0010020A;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 124;
  hrtc.Init.SynchPrediv = 1999;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x16;
  sTime.Minutes = 0x35;
  sTime.Seconds = 0x35;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 0x17;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 8000-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_9B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB11 PB12 PB13
                           PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB15 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PF6 PF7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void changeMode(int MODE_CODE) {
	mode = MODE_CODE;
	HAL_UART_Transmit(&huart1, UART_MODE_START[MODE_CODE], sizeof(UART_MODE_START[MODE_CODE]), 10);

	if (MODE_CODE == MODE_SET_SAMPLE_TIME) MODE_CODE = MODE_LIVE_DATA;
	GPIOB->ODR &= ~(0b11 << 12); //turn off LEDs
	GPIOB->ODR |= (mode << 12);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback and toggle LED
  if (htim == &htim6)
  {
	  second_elapsed++;

	  switch (mode) {
	  case MODE_RECORD_DATA:
		  if (second_elapsed < sample_time) break;
		  readData();
		  storeData();
		  second_elapsed = 0;
		  break;
	  case MODE_STANDBY:
		  if (second_elapsed < 1) break;
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
		  second_elapsed = 0;
		  break;
	  case MODE_SET_SAMPLE_TIME:
		  readData();
		  setSampleTime();
		  break;
	  case MODE_READ_DATA:
		  retrieveData();
		  changeMode(MODE_STANDBY);
		  break;
	  case MODE_LIVE_DATA:
		  if (second_elapsed < 2) break;
		  readData();
		  liveData();
		  second_elapsed = 0;
		  break;
	  }
  }
}


void readData(void) {
	//check calibration, if not calibrate
	ADC1->CR |= ADC_CR_ADCAL;
	while ((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL);

	//check if ADC is RDY
	if ((ADC1->ISR & ADC_ISR_ADRDY) == 0b0) {
		ADC1->CR |= ADC_CR_ADEN;
		while ((ADC1->ISR & ADC_ISR_ADRDY) == 0b0);
	}

	//read thermistor data 12-bit res
	ADC1->CR |= ADC_CR_ADSTART;
	while ((ADC1->ISR & ADC_ISR_EOC) == 0b0);
	data[DATA_THERMISTOR] = ADC1->DR;

	//read potentiometer data 12-bit res
	ADC1->CR |= ADC_CR_ADSTART;
	while ((ADC1->ISR & ADC_ISR_EOC) == 0b0);
	data[DATA_POTENTIOMETER] = ADC1->DR;
	ADC1->ISR &= ~(ADC_ISR_EOS);

	//stop ADC
	ADC1->CR |= ADC_CR_ADSTP;
	while ((ADC1->CR & ADC_CR_ADSTP) == ADC_CR_ADSTP);
	//disable ADC
	ADC1->CR |= ADC_CR_ADDIS;
	while ((ADC1->CR & ADC_CR_ADDIS) == ADC_CR_ADDIS);

	//I2C data retrieval
	//LIGHT SENSOR
	uint8_t buf[6];
	HAL_StatusTypeDef ref;

	ref = HAL_I2C_IsDeviceReady(&hi2c1, LIGHT_ADDR, 2, 1000);

	buf[0] = LIGHT_CONTR_ACTIVE;
	ref = HAL_I2C_Mem_Write(&hi2c1, LIGHT_ADDR, LIGHT_CONTR, 1, buf, 1, 1000);

	//wait until new_data is high
	ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_STATUS, 1, buf, 1, 1000);
	while ((buf[0] & LIGHT_STATUS_NEW_DATA) == 0b0) {
		ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_STATUS, 1, buf, 1, 1000);
	}

	//read data
	ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_DATA_1_L, 1, buf, 1, 1000);
	data[DATA_LIGHT_IR] = buf[0];
	ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_DATA_1_H, 1, buf, 1, 1000);
	data[DATA_LIGHT_IR] |= (buf[0] << 8);
	ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_DATA_0_L, 1, buf, 1, 1000);
	data[DATA_LIGHT_ALL] = buf[0];
	ref = HAL_I2C_Mem_Read(&hi2c1, LIGHT_ADDR, LIGHT_DATA_0_H, 1, buf, 1, 1000);
	data[DATA_LIGHT_ALL] |= (buf[0] << 8);

	buf[0] = 0x00;
	ref = HAL_I2C_Mem_Write(&hi2c1, LIGHT_ADDR, LIGHT_CONTR, 1, buf, 1, 1000);

	//TEMPERATURE HUMIDITY
	ref = HAL_I2C_IsDeviceReady(&hi2c1, TH_ADDR, 2, 1000);

	buf[0] = (TH_WAKE >> 8);
	buf[1] = (TH_WAKE);
	ref = HAL_I2C_Master_Transmit(&hi2c1, TH_ADDR, buf, 2, 1000);

	buf[0] = (TH_READ_DATA >> 8);
	buf[1] = (TH_READ_DATA);
	ref = HAL_I2C_Master_Transmit(&hi2c1, TH_ADDR, buf, 2, 1000);

	//read data
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00;
	while (HAL_I2C_Master_Receive(&hi2c1, TH_ADDR, buf, 6, 1000) != HAL_OK);
	data[DATA_TEMPERATURE] = ((buf[TH_DATA_TEMP_H] << 8) | buf[TH_DATA_TEMP_L]);
	data[DATA_HUMIDITY] = ((buf[TH_DATA_HUM_H] << 8) | buf[TH_DATA_HUM_L]);


	buf[0] = (TH_SLEEP >> 8);
	buf[1] = (TH_SLEEP);
	ref = HAL_I2C_Master_Transmit(&hi2c1, TH_ADDR, buf, 2, 1000);

}

void setSampleTime(void) {
	uint8_t sample_time_index = data[DATA_POTENTIOMETER] / SAMPLE_TIMES_DIVISIONS;
	if (sample_time != SAMPLE_TIMES[sample_time_index]) {
		sample_time = SAMPLE_TIMES[sample_time_index];

		HAL_UART_Transmit(&huart1, UART_SAMPLE_TIME_SET, sizeof(UART_SAMPLE_TIME_SET), 10);
		HAL_UART_Transmit(&huart1, UART_SAMPLE_TIMES[sample_time_index], sizeof(UART_SAMPLE_TIMES[sample_time_index]), 10);
		HAL_UART_Transmit(&huart1, UART_SECONDS, sizeof(UART_SECONDS), 10);
	}
}

void storeData(void) {
	uint8_t page[EEPROM_ENTRY_SIZE];

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	//prepare year.month.date hour.minute.second
	page[PAGE_YEAR] = sDate.Year; page[PAGE_MONTH] = sDate.Month; page[PAGE_DATE] = sDate.Date;
	page[PAGE_HOUR] = sTime.Hours; page[PAGE_MINUTE] = sTime.Minutes; page[PAGE_SECOND] = sTime.Seconds;

	page[PAGE_THERMISTOR_H] = (data[DATA_THERMISTOR] >> 8);
	page[PAGE_THERMISTOR_L] = (data[DATA_THERMISTOR]);

	page[PAGE_POTENTIOMETER_H] = (data[DATA_POTENTIOMETER] >> 8);
	page[PAGE_POTENTIOMETER_L] = (data[DATA_POTENTIOMETER]);

	page[PAGE_LIGHT_ALL_H] = (data[DATA_LIGHT_ALL] >> 8);
	page[PAGE_LIGHT_ALL_L] = (data[DATA_LIGHT_ALL]);

	page[PAGE_LIGHT_IR_H] = (data[DATA_LIGHT_IR] >> 8);
	page[PAGE_LIGHT_IR_L] = (data[DATA_LIGHT_IR]);

	page[PAGE_TEMPERATURE_H] = (data[DATA_TEMPERATURE] >> 8);
	page[PAGE_TEMPERATURE_L] = (data[DATA_TEMPERATURE]);

	page[PAGE_HUMIDITY_H] = (data[DATA_HUMIDITY] >> 8);
	page[PAGE_HUMIDITY_L] = (data[DATA_HUMIDITY]);

	//write data
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, (eeprom_page_addr << 6), 2, page, EEPROM_ENTRY_SIZE, 1000);
	eeprom_page_addr++;
	//write page address until acknowledged
	page[0] = eeprom_page_addr;
	while (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, 0x0000, 2, page, 1, 1000) != HAL_OK);
}

void retrieveData(void) {
	if (eeprom_page_addr == 0x01) {
		HAL_UART_Transmit(&huart1, UART_NO_DATA, sizeof(UART_NO_DATA), 10);
		return;
	}

	HAL_UART_Transmit(&huart1, UART_TRANSMISSION_START, sizeof(UART_TRANSMISSION_START), 10);
	HAL_UART_Transmit(&huart1, UART_TABLE_HEADER, sizeof(UART_TABLE_HEADER), 10);
	HAL_UART_Transmit(&huart1, UART_TABLE_LINE, sizeof(UART_TABLE_LINE), 10);

	uint8_t page[EEPROM_ENTRY_SIZE];
	uint8_t date[9]; uint8_t time[9];

	for (uint16_t addr = 0x01; addr < eeprom_page_addr; addr++) {
		HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, (addr << 6), 2, page, EEPROM_ENTRY_SIZE, 1000);

		sprintf(date,"%02d.%02d.%02d", page[PAGE_DATE], page[PAGE_MONTH], page[PAGE_YEAR]);
		sprintf(time,",%02d.%02d.%02d", page[PAGE_HOUR], page[PAGE_MINUTE], page[PAGE_SECOND]);

		HAL_UART_Transmit(&huart1, (uint8_t *)date, sizeof(date), 1000);
		HAL_UART_Transmit(&huart1, (uint8_t *)time, sizeof(time), 1000);

		data[DATA_THERMISTOR] = ((page[PAGE_THERMISTOR_H] << 8) | page[PAGE_THERMISTOR_L]);
		data[DATA_POTENTIOMETER] = ((page[PAGE_POTENTIOMETER_H] << 8) | page[PAGE_POTENTIOMETER_L]);
		data[DATA_LIGHT_ALL] = ((page[PAGE_LIGHT_ALL_H] << 8) | page[PAGE_LIGHT_ALL_L]);
		data[DATA_LIGHT_IR] = ((page[PAGE_LIGHT_IR_H] << 8) | page[PAGE_LIGHT_IR_L]);
		data[DATA_TEMPERATURE] = ((page[PAGE_TEMPERATURE_H] << 8) | page[PAGE_TEMPERATURE_L]);
		data[DATA_HUMIDITY] = ((page[PAGE_HUMIDITY_H] << 8) | page[PAGE_HUMIDITY_L]);

		transmitFromIndex(DATA_THERMISTOR, 0);
		transmitFromIndex(DATA_POTENTIOMETER, 0);
		transmitFromIndex(DATA_LIGHT_ALL, 0);
		transmitFromIndex(DATA_LIGHT_IR, 0);
		transmitFromIndex(DATA_TEMPERATURE, 0);
		transmitFromIndex(DATA_HUMIDITY, 0);

		HAL_UART_Transmit(&huart1, UART_NEWLINE, sizeof(UART_NEWLINE), 1000);
	}

	HAL_UART_Transmit(&huart1, UART_TABLE_LINE, sizeof(UART_TABLE_LINE), 10);
	HAL_UART_Transmit(&huart1, UART_TRANSMISSION_END, sizeof(UART_TRANSMISSION_END), 10);
	HAL_UART_Transmit(&huart1, UART_DATA_DELETE, sizeof(UART_DATA_DELETE), 10);

	uint8_t buf[1];
	while (1) {
		buf[0] = UTF_SPACE;
		HAL_UART_Receive(&huart1, buf, 1, 20000);
		if (buf[0] == UTF_Y) {
			//'erase' memory (start at 0x00 again)
			buf[0] = 0x01; eeprom_page_addr = 0x01;
			HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, 0x0000, 2, buf, 1, 1000);
			HAL_UART_Transmit(&huart1, UART_DATA_ERASED, sizeof(UART_DATA_ERASED), 10);
			return;
		}
		else if (buf[0] == UTF_N) {
			HAL_UART_Transmit(&huart1, UART_DATA_NOT_ERASED, sizeof(UART_DATA_NOT_ERASED), 10);
			return;
		}
		else if (buf[0] == UTF_SPACE) {
			HAL_UART_Transmit(&huart1, UART_NO_REPLY_DETECTED, sizeof(UART_NO_REPLY_DETECTED), 10);
		}
		else HAL_UART_Transmit(&huart1, UART_INVALID_REPLY_DETECTED, sizeof(UART_INVALID_REPLY_DETECTED), 10);
	}
}

void liveData(void) {
	HAL_UART_Transmit(&huart1, UART_START_PACKET, sizeof(UART_START_PACKET), 10);

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	uint8_t date[16]; uint8_t time[16];

	sprintf(date," Date: %02d.%02d.%02d\t",sDate.Date,sDate.Month,sDate.Year);
	sprintf(time,"Time: %02d.%02d.%02d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds);

	HAL_UART_Transmit(&huart1, (uint8_t *)date, sizeof(date), 10);
	HAL_UART_Transmit(&huart1, (uint8_t *)time, sizeof(time), 10);

	transmitFromIndex(DATA_THERMISTOR, 1);
	transmitFromIndex(DATA_POTENTIOMETER, 1);
	transmitFromIndex(DATA_LIGHT_ALL, 1);
	transmitFromIndex(DATA_LIGHT_IR, 1);
	transmitFromIndex(DATA_TEMPERATURE, 1);
	transmitFromIndex(DATA_HUMIDITY, 1);

	HAL_UART_Transmit(&huart1, UART_END_PACKET, sizeof(UART_END_PACKET), 10);
}

void transmitFromIndex(int index, int newline) {
	uint16_t data_cpy = data[index];
	uint8_t UART_data[6] = "      ", data_len;
	if (newline)  {
		HAL_UART_Transmit(&huart1, UART_SENSOR_PREFIX[index], sizeof(UART_SENSOR_PREFIX[index]), 10);
		sprintf(UART_data, "%01d\r\n", data_cpy);
	}
	else sprintf(UART_data, ",%01d", data_cpy);

	//get exact length of data then transmit
	for (data_len = 0; data_len < 6; data_len++) if (UART_data[data_len] == '\0') break;
	HAL_UART_Transmit(&huart1, (uint8_t *)UART_data, data_len + 1, 10);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
