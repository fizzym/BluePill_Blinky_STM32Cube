#include "stm32f1xx.h"

#define LED_PIN                  GPIO_PIN_13
#define LED_GPIO_PORT            GPIOC
//As the LED is on port C we must enable the GPIOC clock
#define LED_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

/**
 * @brief  Error handling hook.
 * @param  file
 * @param  line
 * @retval None
 */
void _Error_Handler(const char* file, int line) {
	//TODO: Signal error occured.
}

/**Initializes the CPU, AHB and APB busses clocks
 * @brief  System Clock Configuration. Initializes the CPU, AHB and APB
 *         busses clocks.
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_ClkInitTypeDef clkinitstruct = {0};
    RCC_OscInitTypeDef oscinitstruct = {0};

    // Enable HSE Oscillator and activate PLL with HSE as source
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscinitstruct.HSEState = RCC_HSE_ON;
    oscinitstruct.HSIState = RCC_HSI_OFF;
    oscinitstruct.PLL.PLLState = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and
    // PCLK2 clocks dividers
    clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2);
    clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
}

/**
 * @brief  Main function. Contains lowest priority loop.
 * @param  None
 * @retval 0 (not applicable)
 */
int main(void) {
    if (HAL_Init()!=HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    SystemClock_Config();

    //Configure the LED pin:
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = LED_PIN;
    gpio.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(LED_GPIO_PORT, &gpio);

    int delay_ms = 500;
    while (1) {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(delay_ms);
    }

    return 0;
}

/**
 * @brief  SysTick interrupt handler (set at 1ms).
 *         We must hook the systick in order for HAL_Delay to work.
 *         If this is not done the HAL_Delay time will not increment and
 *         calling HAL_Delay() will block.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void){
    HAL_IncTick();
}