#ifndef __BEEP__H
#define __BEEP__H

#include "sys.h"
#include "stdlib.h"	  
#include "delay.h"

//定义GPIOB的位地址变量宏，位输入宏，输出宏
#define Nomal_H PBout(0)=1
#define Nomal_T PBout(1)=1
#define Nomal_B PBout(4)=1

void beep_Init(void);
#endif
