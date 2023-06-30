#include "delay.h"

// use HSI

void Delay_us(uint32_t us)
{       
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
    HAL_Delay(us);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
}

void Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
