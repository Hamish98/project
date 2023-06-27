#ifndef __DHT11_H_
#define __DHT11_H_

#include "gpio.h"
#include "tim.h"
#include "stm32f1xx_hal.h"

#define DHT11_PORT GPIOC
#define DHT11_PIN GPIO_PIN_4

#define DHT11_OUT_1 HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET)
#define DHT11_OUT_0 HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET)

#define DHT11_IN    HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)

typedef struct
{
	uint8_t humi_int; //湿度整数部分
	uint8_t humi_dec; //湿度小数部分
	uint8_t temp_int; //温度整数部分
	uint8_t temp_dec; //温度小数部分
	uint8_t check_sum; //校验和
} DHT11_Data_TypeDef;

void bsp_delay_us(uint16_t us);

uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data);

#endif /* __DHT11_H_ */
