#include "can1.h"

static s16 s16TempData[4]; u16 u16TempData[4];

void CAN1_Configuration(void) 
{
  CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
  CAN_InitStructure.CAN_ABOM = DISABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
  CAN_InitStructure.CAN_NART = DISABLE; //禁止报文自动发送，即只发送一次，无论结果如何
  CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
  CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN硬件工作在正常模式

  /* Seting BaudRate */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
  CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
  CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 1; //一次性发射
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh =((0x01314156 << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (0x01314156 << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FilterInitStructure.CAN_FilterNumber = 2; //转箭防守
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh =((0x01314157 << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (0x01314157 << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterNumber = 3; //转箭防守
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh =((0x01314150 << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (0x01314150 << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FilterInitStructure.CAN_FilterNumber = 4; //翻阅
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh =((0x00010500 << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (0x00010500 << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

static void answer_master(CanRxMsg *rx_message);


void CAN1_RX0_IRQHandler(void) 
{
  CanRxMsg rx_message;
  if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) 
  {
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_ClearFlag(CAN1, CAN_IT_FMP0);
    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
    
    if((rx_message.IDE == CAN_Id_Extended)&&(rx_message.RTR==CAN_RTR_Data))
    {
      if((rx_message.Data[0]=='S')&&(rx_message.Data[1]=='U'))//一次性发射
      {
        GPIOB->ODR ^= PIN12;
        
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141316;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
      else if((rx_message.Data[0]=='Z')&&(rx_message.Data[1]=='S'))//转箭速度
      {
        float speed;
        DecodeFloatData_From_4byte(&speed, &rx_message.Data[3]);
        VESCmotor[0].enable = rx_message.Data[2];
        VESCmotor[0].valSet.speed = speed;
        
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141317;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
      else if((rx_message.Data[0]=='F')&&(rx_message.Data[1]=='J'))//转台角度
      {
        motor[0].enable = rx_message.Data[2];
        float angle;
        DecodeFloatData_From_4byte(&angle, &rx_message.Data[3]);
        motor[0].valueSet.angle = angle;
      
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141317;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
      else if((rx_message.Data[0]=='G')&&(rx_message.Data[1]=='D'))//转台高度
      {
        motor[1].enable = rx_message.Data[2];
        float angle;
        DecodeFloatData_From_4byte(&angle, &rx_message.Data[3]);
        motor[1].valueSet.angle = angle;
      
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141317;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
      else if((rx_message.Data[0]=='F')&&(rx_message.Data[1]=='Z'))//3508寻零
      {
        motor[0].enable = true;
        motor[0].mode = zero;
        motor[0].status.zero = false;
        motor[1].enable = true;
        motor[1].mode = zero;
        motor[1].status.zero = false;
        
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141317;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
      else if((rx_message.Data[0]=='S')&&(rx_message.Data[1]=='T')&&(rx_message.Data[2]=='A')&&(rx_message.Data[3]=='L'))//急停
      {
        VESCmotor[0].enable = false;
        motor[0].begin = false;
        motor[1].begin = false;
          
        CanTxMsg tx_message;
        tx_message.IDE = CAN_Id_Extended;
        tx_message.ExtId = 0x05141317;
        tx_message.RTR = CAN_RTR_Data;
        tx_message.DLC = rx_message.DLC;
        memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
        CAN_Transmit(CAN1, &tx_message);
      }
#if USE_TMOTOR
        if(rx_message.ExtId == 0x00010500)
        {
            if((rx_message.Data[0]=='G')&&(rx_message.Data[1]=='L'))//重置腿零点
            {
                tmotor[TMOTOR_FR].status.isSetZero = true;
                tmotor[TMOTOR_FL].status.isSetZero = true;
                tmotor[TMOTOR_RR].status.isSetZero = true;
                tmotor[TMOTOR_RL].status.isSetZero = true;
                
                CanTxMsg tx_message;
                tx_message.IDE = CAN_Id_Extended;
                tx_message.ExtId = 0x00050101;
                tx_message.RTR = CAN_RTR_Data;
                tx_message.DLC = rx_message.DLC;
                memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
                CAN_Transmit(CAN1, &tx_message);
            }
            else if((rx_message.Data[0]=='M')&&(rx_message.Data[1]=='O')) //腿 失能使能
            {
                bool state = rx_message.Data[2];
                if(state)
                {
                    tmotor[TMOTOR_FL].mode = position;
                    tmotor[TMOTOR_FR].mode = position;
                    tmotor[TMOTOR_RL].mode = position;
                    tmotor[TMOTOR_RR].mode = position;
                }
                else
                {
                    tmotor[TMOTOR_FL].mode = 0;
                    tmotor[TMOTOR_FR].mode = 0;
                    tmotor[TMOTOR_RL].mode = 0;
                    tmotor[TMOTOR_RR].mode = 0;
                }
                
                CanTxMsg tx_message;
                tx_message.IDE = CAN_Id_Extended;
                tx_message.ExtId = 0x00050101;
                tx_message.RTR = CAN_RTR_Data;
                tx_message.DLC = rx_message.DLC;
                memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
                CAN_Transmit(CAN1, &tx_message);
            }
            else if((rx_message.Data[0]=='K')&&(rx_message.Data[1]=='L')) //步骤
            {
                s32 index = 3;
//                overturn.downtim = get_s16_from_buffer(rx_message.Data, &index);
                overturn.process = rx_message.Data[2];
                
                CanTxMsg tx_message;
                tx_message.IDE = CAN_Id_Extended;
                tx_message.ExtId = 0x00050101;
                tx_message.RTR = CAN_RTR_Data;
                tx_message.DLC = rx_message.DLC;
                memcpy(&tx_message.Data[0],&rx_message.Data[0],rx_message.DLC);
                CAN_Transmit(CAN1, &tx_message);
            }
            else if((rx_message.Data[0]=='A')&&(rx_message.Data[1]=='S')) //查询腿角度
            {
                CanTxMsg tx_message;
                tx_message.IDE = CAN_Id_Extended;
                tx_message.ExtId = 0x00050101;
                tx_message.RTR = CAN_RTR_Data;
                tx_message.DLC = 7;
                tx_message.Data[0] = 'A';
                tx_message.Data[1] = 'S';
                tx_message.Data[2] = 'K';
                s16 speed_front = tmotor[TMOTOR_FL].valReal.angle;
				s16 speed_rear = tmotor[TMOTOR_RL].valReal.angle;
                EncodeS16Data(&speed_front, &tx_message.Data[3]);
                EncodeS16Data(&speed_rear, &tx_message.Data[5]);
                CAN_Transmit(CAN1, &tx_message);
            }
        }
#endif
    }
  }
}
  
/****接收反馈原报文****/
static void answer_master(CanRxMsg *rx_message)
{
  CanTxMsg tx_message;
  if (!(CAN_Procedure.status & BIT0) || (CAN_Procedure.status & BIT1) || !(CAN_Procedure.status & BIT2)) 
  {
    if(rx_message->IDE == CAN_Id_Standard)
    {
      tx_message.IDE = CAN_Id_Standard;
      tx_message.StdId = rx_message->StdId - 0x80;
    }
    #ifdef ActionMotor
    else if(rx_message->IDE == CAN_Id_Extended)
    {
      tx_message.IDE = CAN_Id_Extended;
      tx_message.ExtId = ID_BACK;
    }
    #endif
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = rx_message->DLC;
    memcpy(&tx_message.Data[0],&rx_message->Data[0],rx_message->DLC);
    tx_message.Data[0] += 0x40;
    CAN_Transmit(CAN1, &tx_message);
  }
}
