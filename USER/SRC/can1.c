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
#ifdef SteeringMotor
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  #if ID_SELF == MOTOR_0_3
  CAN_FilterInitStructure.CAN_FilterIdHigh =0X305<< 5;
  CAN_FilterInitStructure.CAN_FilterIdLow =0X308 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0x320 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow =0 << 5;
  #elif ID_SELF == MOTOR_1_2
  CAN_FilterInitStructure.CAN_FilterIdHigh =0X306 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow =0X307 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0x320 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow =0x309 << 5;
  #endif
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  #if ID_SELF == MOTOR_0_3
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =0x281<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x282<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0x283 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =0x284 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
  #endif
#endif
#ifdef ActionMotor
  CAN_FilterInitStructure.CAN_FilterNumber = 0; //屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = ((ID_SELF << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (ID_SELF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterNumber = 1; //屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh =((BROADCAST_ID << 3) & 0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = (BROADCAST_ID << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
#endif
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
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
    memcpy(&rx_message->Data[0],&rx_message->Data[0],rx_message->DLC);
    CAN_Transmit(CAN1, &tx_message);
  }
}

/****反馈电机角度****/
static void feedbackAngle(u16 motorID)
{
  CanTxMsg tx_message;
  tx_message.StdId =motorID-0x80;
  tx_message.IDE = CAN_Id_Standard;
  tx_message.RTR = CAN_RTR_Data;
  tx_message.DLC = 5;
  tx_message.Data[0] = 'B';
  tx_message.Data[1] = 'B';
  tx_message.Data[2] = 'G';
  #if ID_SELF==MOTOR_0_3
    if(motorID==0x305)
    {
      s16TempData[0]=motor[0].valueReal.angle*30.f/GearRatio;
      EncodeS16Data(&s16TempData[0], &tx_message.Data[3]);
    }
    else if(motorID==0x308)
    {
      s16TempData[3]=motor[3].valueReal.angle*30.f/GearRatio;
      EncodeS16Data(&s16TempData[3], &tx_message.Data[3]);      
    }
  #elif ID_SELF==MOTOR_1_2
    if(motorID==0x306)
    {
      s16TempData[1]=motor[1].valueReal.angle*30.f/GearRatio;
      EncodeS16Data(&s16TempData[1], &tx_message.Data[3]);
    }
    else if(motorID==0x307)
    {
      s16TempData[2]=motor[2].valueReal.angle*30.f/GearRatio;
      EncodeS16Data(&s16TempData[2], &tx_message.Data[3]);      
    }
  #endif
  CAN_Transmit(CAN1, &tx_message);
}

