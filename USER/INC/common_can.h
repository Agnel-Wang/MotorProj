#ifndef _COMMON_CAN_H
#define _COMMON_CAN_H

#include "canctrllist.h"
#include "queue.h"

void NMT_Operational(CAN_TypeDef* CANx);
void NMT_PreOperational(CAN_TypeDef* CANx);
void NMT_Stopped(CAN_TypeDef* CANx);
void NMT_ResetApplication(CAN_TypeDef* CANx);
void NMT_Communication(CAN_TypeDef* CANx);

#endif
