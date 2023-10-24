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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define GreenLedPin			GPIO_PIN_7
#define WhiteLedPin			GPIO_PIN_5
#define BlueLedPin			GPIO_PIN_4


#define 	Zero		(0x98)
#define 	One			(0xA2)
#define 	Two			(0x62)
#define 	Three		(0xE2)
#define 	Four		(0x22)
#define 	Five		(0x02)
#define 	Six			(0xC2)
#define 	Seven		(0xE0)
#define 	Eight		(0xA8)
#define 	Nine		(0x90)
#define 	UP			(0x18)
#define 	Down		(0x4A)
#define 	Right		(0x5A)
#define 	Left		(0x10)
#define 	OK			(0x38)
#define 	Astrecs		(0x68)
#define 	Window		(0xb0)


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t tempCode;
uint8_t bitIndex;
uint8_t cmd;
uint8_t cmdli;
uint8_t code;

uint8_t GreenLedCMD = Two;
uint8_t WhiteLedCMD = Five;
uint8_t BlueLedCMD = Eight;

uint8_t buttonPressed = 0;
uint8_t LedsCounter = 3;

void UpdateLeds();
void ReProgrammingFunction();


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_11)
  {
    if (__HAL_TIM_GET_COUNTER(&htim1) > 8000)
    {
      tempCode = 0;
      bitIndex = 0;
    }
    else if (__HAL_TIM_GET_COUNTER(&htim1) > 1700)
    {
      tempCode |= (1UL << (31-bitIndex));   // write 1
      bitIndex++;
    }
    else if (__HAL_TIM_GET_COUNTER(&htim1) > 1000)
    {
      tempCode &= ~(1UL << (31-bitIndex));  // write 0
      bitIndex++;
    }
    if(bitIndex == 32)
    {
      cmdli = ~tempCode; // Logical inverted last 8 bits
      cmd = tempCode >> 8; // Second last 8 bits
      if(cmdli == cmd) // Check for errors
      {
        code = tempCode; // If no bit errors

        if(buttonPressed) ReProgrammingFunction();
        else UpdateLeds();

        char snum[10];
        itoa(code, snum, 10);
      }
      bitIndex = 0;
    }
    __HAL_TIM_SET_COUNTER(&htim1, 0);
  }
}

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
  MX_TIM1_Init();

  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start(&htim1);
	__HAL_TIM_SET_COUNTER(&htim1, 0);


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void UpdateLeds()
{
	if(GreenLedCMD == cmd) HAL_GPIO_TogglePin(GPIOB, GreenLedPin);
	else if(WhiteLedCMD == cmd) HAL_GPIO_TogglePin(GPIOB, WhiteLedPin);
	else if(BlueLedCMD == cmd) HAL_GPIO_TogglePin(GPIOB, BlueLedPin);
	else if(OK == cmd) buttonPressed = 1;
}

void ReProgrammingFunction()
{
	if(LedsCounter == 3)
	{
		GreenLedCMD = cmd;
		LedsCounter--;
	}
	else if(LedsCounter == 2)
	{
		WhiteLedCMD = cmd;
		LedsCounter--;
	}
	else if(LedsCounter == 1)
	{
		BlueLedCMD = cmd;
		LedsCounter = 3;
		buttonPressed = 0;
	}
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
