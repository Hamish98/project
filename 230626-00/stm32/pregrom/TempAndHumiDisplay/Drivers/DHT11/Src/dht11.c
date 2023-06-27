#include "dht11.h"

/**
 * @brief 微妙级延时
 *
 */
 void bsp_delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	
	HAL_TIM_Base_Start(&htim2);
	
	while(__HAL_TIM_GET_COUNTER(&htim2) != us);
	
	HAL_TIM_Base_Stop(&htim2);
}

/**
 * @brief DHT11 输出模式
 *
 */
void DHT11_Mode_OUT_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = DHT11_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/**
 * @brief DHT11 输入模式
 *
 */
void DHT11_Mode_IN_NP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = DHT11_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/**
 * @brief DHT11 读取字节
 *
 */
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp = 0;
	
	for(i = 0; i < 8; i++)
	{
		while(DHT11_IN == 0); //等待低电平结束
		
		bsp_delay_us(40); //延时40微妙
		
		if(DHT11_IN == 1)
		{
			while(DHT11_IN == 1); //等待高电平结束
			
			temp |= (uint8_t)(0x01 << (7 - i));
		}
		else
		{
			temp &= (uint8_t)~(0x01 << (7 - i));
		}
	}
	return temp;
}

/**
 *
 *
 */
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_OUT_PP();  //主机输出。主机拉低
	DHT11_OUT_0;
	HAL_Delay(18); //延时18ms
	
	DHT11_OUT_1; //主机拉高，延时30us
	bsp_delay_us(30);
	
	DHT11_Mode_IN_NP(); //主机输入，获取DHT11数据
	
	if(DHT11_IN == 0) //收到从机应答
	{
		while(DHT11_IN == 0); //等到从几应答的低电平结束
		
		while(DHT11_IN == 1); //等待从几应答的高电平结束
		
		/*开始接受数据*/
		DHT11_Data->humi_int = DHT11_ReadByte();
		DHT11_Data->humi_dec = DHT11_ReadByte();	
		DHT11_Data->temp_int = DHT11_ReadByte();		
		DHT11_Data->temp_dec = DHT11_ReadByte();
		DHT11_Data->check_sum = DHT11_ReadByte();
		
		DHT11_Mode_OUT_PP(); //读取结束，主机拉高
		DHT11_OUT_1;
		
		//数据校验
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_dec + DHT11_Data->temp_int + DHT11_Data->temp_dec)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	
	return 1;
}

