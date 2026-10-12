/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

int delay = 50;

uint8_t display = 0;
uint8_t led_count = 0;

/* Segment ports */
GPIO_TypeDef *segPorts[7] =
{
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB
};

/* Segment pins */
uint16_t segPins[7] =
{
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

void display7SEG(GPIO_TypeDef *ports[7],
                 uint16_t pins[7],
                 uint8_t num);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief Display a number on the 7-segment LED.
  *
  * Segment order:
  * a b c d e f g
  *
  * 0 = segment ON
  * 1 = segment OFF
  */
void display7SEG(GPIO_TypeDef *ports[7],
                 uint16_t pins[7],
                 uint8_t num)
{
    uint8_t seg[10][7] =
    {
        {0,0,0,0,0,0,1}, // 0
        {1,0,0,1,1,1,1}, // 1
        {0,0,1,0,0,1,0}, // 2
        {0,0,0,0,1,1,0}, // 3
        {1,0,0,1,1,0,0}, // 4
        {0,1,0,0,1,0,0}, // 5
        {0,1,0,0,0,0,0}, // 6
        {0,0,0,1,1,1,1}, // 7
        {0,0,0,0,0,0,0}, // 8
        {0,0,0,0,1,0,0}  // 9
    };

    if (num > 9)
    {
        return;
    }

    for (int i = 0; i < 7; i++)
    {
        HAL_GPIO_WritePin(
            ports[i],
            pins[i],
            seg[num][i]
        );
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

    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();

    /* USER CODE BEGIN 2 */

    /*
     * EN pins are ACTIVE-LOW:
     *
     * EN0 = PA6 -> display 1
     * EN1 = PA7 -> display 2
     * EN2 = PA8 -> display 3
     * EN3 = PA9 -> display 0
     *
     * LOW  = ON
     * HIGH = OFF
     */

    /* Turn all displays OFF */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9,
        GPIO_PIN_SET
    );

    /* Put number 1 on the segment bus */
    display7SEG(segPorts, segPins, 1);

    /* Turn EN0 ON -> display 1 */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_6,
        GPIO_PIN_RESET
    );

    /* Middle LEDs initially OFF */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4,
        GPIO_PIN_RESET
    );

    /* Start TIM2 interrupt */
    HAL_TIM_Base_Start_IT(&htim2);

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
      */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */

    htim2.Instance = TIM2;

    /*
     * HSI = 8 MHz
     *
     * 8 MHz / (7999 + 1) = 1000 Hz
     * 1000 Hz / (9 + 1) = 100 Hz
     *
     * Therefore:
     * 1 interrupt = 10 ms
     */
    htim2.Init.Prescaler = 7999;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 9;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(
            &htim2,
            &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(
            &htim2,
            &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIOA output levels */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9,
        GPIO_PIN_RESET
    );

    /* Configure GPIOB output levels */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6,
        GPIO_PIN_RESET
    );

    /* Configure GPIO pins:
       PA4 PA5 PA6 PA7 PA8 PA9 */
    GPIO_InitStruct.Pin =
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure GPIO pins:
       PB0 PB1 PB2 PB3 PB4 PB5 PB6 */
    GPIO_InitStruct.Pin =
        GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/**
  * @brief Timer interrupt callback
  *
  * Each interrupt = 10 ms.
  * 50 interrupts = 500 ms.
  *
  * EN0 -> 1
  * EN1 -> 2
  * EN2 -> 3
  * EN3 -> 0
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        delay--;

        if (delay <= 0)
        {
            delay = 50;

            /*
             * ---------------------------------------------------------
             * 1. Turn OFF the currently active display.
             *
             * EN pins are ACTIVE-LOW:
             * LOW  = ON
             * HIGH = OFF
             * ---------------------------------------------------------
             */

            if (display == 0)
            {
                // EN0 -> PA6
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_6,
                    GPIO_PIN_SET
                );
            }
            else if (display == 1)
            {
                // EN1 -> PA7
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_7,
                    GPIO_PIN_SET
                );
            }
            else if (display == 2)
            {
                // EN2 -> PA8
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_8,
                    GPIO_PIN_SET
                );
            }
            else
            {
                // EN3 -> PA9
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_9,
                    GPIO_PIN_SET
                );
            }


            /*
             * ---------------------------------------------------------
             * 2. Move to the next display.
             * ---------------------------------------------------------
             */

            display++;

            if (display > 3)
            {
                display = 0;
            }


            /*
             * ---------------------------------------------------------
             * 3. Change segment data and turn ON the next display.
             *
             * EN0 -> 1
             * EN1 -> 2
             * EN2 -> 3
             * EN3 -> 0
             * ---------------------------------------------------------
             */

            if (display == 0)
            {
                // EN0 -> display number 1

                display7SEG(
                    segPorts,
                    segPins,
                    1
                );

                // ACTIVE-LOW -> LOW means ON
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_6,
                    GPIO_PIN_RESET
                );
            }
            else if (display == 1)
            {
                // EN1 -> display number 2

                display7SEG(
                    segPorts,
                    segPins,
                    2
                );

                // ACTIVE-LOW -> LOW means ON
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_7,
                    GPIO_PIN_RESET
                );
            }
            else if (display == 2)
            {
                // EN2 -> display number 3

                display7SEG(
                    segPorts,
                    segPins,
                    3
                );

                // ACTIVE-LOW -> LOW means ON
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_8,
                    GPIO_PIN_RESET
                );
            }
            else
            {
                // EN3 -> display number 0

                display7SEG(
                    segPorts,
                    segPins,
                    0
                );

                // ACTIVE-LOW -> LOW means ON
                HAL_GPIO_WritePin(
                    GPIOA,
                    GPIO_PIN_9,
                    GPIO_PIN_RESET
                );
            }


            /*
             * ---------------------------------------------------------
             * 4. Blink the two middle LEDs every 1 second.
             *
             * One display change = 500 ms.
             * Two display changes = 1000 ms = 1 second.
             * ---------------------------------------------------------
             */

            led_count++;

            if (led_count >= 2)
            {
                led_count = 0;

                HAL_GPIO_TogglePin(
                    GPIOA,
                    GPIO_PIN_4
                );
            }
        }
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

    __disable_irq();

    while (1)
    {
    }

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

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

    /* User can add implementation to report the file name and line number */

    /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
