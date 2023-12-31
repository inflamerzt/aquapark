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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "test.h"
#include "font8sans.h"
#include "font16.h"
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

dcount_t cnt1;
uint8_t cntupd = 0;
char str[] = "00.00";
char mixstr[] = "0000";

typedef struct type21{ uint8_t x; uint8_t y;}type21;

//typedef struct type24 { type21 x[3]; } type24;

volatile type21 test123[3] ={{1,2},{1,2},{1,2}};



const uint16_t st_text[] = {
		0x0000,
		0x33D2,
		0x4A14,

		0x4398,
		0x4A14,
		0x33D2,
		0x0000,
		0x0000,
};
//bool bufrw=true;
//bool wfdt=true;
uint8_t str_cnt=0;
uint8_t LP_lock = 0;
uint16_t LP_buffer[32]; //nthStr - 2*16 + (nth+8)Str - 2*16

uint16_t display_buffer[16][2];/*=
{
		{0x8000,0x0001},
		{0x0800,0x0010},
		{0x0080,0x0100},
		{0x0008,0x1000},

		{0xAA00,0x0055},
		{0x5500,0x00AA},
		{0xAA00,0x0055},
		{0x5500,0x00AA},

		{0x00AA,0x5500},
		{0x0055,0xAA00},
		{0x00AA,0x5500},
		{0x0055,0xAA00},

		{0x0008,0x1000},
		{0x0080,0x0100},
		{0x0800,0x0010},
		{0x8000,0x0001}
};*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM6_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void cpToLPBuffer(void);
void putStrToBuff(char *str);
void putStrToBuffMix(char *str);
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
  LL_DBGMCU_EnableDBGSleepMode();
  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_TIM3_STOP | LL_DBGMCU_APB1_GRP1_TIM4_STOP);
  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_TIM6_STOP);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
  MX_TIM6_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_5 | LL_EXTI_LINE_6 | LL_EXTI_LINE_7);
  LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);


  LL_TIM_EnableCounter(TIM3);
  LL_TIM_ClearFlag_UPDATE(TIM3);
  LL_TIM_EnableIT_UPDATE(TIM3);
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
  LL_TIM_OC_SetCompareCH4(TIM3, 29000);//170 - 30001);

  LL_TIM_EnableCounter(TIM6);

  LL_TIM_EnableIT_UPDATE(TIM6);

	cpToLPBuffer();
	/*
  //update LP buffer
  //nthStr - 2*16 + (nth+8)Str - 2*16
  for (uint8_t i=0;i<8;i++){
	  LP_buffer [4*i] = display_buffer[i+8][0];
	  LP_buffer [4*i+1] = display_buffer[i+8][1];
	  LP_buffer [4*i+2] = display_buffer[i][0];
	  LP_buffer [4*i+3] = display_buffer[i][1];
  };
*/

  //LL_DMA_ConfigTransfer(DMAx, Channel, Configuration)
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&LP_buffer[4],LL_SPI_DMA_GetRegAddr(SPI1) ,LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  //LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t *)LP_buffer);
  //LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 4);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
  LL_SPI_EnableDMAReq_TX(SPI1);
  //LL_SPI_EnableIT_TXE(SPI1);

  LL_SPI_Enable(SPI1);


  __NOP();
  //uint8_t pos = font_8ptChrsDescr[4]->position;
  //uint8_t siz = symb_8s_siz(4);

  volatile uint32_t test;
//  test = '0' - font_8pt_info.stchar;
  test = test123->x;
  __NOP();
//  test = font_8ptChrsDescr[4].position;
  test = font_8ptChrsDescr[4].size;
  test = font_8ptChrsDescr[4].position;
  test = font_8pt_info.stchar;
  test = font_8pt_info.endchar;
  test = font_8pt_info.sp_width;
  test = font_8pt_info.height;
  test = font_8ptBtmps[6];

/*
  uint8_t rpos = '1'-font_8pt_info.stchar;
  uint8_t bpos = font_8ptChrsDescr[rpos].position;
  uint8_t chw = font_8ptChrsDescr[rpos].size;
  uint8_t rpos1 = '0'-font_8pt_info.stchar;
  uint8_t bpos1 = font_8ptChrsDescr[rpos1].position;
*/
/*
#define bmpaddr(c)  font_8ptChrsDescr[c - font_8pt_info.stchar].position;

  bpos = bmpaddr('1');
*/


  test = strlen(str);

  uint8_t width;
   uint8_t tmp;
