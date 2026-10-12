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

const int MAX_LED = 4;

int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};
int hour = 15, minute = 53, second = 50;
const int global_delay = 25;

int delay = global_delay; //changed delay here (1) cycle_time / 10
uint8_t led_count = 0;

GPIO_TypeDef *segPorts[7] =
{
    GPIOB, GPIOB, GPIOB,
    GPIOB, GPIOB, GPIOB, GPIOB
};

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

/* MATRIX_BEGIN ------------------------------------------------------------- */
/* LED matrix: ENM0..ENM7 (column enables through the ULN2803) are on
 * PA2, PA3, PA10, PA11, PA12, PA13, PA14, PA15; ROW0..ROW7 are on PB8..PB15. */
const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;

/* One byte per COLUMN: bit k = ROWk (bit 0 = ROW0 = PB8). Letter "A". */
uint8_t matrix_buffer[8] = {0x00, 0xFC, 0x12, 0x11, 0x11, 0x12, 0xFC, 0x00};

GPIO_TypeDef *enmPorts[8] =
{
    GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA
};

uint16_t enmPins[8] =
{
    GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_10, GPIO_PIN_11,
    GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
};

#define MATRIX_COL_ON           GPIO_PIN_RESET
#define MATRIX_COL_OFF          GPIO_PIN_SET
#define MATRIX_ROW_ACTIVE_HIGH  0
/* MATRIX_END --------------------------------------------------------------- */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

void display7SEG(GPIO_TypeDef *ports[7], uint16_t pins[7], uint8_t num);
void update7SEG(int index);
void matrixAllOff(void);
void updateLEDMatrix(int index);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 1;          /* ms per timer tick (TIM2 now ticks every 1 ms) */
int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;       /* LED matrix scan timer */
int timer2_flag = 0;
#define MATRIX_SCAN_MS 125     /* one column every 2 ms -> 16 ms per frame (~62 Hz) */

void setTimer0(int duration ){
	timer0_counter = duration / TIMER_CYCLE ;
	timer0_flag = 0;
}
void setTimer1(int duration) {
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}
void setTimer2(int duration) {
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}
void timer_run() {
    if (timer0_counter > 0) {
        timer0_counter--;
        if (timer0_counter == 0) timer0_flag = 1;
    }
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter == 0) timer1_flag = 1;
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter == 0) timer2_flag = 1;
    }
}

void display7SEG(GPIO_TypeDef *ports[7], uint16_t pins[7], uint8_t num)
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
        return;

    for (int i = 0; i < 7; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], seg[num][i]);
    }
}

void update7SEG(int index)
{
    /* Turn all displays off */
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_6 | GPIO_PIN_7 |
                      GPIO_PIN_8 | GPIO_PIN_9,
                      GPIO_PIN_SET);

    switch (index)
    {
        case 0:
            display7SEG(segPorts, segPins, led_buffer[0]);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
            break;

        case 1:
            display7SEG(segPorts, segPins, led_buffer[1]);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            break;

        case 2:
            display7SEG(segPorts, segPins, led_buffer[2]);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
            break;

        case 3:
            display7SEG(segPorts, segPins, led_buffer[3]);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}
