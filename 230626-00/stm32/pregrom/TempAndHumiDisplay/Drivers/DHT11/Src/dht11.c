#include "dht11.h"

/**
 * @brief ΢���ʱ
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
 * @brief DHT11 ���ģʽ
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
 * @brief DHT11 ����ģʽ
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
 * @brief DHT11 ��ȡ�ֽ�
 *
 */
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp = 0;
	
	for(i = 0; i < 8; i++)
	{
		while(DHT11_IN == 0); //�ȴ��͵�ƽ����
		
		bsp_delay_us(40); //��ʱ40΢��
		
		if(DHT11_IN == 1)
		{
			while(DHT11_IN == 1); //�ȴ��ߵ�ƽ����
			
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
	DHT11_Mode_OUT_PP();  //�����������������
	DHT11_OUT_0;
	HAL_Delay(18); //��ʱ18ms
	
	DHT11_OUT_1; //�������ߣ���ʱ30us
	bsp_delay_us(30);
	
	DHT11_Mode_IN_NP(); //�������룬��ȡDHT11����
	
	if(DHT11_IN == 0) //�յ��ӻ�Ӧ��
	{
		while(DHT11_IN == 0); //�ȵ��Ӽ�Ӧ��ĵ͵�ƽ����
		
		while(DHT11_IN == 1); //�ȴ��Ӽ�Ӧ��ĸߵ�ƽ����
		
		/*��ʼ��������*/
		DHT11_Data->humi_int = DHT11_ReadByte();
		DHT11_Data->humi_dec = DHT11_ReadByte();	
		DHT11_Data->temp_int = DHT11_ReadByte();		
		DHT11_Data->temp_dec = DHT11_ReadByte();
		DHT11_Data->check_sum = DHT11_ReadByte();
		
		DHT11_Mode_OUT_PP(); //��ȡ��������������
		DHT11_OUT_1;
		
		//����У��
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

