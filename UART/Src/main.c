/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

uint8_t RxBuffer[11]; // buffer for data received
uint8_t pBuffer = 0; // data counter
uint8_t data; // single data received
uint8_t sum = 0; // sum of the data in buffer

double angleY_DEC = 0.0; // Y angle in HEX
uint8_t angleY_ASCII[8] = {43,48,48,48,46,48,48,48}; // Y angle in ASCII; +xxx.xxx or -xxx.xxx

uint8_t enter[2] = {0x0D, 0x0A}; // carriage return

uint8_t freq_DEC = 0; // transmission frequency
uint8_t freq_ASCII[3] = {48,48,48}; 
uint8_t bit;

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
  MX_UART8_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart6, &data, 1); // open the reception interruption
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint32_t timer_DEC = 0;
	uint8_t bit = 0;
	
	timer_DEC = HAL_GetTick(); // start from board powered, maximum approximately 1193 hours
  while (1)
  {
		if(HAL_GetTick() - timer_DEC > 999){ // 1000 ms passed
			timer_DEC = HAL_GetTick(); // modify timer
			for(int i = 0; i < 3; ++i){
				bit = (uint8_t)(freq_DEC / pow(10, 2-i) + 48); // convert to ASCII value bitwise
		    freq_ASCII[i] = bit; // assignment
				freq_DEC -= (bit-48) * pow(10, 2-i);
			}
			// HAL_UART_Transmit(&huart8, freq_ASCII, 3, 0xFFFF); // print frequency
			// HAL_UART_Transmit(&huart8, enter, 2, 0xFFFF); // \r\n
			freq_DEC = 0; // clear frequency count
		}
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART6){
		if(data == 0x55){ // head of packet
			pBuffer = 0; // set 0x55 as head
			memset(RxBuffer, 0, 11*sizeof(uint8_t)); // clear the buffer (p.s. sizeof returns the bytes×Ö½Ú a value have, not bits)
			RxBuffer[pBuffer++] = data; // save 0x55 and move the pointer
			sum = data; // calculate sum of the buffer data by data
		}
		else{
			RxBuffer[pBuffer++] = data; // save the data and move the pointer
			if(pBuffer == 11){ // end of the buffer, also should be end of the packet
				if(sum == data && RxBuffer[1] == 0x53){ // packet is correct
					freq_DEC += 1;					
					angleY_DEC = ((int)((RxBuffer[5]<<8)|RxBuffer[4])/32768.0*180); // Y axis of angle; converted
					if(angleY_DEC > 180){
						angleY_DEC = 360 - angleY_DEC;
						angleY_ASCII[0] = (uint8_t)45;
					}
					else
						angleY_ASCII[0] = (uint8_t)43;
					uint8_t bit; // bitwise value
					for(int i = 0; i < 3; ++i){ // integral bits
						bit = (uint8_t)(angleY_DEC / pow(10, 2-i) + 48); // convert to ASCII value bitwise
						angleY_ASCII[i+1] = bit; // assignment
						angleY_DEC -= (bit-48) * pow(10, 2-i);
					}
					for(int i = 4; i < 7; ++i){ // decimal bits; same as above
						bit = (uint8_t)(angleY_DEC / pow(10, 3-i) + 48);
						angleY_ASCII[i+1] = bit;
						angleY_DEC -= (bit-48) * pow(10, 3-i);
					}
					HAL_UART_Transmit(&huart8, angleY_ASCII, 8, 0xFFFF); // transmit
					HAL_UART_Transmit(&huart8, enter, 2, 0xFFFF); // carriage return
					
					pBuffer = 0; // move the point to head of the buffer
					sum = 0; // clear sum of the buffer
				}
			}
			else // middle of the buffer
				sum += data; // calculate sum of the buffer data by data
		}
	}
	
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&data, 1); // re-open the reception interruption
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
