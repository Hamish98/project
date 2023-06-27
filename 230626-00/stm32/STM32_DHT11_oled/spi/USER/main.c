#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "KEY_ExitInit.h"
#include "oled.h"
#include "beep.h"
#include "bsp_dht11.h"

u8 war[8];
u8 yuzhi[4] = {30, 70, 20, 30};
u16 count=0;

DHT11_Data_TypeDef DHT11_Data;
u8 arrow[] = {0x00,0x04,0x04,0x04,0x15,0x0E,0x04};
u8 shuid[] = {0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0x3E,0x06,0x07,0x3E,0xF8,0xE0,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0x3F,0x7D,0x78,0xF0,0xE0,0x60,0x70,0x3F,0x1F,0x00,0x00};
u8 taiy[] = {0x00,0x00,0x80,0x80,0x98,0xD8,0xF0,0xF0,0xFE,0xFE,0xF0,0xE0,0xD8,0x98,0x80,0x80,
0x00,0x00,0x01,0x01,0x19,0x1D,0x0F,0x07,0x3F,0x3F,0x0F,0x07,0x1F,0x19,0x01,0x01,0x00};
u8 nomal[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int arrow_idx[2] = {-1,0};

void buzz(u8 pin)
{
	if(count < 4000)
	{
		PBout(pin)=0;
	}
	else if(count < 8000)
	{
		PBout(pin)=1;
	}
	else if(count < 12000)
	{
		PBout(pin)=0;
	}
	else if(count < 16000)
	{
		PBout(pin)=1;
	}
	else if(count < 20000)
	{
		PBout(pin)=0;
	}
}

/*
* oled 显示/切换箭头
*/
void show_arrow(void)
{
	if(arrow_idx[0] == arrow_idx[1])
	{
		return;
	}
	if(arrow_idx[0] == -1) arrow_idx[0] = 0;
	arrow_idx[0] = arrow_idx[1];
	switch(arrow_idx[1])
	{
		case 0:
			OLED_DrawBMP(64,5,71,6,nomal);
			OLED_DrawBMP(0,3, 7,4,arrow);
			break;
		case 1:
			OLED_DrawBMP(0,3, 7,4,nomal);
			OLED_DrawBMP(64,3,71,4,arrow);
			break;
		case 2:
			OLED_DrawBMP(64,3,71,4,nomal);
			OLED_DrawBMP(0,5, 7,6,arrow);
			break;
		case 3:
			OLED_DrawBMP(0,5, 7,6,nomal);
			OLED_DrawBMP(64,5,71,6,arrow);
			break;
		default:
			break;
	}
}

/*
* oled 显示数据
*/
void show_oled(void)
{
		OLED_ShowCHinese(0,0,0);//温
		OLED_ShowCHinese(18,0,1);//湿
		OLED_ShowCHinese(36,0,2);//度
		OLED_ShowCHinese(54,0,3);//检
		OLED_ShowCHinese(72,0,4);//测
		OLED_ShowCHinese(90,0,5);//系
		OLED_ShowCHinese(108,0,6);//统
		if(DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
		{        
			sprintf((char *)war, "L_H:%d", yuzhi[0]);
			OLED_ShowString(8, 2, war);
			sprintf((char *)war, "H_H:%d",yuzhi[1]);
			OLED_ShowString(72, 2, war);
			sprintf((char *)war, "L_T:%d", yuzhi[2]);	
			OLED_ShowString(8, 4, war);
			sprintf((char *)war, "H_T:%d",yuzhi[3]);
			OLED_ShowString(72, 4, war);
			OLED_DrawBMP(0,6,15,8,shuid);
			sprintf((char *)war, ":%d%d.%d  ", DHT11_Data.humi_int / 10, DHT11_Data.humi_int % 10, DHT11_Data.humi_deci % 10);
			OLED_ShowString(18, 6, war);
			OLED_DrawBMP(64,6,80,8,taiy);
			sprintf((char *)war, ":%d%d.%d", DHT11_Data.temp_int / 10, DHT11_Data.temp_int % 10, DHT11_Data.temp_deci % 10);
			OLED_ShowString(80, 6, war);
 }
}


/*
按键一
*/
void EXTI0_IRQHandler(void)
{
	delay_ms(20);	
	if(KEY0 == 0)
  {
     yuzhi[arrow_idx[1]]--;;
	}
    EXTI_ClearITPendingBit(EXTI_Line0);
}

/*
按键二
*/
void EXTI1_IRQHandler(void)
{
	delay_ms(20);
	if(KEY1 == 0)
  {	
		 yuzhi[arrow_idx[1]]++;	
	}
  EXTI_ClearITPendingBit(EXTI_Line1);
}

/*
按键三
*/
void EXTI2_IRQHandler(void)
{
	delay_ms(20);
	if(KEY2 == 0)
  {
		 arrow_idx[1] = (arrow_idx[1] + 1) % 4;
	}
  EXTI_ClearITPendingBit(EXTI_Line2);
}

int main(void)
{
		u8 H_Flag=0,T_Flag=0;
    
    delay_init();	     //延时初始化
    DHT11_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    beep_Init();
		KEY_ExitInit();
    OLED_Init();
    OLED_Clear();
    delay_ms(20);
		show_oled();
    while(1)
    {		 
				count++;
				//读取DHT11消耗的时间太长
        if(count==20000)
        {
            show_oled();
            count=0;
        }

				show_arrow();
				
				T_Flag=1;
				if( DHT11_Data.temp_int < yuzhi[2] || DHT11_Data.temp_int > yuzhi[3])
        {
            buzz(1);
        }
				else 
				{
					Nomal_T;
					T_Flag=0;
				}

				H_Flag=1;
				if( DHT11_Data.humi_int < yuzhi[0] || DHT11_Data.humi_int > yuzhi[1])
        {
            buzz(0);
        }
				else
				{
					H_Flag=0;
					Nomal_H;
				}
				
				if(H_Flag || T_Flag)
				{
					buzz(4);
				}
				else
				{
					Nomal_B;
				}
    }
}
