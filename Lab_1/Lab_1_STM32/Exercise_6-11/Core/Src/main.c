/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 3-Hand LED Clock (1 tick = 100ms)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void clearAllClocks(void);
void setNumberOnClock(int num);
void clearNumberOnClock(int num);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  clearAllClocks();

  int hour = 6, min = 15, sec = 0;

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // 1. Clear display
      clearAllClocks();

      // 2. Map 60-second and 60-minute values to 12 LED positions (0 to 11)
      int sec_pos = sec / 5;
      int min_pos = min / 5;
      int hour_pos = hour;

      // 3. Light up hands (overlapping hands naturally combine)
      setNumberOnClock(sec_pos);
      setNumberOnClock(min_pos);
      setNumberOnClock(hour_pos);

      // 4. Tick speed (100ms per second)
      HAL_Delay(100);

      // 5. Advance clock time
      sec++;
      if (sec >= 60)
      {
          sec = 0;
          min++;
          if (min >= 60)
          {
              min = 0;
              hour = hour % 12;
          }
      }
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
void clearAllClocks(void)
{
    // Turns OFF all pins from PA4 to PA15 (Active-LOW: SET = OFF)
    HAL_GPIO_WritePin(GPIOA, 0xFFF0, GPIO_PIN_SET);
}

void setNumberOnClock(int num)
{
    if (num >= 0 && num <= 11)
    {
        // 0 -> PA15, 1 -> PA4, 2 -> PA5 ... 11 -> PA14
        int pin = (num == 0) ? 15 : (3 + num);
        HAL_GPIO_WritePin(GPIOA, (1 << pin), GPIO_PIN_RESET); // ON (Active-LOW)
    }
}

void clearNumberOnClock(int num)
{
    if (num >= 0 && num <= 11)
    {
        // 0 -> PA15, 1 -> PA4, 2 -> PA5 ... 11 -> PA14
        int pin = (num == 0) ? 15 : (3 + num);
        HAL_GPIO_WritePin(GPIOA, (1 << pin), GPIO_PIN_SET); // OFF (Active-LOW)
    }
}
/* USER CODE END 4 */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                        |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                        |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
