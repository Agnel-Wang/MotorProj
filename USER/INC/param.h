#ifndef _PARAM_H_
#define _PARAM_H_

#include "motor.h"
#include "led.h"
#include "buffer.h"
#include "vesc.h"

#define RESET_PRO {__set_FAULTMASK(1);NVIC_SystemReset();}

/**
 * @description: serial communication buffer size
 */
typedef enum
{
  USART1_Tx_BufferSize = 256, //串口一发送数据长度
  USART2_Tx_BufferSize = 256, //串口二发送数据长度
  USART3_Tx_BufferSize = 32,  //串口三发送数据长度
  USART1_Rx_BufferSize = 32, //串口一接收数据长度（接收中断内使用）
  USART2_Rx_BufferSize = 32, //串口二接收数据长度（接收中断内使用）
  USART3_Rx_BufferSize = 32, //串口三接收数据长度（接收中断内使用）
} Usart_Enum;

/**
 * @description: serial communication data buffer struct
 */
typedef struct{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //串口一发送数据
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //串口一接收数据

  u8 TxBuffer_UASRT2[USART2_Tx_BufferSize]; //串口二发送数据
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //串口二接收数据

  u8 TxBuffer_UASRT3[USART3_Tx_BufferSize]; //串口三发送数据
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //串口三接收数据
} Usart_Struct;

/****标志位结构体****/
typedef struct{
    u8 led;
    volatile bool Can1CtrlList_Enable; // CAN1报文控制表启动标志位
    volatile bool Can2CtrlList_Enable; // CAN2报文控制表启动标志位
    volatile bool Can1SendqueueFULL;   // CAN1发送队列满
    volatile bool Can2SendqueueFULL;   // CAN2发送队列满
    volatile bool CanSendqueueEMPTY;   // CAN队列为空
    volatile bool has_timeout[8];      // CAN报文接收超时
    volatile bool poor_contact[8];     //接触不良
    volatile bool Usart1DMASendFinish; // Usart1_DMA传输
    volatile bool Usart2DMASendFinish; // Usart2_DMA传输
    bool valve[4];                     //当前电磁阀状态
} FlagTypeDef;

extern sc16 KickWaitTim;
extern sc32 KickPulse, KickSP;
extern Usart_Struct usart;
extern FlagTypeDef flag;

void param_Init(void);
void Flag_Init(void);
 void ChangeData(u8* buff2,u8* buff1);

#endif