void updateClockBuffer(void) {
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

/* MATRIX_FUNC_BEGIN -------------------------------------------------------- */
/* Disable every column (ENM pins) */
static void matrixColumnsOff(void)
{
    for (int i = 0; i < 8; i++)
        HAL_GPIO_WritePin(enmPorts[i], enmPins[i], MATRIX_COL_OFF);
}

/* Write 8 row bits to PB8..PB15 in one atomic BSRR write; PB0..PB6 (7-seg)
 * and PB7 are not touched. Bit k of 'bits' = 1 means "light ROWk". */
static void matrixWriteRows(uint8_t bits)
{
    uint8_t out = MATRIX_ROW_ACTIVE_HIGH ? bits : (uint8_t)(~bits);
    /* low half of BSRR sets pins, high half resets pins */
    GPIOB->BSRR = ((uint32_t)(uint8_t)(~out) << 24) | ((uint32_t)out << 8);
}

/* Everything off (call once at start; also makes polarity irrelevant at boot) */
void matrixAllOff(void)
{
    matrixColumnsOff();
    matrixWriteRows(0x00);
}

/* Show column 'index' (0..7): disable all columns first, put that column's
 * data on the rows, then enable only that column, so there is no ghosting. */
void updateLEDMatrix(int index)
{
    matrixColumnsOff();
    switch (index)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            matrixWriteRows(matrix_buffer[index]);
            HAL_GPIO_WritePin(enmPorts[index], enmPins[index], MATRIX_COL_ON);
            break;

        default:
            break;
    }
}
/* MATRIX_FUNC_END ---------------------------------------------------------- */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    MX_GPIO_Init();
    MX_TIM2_Init();

    /* USER CODE BEGIN 2 */

    /* EN pins are active-low */
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_6 | GPIO_PIN_7 |
                      GPIO_PIN_8 | GPIO_PIN_9,
                      GPIO_PIN_SET);

    /* Middle LEDs off */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    matrixAllOff();
    updateClockBuffer();
    HAL_TIM_Base_Start_IT(&htim2);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    setTimer0(1000);
    setTimer1(250);
    setTimer2(MATRIX_SCAN_MS);
    while (1) {
        /* Update 7-segment display every 250 ms */
        if (timer1_flag == 1) {
            timer1_flag = 0;
            update7SEG(index_led);
            index_led++;
            if (index_led >= MAX_LED) index_led = 0;
            setTimer1(250);
        }
        /* Update clock every 1 second */
        if (timer0_flag == 1) {
            timer0_flag = 0;
            second++;
            if (second >= 60) {
                second = 0;
                minute++;
            }
            if (minute >= 60) {
                minute = 0;
                hour++;
            }
            if (hour >= 24) hour = 0;
            updateClockBuffer();
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
            setTimer0(1000);
        }
        /* Scan the LED matrix: one column every MATRIX_SCAN_MS */
        if (timer2_flag == 1) {
            timer2_flag = 0;
            updateLEDMatrix(index_led_matrix);
            index_led_matrix++;
            if (index_led_matrix >= MAX_LED_MATRIX) index_led_matrix = 0;
            setTimer2(MATRIX_SCAN_MS);
        }
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
    htim2.Init.Prescaler = 799;      /* 8 MHz / 800 = 10 kHz (was 7999 -> 1 kHz) */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 9;           /* 10 counts at 10 kHz = 1 ms per interrupt */
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
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

    /* GPIOA initial state: PA2/PA3 = ENM0/ENM1, PA4 = DOT, PA5 = D1,
     * PA6..PA9 = EN0..EN3, PA10..PA15 = ENM2..ENM7 */
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_2 | GPIO_PIN_3 |
                      GPIO_PIN_4 | GPIO_PIN_5 |
                      GPIO_PIN_6 | GPIO_PIN_7 |
                      GPIO_PIN_8 | GPIO_PIN_9 |
                      GPIO_PIN_10 | GPIO_PIN_11 |
                      GPIO_PIN_12 | GPIO_PIN_13 |
                      GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_RESET);

    /* GPIOB initial state: PB0..PB6 = SEG0..SEG6, PB8..PB15 = ROW0..ROW7 */
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_0 | GPIO_PIN_1 |
                      GPIO_PIN_2 | GPIO_PIN_3 |
                      GPIO_PIN_4 | GPIO_PIN_5 |
                      GPIO_PIN_6 |
                      GPIO_PIN_8 | GPIO_PIN_9 |
                      GPIO_PIN_10 | GPIO_PIN_11 |
                      GPIO_PIN_12 | GPIO_PIN_13 |
                      GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_RESET);

    /* PA2..PA15 */
    GPIO_InitStruct.Pin =
        GPIO_PIN_2 | GPIO_PIN_3 |
        GPIO_PIN_4 | GPIO_PIN_5 |
        GPIO_PIN_6 | GPIO_PIN_7 |
        GPIO_PIN_8 | GPIO_PIN_9 |
        GPIO_PIN_10 | GPIO_PIN_11 |
        GPIO_PIN_12 | GPIO_PIN_13 |
        GPIO_PIN_14 | GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PB0..PB6 and PB8..PB15 */
    GPIO_InitStruct.Pin =
        GPIO_PIN_0 | GPIO_PIN_1 |
        GPIO_PIN_2 | GPIO_PIN_3 |
        GPIO_PIN_4 | GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_8 | GPIO_PIN_9 |
        GPIO_PIN_10 | GPIO_PIN_11 |
        GPIO_PIN_12 | GPIO_PIN_13 |
        GPIO_PIN_14 | GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) timer_run();
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
  * @param  line: assert_param error line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */

    /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