/*
  for (char* s=&str[0]; *s != '\0'; s++ )
  {
      //printf("%c\n", *s);
	  rpos = *s - font_8pt_info.stchar;
	  width = font_8ptChrsDescr[rpos].size;
	  bpos = font_8ptChrsDescr[rpos].position;
	  tmp = font_8ptBtmps[bpos];

  };
*/



   //strcpy(str,str_templ);

   putStrToBuff(&str[0]);
/*
   char *s;

  //str[4] = '0' + 5;


  uint8_t rpos, bpos, st_pos;

  for (uint8_t i=0;i<8;i++){
	  s=&str[0];
	  st_pos = 32;
  uint32_t tmp = 0;
  while (*s != '\0'){
	  rpos = *s - font_8pt_info.stchar;
	  width = font_8ptChrsDescr[rpos].size;
	  bpos = font_8ptChrsDescr[rpos].position;
	  st_pos -= width;
	  tmp |= font_8ptBtmps[bpos + i] << st_pos;
	  st_pos--;

	  s++;

  };

  display_buffer[i][0] = (tmp >> 16);
  display_buffer[i][1] = (tmp);

  };

  */
	/*	  for (uint8_t i=0;i<8;i++){
			  uint32_t temp;
			  st_pos = 32;
			  st_pos -= chw;
			  temp = ((font_8ptBtmps[bpos+i]) << (st_pos));

			  st_pos -= chw + 1;
			  temp |= (font_8ptBtmps[bpos1+i] << (st_pos));

			  st_pos -= chw + 1;
			  temp |= (font_8ptBtmps[bpos+i] << (st_pos));

			  st_pos -= chw + 1;
			  temp |= (font_8ptBtmps[bpos1+i] << (st_pos));

			  st_pos -= chw + 1;
			  temp |= (font_8ptBtmps[bpos+i] << (st_pos));

			  display_buffer[i][0] = (temp >> 16);
			  display_buffer[i][1] = (temp);
		  }
*/
		  __NOP();
		  uint32_t testop1 = 10;
		  uint32_t testop2 = 2;
		  uint32_t result;
		  asm("nop");
		  result = __ROR(testop1, testop2);
//  return (op1 >> op2) | (op1 << (32U - op2));
		  result = (testop1 >> testop2) | (testop1 << (32U -testop2));
		  result = __RBIT(result);
		  uint32_t value = result;
    __ASM volatile ("rbit %0, %1" : "=r" (result) : "r" (value) );

    __ASM volatile ("ror.w %0, %1" : "=r" (result) : "r" (testop2));

    	//display

  cpToLPBuffer();

  /*
   * fit in 16bit register
   */

  	  uint8_t positions;
  	  uint8_t chars;

  	  /*
  	   * cpos + chw < 16
  	   * yes:
  	   *  put char cpos += chw
  	   * no:
  	   * cpos = chw +  16 - cpos (partially put in fist bit)
  	   *  put part of sym in register
  	   * calculate size of part in next register
  	   *
  	   * go to yes:
  	   *
  	   * max width = 6 * 5 symols = 30 bits
  	   *
  	   *
  	   *
  	   * don't convert hex to dec, count in dec
  	   */




