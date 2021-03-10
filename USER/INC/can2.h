#ifndef __CAN2_H
#define __CAN2_H

#include "elmo.h"
#include "param.h"
#include "queue.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "vesc.h"

//电磁阀状态
#define valve_open 		true
#define valve_close 	false

void CAN2_Configuration(void);
void currentInput(u8 id);
void valveCtrl(u8 ID, bool status);

#endif
