#ifndef _EPOS_H
#define _EPOS_H

#include "param.h"
#include "queue.h"
#include "canctrllist.h"
#include "stm32f4xx.h"

/****EPOS_CAN报文ID****/
#define EPOS_Motor1_TX 0x601
#define EPOS_Motor2_TX 0x602
#define EPOS_Motor3_TX 0x603
#define EPOS_Motor4_TX 0x604
#define EPOS_Motor1_RX 0x581
#define EPOS_Motor2_RX 0x582
#define EPOS_Motor3_RX 0x583
#define EPOS_Motor4_RX 0x584
#define EPOS_Motor1_Init 0x181
#define EPOS_Motor2_Init 0x182
#define EPOS_Motor3_Init 0x183
#define EPOS_Motor4_Init 0x184
#define EPOS_Motor1_error 0x81
#define EPOS_Motor2_error 0x82
#define EPOS_Motor3_error 0x83
#define EPOS_Motor4_error 0x84

extern u8 eposInitOK;

void EPOS_SetMode(u8 ID, s32 mode, u8 InInConGrpFlag);
void EPOS_StartMotor(u8 ID, u8 InInConGrpFlag);
void EPOS_RelieveMotor(u8 ID, u8 InInConGrpFlag);
void EPOS_SetPVMspeed(u8 ID, s32 speed, u8 InConGrpFlag);
void EPOS_EnableOperation(u8 ID, u8 InConGrpFlag);
void EPOS_QuickStop(u8 ID, u8 InConGrpFlag);
void EPOS_Halt(u8 ID, u8 InConGrpFlag);
void EPOS_ClearFault(u8 ID, u8 InConGrpFlag);
void EPOS_ReadStatusword(u8 ID, u8 InConGrpFlag);
void EPOS_ReadPosition(u8 ID, u8 InConGrpFlag);
void EPOS_ReadVelocity(u8 ID, u8 InConGrpFlag);
void EPOS_ReadTorque(u8 ID, u8 InConGrpFlag);
void EPOS_BootUP(u8 ID, u8 InConGrpFlag);

#endif