void CAN1_RX0_IRQHandler(void) 
{
  CanRxMsg rx_message;
  if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) 
  {
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_ClearFlag(CAN1, CAN_IT_FMP0);
    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
  	#ifdef VER
			if((u8)rxmsg.Data[1] >= (u8)('A'+0x40))		
			{
				CAN_Procedure.status |= BIT0;//当前报文拥有流程号
				CAN_Procedure.order_now = (u8)rxmsg.Data[rxmsg.DLC - 1];//流程号为报文最后一个字节	
				if(CAN_Procedure.status & BIT7)
				{
					CAN_Procedure.order_valid = CAN_Procedure.order_now;			
					CAN_Procedure.order_valid++;	
					CAN_Procedure.status = 0x03;				
				}
				else
				{
					CAN_Procedure.status = 0x01;							
					if(CAN_Procedure.order_valid == CAN_Procedure.order_now)
					{
						CAN_Procedure.status |= BIT1;				
						CAN_Procedure.order_valid ++;
					}
					else
					{
						CAN_Procedure.status &= ~BIT1;					
						if(CAN_Procedure.order_valid > 127)
						{
							if((CAN_Procedure.order_now >= (CAN_Procedure.order_valid - 128)) && (CAN_Procedure.order_now < CAN_Procedure.order_valid))						
								CAN_Procedure.status &= ~BIT2;
							else
								CAN_Procedure.status |= BIT2;	
						}
						else
						{
							if((CAN_Procedure.order_now <= (CAN_Procedure.order_valid + 127)) && (CAN_Procedure.order_now > CAN_Procedure.order_valid))							
								CAN_Procedure.status |= BIT2;	
							else
								CAN_Procedure.status &= ~BIT2;	
						}
					}
				}
			}
			else
			{
				CAN_Procedure.status &= ~BIT0;
			}
		#else		
			CAN_Procedure.status = (BIT0|BIT1);//流程号关闭，则默认具有正确流程号
		#endif
    #ifdef SteeringMotor  //转向电机报文处理
    if((rx_message.IDE == CAN_Id_Standard)&&(rx_message.RTR==CAN_RTR_Data))
    {
      #ifdef PassRobot
        #if ID_SELF==MOTOR_1_2
          if((rx_message.StdId==0x306)||(rx_message.StdId==0x307))
        #elif ID_SELF==MOTOR_0_3
          if((rx_message.StdId==0x305)||(rx_message.StdId==0x308))
        #endif
        {
          u8 SteeringID=rx_message.StdId-0x305;
          if(rx_message.Data[0]=='C'&&rx_message.Data[1]=='W'&&rx_message.Data[2]=='H'&&rx_message.Data[3]=='U')
          {
            answer_master(&rx_message);
          }
          else if(rx_message.Data[0] == 'C' && rx_message.Data[1] == 'H')//旋转
          {
            DecodeS16Data(&s16TempData[SteeringID],&rx_message.Data[2]);
            motor[SteeringID].valueSet.angle = s16TempData[SteeringID]/30.f*GearRatio;
            motor[SteeringID].begin=true;
          }
          else if(rx_message.Data[0]=='S'&&rx_message.Data[1]=='P')//速度设定
          {
            DecodeS16Data(&u16TempData[SteeringID],&rx_message.Data[2]);
            motor[SteeringID].limit.posSPlimit=u16TempData[SteeringID];
            answer_master(&rx_message);
          }
          else if(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&rx_message.Data[2]==1)//电机使能
          {
            motor[SteeringID].mode=position;//位置模式
            motor[SteeringID].begin=true;//锁当前位置
            motor[SteeringID].limit.isPosLimitON=true;//开启位置限制保护
            motor[SteeringID].limit.maxAngle=540.f;//最大轴前旋转角度
            motor[SteeringID].limit.isPosSPLimitOn=true;//开始位置速度限制保护
            motor[SteeringID].enable=true;//电机使能
            answer_master(&rx_message);
          }
          else if(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&rx_message.Data[2]==0)//电机失能
          {
            motor[SteeringID].enable=false;
            motor[SteeringID].begin=false;
            answer_master(&rx_message);
          }
          else if(rx_message.Data[0]=='S'&&rx_message.Data[1]=='T')//急停
          {
            motor[SteeringID].begin=false;//锁当前位置
            answer_master(&rx_message);
          }
          else if(rx_message.Data[0] == 'B' && rx_message.Data[1] == 'B' && rx_message.Data[2] == 'G')
          {
            feedbackAngle(rx_message.StdId);
          }
        }
        if(rx_message.StdId==0x320)
        {
          //FIXME: 修改此处的begin
            motor[0].begin=true;motor[1].begin=true;motor[2].begin=true;motor[3].begin=true;
          /****************/
            DecodeS16Data(&s16TempData[0], &rx_message.Data[0]);
            DecodeS16Data(&s16TempData[1], &rx_message.Data[2]);
            DecodeS16Data(&s16TempData[2], &rx_message.Data[4]);
            DecodeS16Data(&s16TempData[3], &rx_message.Data[6]);              
          #if ID_SELF==MOTOR_1_2
            motor[1].valueSet.angle = s16TempData[1]/30.f*GearRatio;
            motor[1].begin=true;motor[1].mode=position;
            motor[2].valueSet.angle = s16TempData[2]/30.f*GearRatio;
            motor[2].begin=true;motor[2].mode=position;
          #elif ID_SELF==MOTOR_0_3
            motor[0].valueSet.angle = s16TempData[0]/30.f*GearRatio;
            motor[0].begin=true;motor[0].mode=position;
            motor[3].valueSet.angle = s16TempData[3]/30.f*GearRatio;
            motor[3].begin=true;motor[3].mode=position;
          #endif
            answer_master(&rx_message);
        }
        #if ID_SELF==MOTOR_0_3
          if((rx_message.StdId==Elmo_Motor1_RX)||(rx_message.StdId==Elmo_Motor2_RX)||(rx_message.StdId==Elmo_Motor3_RX)||(rx_message.StdId==Elmo_Motor4_RX))
          {
            u8 ElmoID=rx_message.StdId-0x281;
            ELMOmotor[ElmoID].enable=1;
          }
        #endif
      #elif defined TryRobot

      #endif
    }
    #elif defined ActionMotor //执行电机报文处理
    if((rx_message.IDE == CAN_Id_Extended)&&(rx_message.RTR==CAN_RTR_Data))
    {
      if(rx_message.Data[0]==0x00)//广播帧
      {
        if((rx_message.Data[1]=='C')&&(rx_message.Data[2]=='W')&&(rx_message.Data[3]=='H')&&(rx_message.Data[4]=='U'))
          answer_master(&rx_message);
      }
      if(rx_message.Data[0]==0x03)
      {
      #ifdef PassRobot

      #elif defined TryRobot

      #endif
      }
    }
    #endif
  }
}
