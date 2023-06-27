#include "KEY_ExitInit.h"

void KEY_ExitInit(void)
{
	GPIO_InitTypeDef PB0;     
	EXTI_InitTypeDef EPB0;
	NVIC_InitTypeDef NPB0;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
  PB0.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
  PB0.GPIO_Mode = GPIO_Mode_IPU;  //输入
	PB0.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&PB0);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//中断线映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	//SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource2);
	
	EPB0.EXTI_Line    = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 ;
	EPB0.EXTI_Mode    = EXTI_Mode_Interrupt;
	EPB0.EXTI_Trigger = EXTI_Trigger_Rising;  //下降沿触发
	EPB0.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EPB0);
	
  NPB0.NVIC_IRQChannel = EXTI0_IRQn;
	NPB0.NVIC_IRQChannelPreemptionPriority = 2; 
	NPB0.NVIC_IRQChannelSubPriority = 1;  
	NPB0.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NPB0);
	
	NPB0.NVIC_IRQChannel = EXTI1_IRQn;
	NPB0.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NPB0);

	NPB0.NVIC_IRQChannel = EXTI2_IRQn;
	NPB0.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NPB0);
}
