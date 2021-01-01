#ifndef __USART2_H
#define __USART2_H

#include "dma.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "param.h"
#include "stm32f4xx_usart.h"
#include "epos.h"
#include "stm32f4xx_can.h"

void USART2_Configuration(void);
void UsartLCDshow(void);

#endif
