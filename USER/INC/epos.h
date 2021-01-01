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

//状态结构体
typedef struct{

  vu16 statusword;
}EPOSmotorStatus;

typedef struct{
  vs16 angle;
  vs32 pulse;
  vs16 speed;
}EPOSmotorValue;

//EPOS电机结构体
typedef struct{
  EPOSmotorValue EPOSmotorValSet, EPOSmotorValReal;
  EPOSmotorStatus status;
}EPOSmotorTypedef;

extern u8 eposInitOK;
extern EPOSmotorTypedef EPOSmotor[4];

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

#endif
