#ifndef _BEEP_H
#define _BEEP_H

#include "sys.h"
#include "delay.h"
#include "includes.h"

#define BEEP_ON     	 GPIOA->BSRRL = GPIO_Pin_8
#define BEEP_OFF     	 GPIOA->BSRRH = GPIO_Pin_8

void Beep_Configuration(void);
void Beep_Show(u8 num);

#endif
