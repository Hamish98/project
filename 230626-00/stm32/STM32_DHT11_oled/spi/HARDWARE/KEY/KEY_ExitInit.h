#ifndef KEY_ExitInit_H
#define KEY_ExitInit_H

#include "stm32f10x.h"
#include "sys.h"

#define KEY0 PAin(0)
#define KEY1 PAin(1)
#define KEY2 PAin(2)	 

void KEY_ExitInit(void);

#endif