/*

  for (uint8_t str_cnt = 0;str_cnt < 8; str_cnt++){

	GPIOB->BSRR = (str_cnt << 12u) | ((0x7-str_cnt) << (12u+16u));

  }
  */




  	  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  __WFI();
	  __NOP();
	  __NOP();

	  if (cntupd) {
	  mixstr[0] = '0' + cnt1.c3;
	  mixstr[1] = '0' + cnt1.c2;
	  mixstr[2] = '0' + cnt1.c1;
	  mixstr[3] = '0' + cnt1.c0;

	  putStrToBuffMix(mixstr);
	  cpToLPBuffer();
	  cntupd = 0;
	  };


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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }
  LL_RCC_EnableRTC();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(72000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL_DIV_1_5);
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

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

    LL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_BKP);
  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.AsynchPrescaler = 0xFFFFFFFFU;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  LL_RTC_SetAsynchPrescaler(RTC, 0xFFFFFFFFU);
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_HALFWORD);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_16BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  /* TIM3 interrupt Init */
  NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM3_IRQn);

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 30000-LL_TIM_IC_FILTER_FDIV1_N2;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM3);
  LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 30001;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH4);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**TIM3 GPIO Configuration
  PB1   ------> TIM3_CH4
  */
  GPIO_InitStruct.Pin = LP_nOE_T3C4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(LP_nOE_T3C4_GPIO_Port, &GPIO_InitStruct);

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

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  /* TIM6 interrupt Init */
  NVIC_SetPriority(TIM6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));
  NVIC_EnableIRQ(TIM6_IRQn);

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  TIM_InitStruct.Prescaler = 20;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 36000;
  LL_TIM_Init(TIM6, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM6);
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM6);
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LP_LE_Pin|LP_MUX_A_Pin|LP_MUX_B_Pin|LP_MUX_BB14_Pin
                          |LED_Pin);

  /**/
  GPIO_InitStruct.Pin = LP_LE_Pin|LP_MUX_A_Pin|LP_MUX_B_Pin|LP_MUX_BB14_Pin
                          |LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE5);

  /**/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE6);

  /**/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE7);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_6;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_7;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

  /**/
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);

  /**/
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);

  /**/
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));
  NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void cpToLPBuffer(void){
  //update LP buffer
  //nthStr - 2*16 + (nth+8)Str - 2*16
  	for (uint8_t i=0;i<8;i++){
	LP_buffer [4*i] = display_buffer[i+8][0];
	LP_buffer [4*i+1] = display_buffer[i+8][1];
	LP_buffer [4*i+2] = display_buffer[i][0];
	LP_buffer [4*i+3] = display_buffer[i][1];
  };
};

void putStrToBuff(char *string){
		char *s ;
	  uint8_t rpos, bpos, st_pos, w;
	  for (uint8_t i=0;i<8;i++){
		  s=string;
		  st_pos = 32;
	  uint32_t tmp = 0;
	  while (*s != '\0'){
		  rpos = *s - font_8pt_info.stchar;
		  w = font_8ptChrsDescr[rpos].size;
		  bpos = font_8ptChrsDescr[rpos].position;
		  st_pos -= w;
		  tmp |= font_8ptBtmps[bpos + i] << st_pos;
		  st_pos--;

		  s++;

	  };

	  display_buffer[i][0] = (tmp >> 16);
	  display_buffer[i][1] = (tmp);

	  };


}

void putStrToBuffMix(char *string){
		char *s ;
		uint8_t count;
	  uint8_t rpos, bpos, st_pos, w;
	  for (uint8_t i=0;i<8;i++){
		  s=string;
		  st_pos = 32;
		  count = 0;
	  uint32_t tmp = 0;
	  uint32_t tmp1 = 0;
	  while (*s != '\0'){
		  if (count <2){
			  count++;

			  rpos = *s - font_16pt_info.stchar;
			  w = font_16ptChrsDescr[rpos].size;
			  bpos = font_16ptChrsDescr[rpos].position;
			  st_pos -= w;
			  tmp |= font_16ptBtmps[bpos + i] << st_pos;
			  tmp1 |= font_16ptBtmps[bpos+8 + i] << st_pos;
			  st_pos--;




		  }
		  else{
			  if (count == 2){st_pos -= 2;count++;};
		  rpos = *s - font_8pt_info.stchar;
		  w = font_8ptChrsDescr[rpos].size;
		  bpos = font_8ptChrsDescr[rpos].position;
		  st_pos -= w;
		  tmp |= font_8ptBtmps[bpos + i] << st_pos;
		  tmp1 |= st_text[i];
		  st_pos--;
		  }
		  s++;

	  };

	  display_buffer[i][0] = (tmp >> 16);
	  display_buffer[i][1] = (tmp);

	  display_buffer[i+8][0] = (tmp1 >> 16);
	  display_buffer[i+8][1] = (tmp1);

	  };


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
