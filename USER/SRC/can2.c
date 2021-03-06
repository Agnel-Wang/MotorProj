#include "can2.h"

void CAN2_Configuration() 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	/* SET CAN pin: RX */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
 	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	/* CAN RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	/* CAN cell init */
  	CAN_InitStructure.CAN_TTCM=DISABLE;  									//非时间触发通道模式
	CAN_InitStructure.CAN_ABOM=DISABLE;  									//软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
	CAN_InitStructure.CAN_AWUM=DISABLE;  									//睡眠模式由软件唤醒
	CAN_InitStructure.CAN_NART=DISABLE;  									//禁止报文自动发送，即只发送一次，无论结果如何
	CAN_InitStructure.CAN_RFLM=DISABLE;  									//报文不锁定，新的覆盖旧的
	CAN_InitStructure.CAN_TXFP=DISABLE;  									//发送FIFO的优先级由标识符决定
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;								//CAN硬件工作在正常模式

	/* Seting BaudRate */	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;									//重新同步跳跃宽度为一个时间单位
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 									//时间段1占用8个时间单位
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;									//时间段2占用7个时间单位
	CAN_InitStructure.CAN_Prescaler=3; 										//分频系数（Fdiv）
	CAN_Init(CAN2, &CAN_InitStructure); 									//初始化CAN1
	
	//3508或2006过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =DJ_MOTOR1_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow =DJ_MOTOR2_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =DJ_MOTOR3_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =DJ_MOTOR4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 15;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =DJ_MOTOR5_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow =DJ_MOTOR6_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =DJ_MOTOR7_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =DJ_MOTOR8_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//ELMO过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 16;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor1_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor2_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor3_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 17;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor5_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor6_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor7_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor8_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
		
	CAN_FilterInitStructure.CAN_FilterNumber = 18;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor1_error<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor2_error<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor3_error << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor4_error << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
		
	CAN_FilterInitStructure.CAN_FilterNumber = 19;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor5_error<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor6_error<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor7_error << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor8_error << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//VESC过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 20;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =((((uint32_t)CAN_PACKET_STATUS << 8)<<3)&0xffff0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)CAN_PACKET_STATUS << 8)<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0xffffff00<<3)>>16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(0xffffff00<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

  //EPOS过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 21;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =EPOS_Motor1_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = EPOS_Motor2_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =EPOS_Motor4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =EPOS_Motor4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
  
  //CAN紧急报错
  CAN_FilterInitStructure.CAN_FilterNumber = 22;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =0x00<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x00<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =CAN_EMERGENCY3 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =CAN_EMERGENCY4 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FilterInitStructure.CAN_FilterNumber = 23;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =CAN_EMERGENCY5<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_EMERGENCY6<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =CAN_EMERGENCY7 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =CAN_EMERGENCY8 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
#ifdef USE_DJ
	CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE);
#endif
#if defined USE_ELMO | defined USE_VESC | defined USE_EPOS | USE_TMOTOR
	CAN_ITConfig(CAN2,CAN_IT_FMP1, ENABLE);
#endif
}

/*
 * RX0 用于接收大疆电机报文
 */
void CAN2_RX0_IRQHandler(void) 
{
  if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET) 
  {
    CanRxMsg rx_message;
    CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
    CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);

    if ((rx_message.StdId >= 0x201) && (rx_message.StdId <= 0x208) && (rx_message.RTR == CAN_RTR_Data)) //DJ电机报文处理
    {
        u8 id = rx_message.StdId - 0x201;
        ChangeData(&rx_message.Data[0],&rx_message.Data[1]);
        ChangeData(&rx_message.Data[2],&rx_message.Data[3]);
        ChangeData(&rx_message.Data[4],&rx_message.Data[5]);
      if(motor[id].intrinsic.CURRENT_LIMIT == RM6025instrin.CURRENT_LIMIT)//判断是否为RM6025反馈
        {//RM6025只有脉冲和电流反馈
          DecodeS16Data(&motor[id].valueReal.pulseRead,&rx_message.Data[0]);
          DecodeS16Data(&motor[id].valueReal.current,&rx_message.Data[2]);
        }
        else
        {
          DecodeS16Data(&motor[id].valueReal.pulseRead,&rx_message.Data[0]);
          DecodeS16Data(&motor[id].valueReal.speed,&rx_message.Data[2]);
          DecodeS16Data(&motor[id].valueReal.current,&rx_message.Data[4]);
        }
        
        motor[id].argum.distance=motor[id].valueReal.pulseRead-motor[id].valuePrv.pulseRead;
        motor[id].valuePrv=motor[id].valueReal;
        if(ABS(motor[id].argum.distance)>4000) motor[id].argum.distance -= SIG(motor[id].argum.distance)*motor[id].intrinsic.PULSE;
        motor[id].valueReal.pulse+=motor[id].argum.distance;//累计脉冲计算
        motor[id].argum.difPulseSet=motor[id].valueReal.pulse-motor[id].valueSet.pulse;//更新误差
        
        motor[id].valueReal.angle=motor[id].valueReal.pulse*360.f/(double)(motor[id].intrinsic.RATIO)/motor[id].intrinsic.PULSE/motor[id].intrinsic.GEARRATIO;
        motor[id].argum.lastRxTim=OSTimeGet();
    }
  }
}

/****接收到报文反馈后控制表处理****/
static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
{
	Can_MesgSentList->ReceiveNumber+=1;
	Can_MesgSentList->TimeOut=0;
	Can_MesgSentList->SendSem--;
	Can_MesgSentList->SentQueue.Front=(Can_MesgSentList->SentQueue.Front+1)%CAN_QUEUESIZE;
}

/*
 * RX1 用于接收ELMO及VESC报文
 */
void CAN2_RX1_IRQHandler(void) 
{
  if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET) 
  {
    CanRxMsg rx_message;
    CAN_Receive(CAN2, CAN_FIFO1, &rx_message);
    CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
    CAN_ClearFlag(CAN2, CAN_IT_FMP1);
#ifdef USE_ELMO
	if(((rx_message.StdId >= 0x281) && (rx_message.StdId <= 0x288))&&(rx_message.RTR == CAN_RTR_Data))//ELMO报文处理
	{
		u8 id = rx_message.StdId-0x281;
    	ELMOmotor[id].argum.lastRxTim=OSTimeGet();
		if(	(rx_message.Data[0]=='A'&&rx_message.Data[1]=='C'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='B'&&rx_message.Data[1]=='G'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='J'&&rx_message.Data[1]=='V'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='P'&&rx_message.Data[1]=='R'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='S'&&rx_message.Data[1]=='D'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='S'&&rx_message.Data[1]=='T'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='T'&&rx_message.Data[1]=='C'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='U'&&rx_message.Data[1]=='M'&&(rx_message.Data[3]&BIT6)!=1) )
			Elmo_Feedback_Deel(&Can2_MesgSentList[id]);
		if(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&(rx_message.Data[3]&BIT6)!=1)
		{
      if(ABS(rx_message.Data[4])<0x02)//20.09.26 暂时先这样根据大小判断一下
			ELMOmotor[id].enable=rx_message.Data[4];
		}
		if(rx_message.Data[0]=='V'&&rx_message.Data[1]=='X'&&(rx_message.Data[3]&BIT6)!=1)
		{
			DecodeS32Data(&ELMOmotor[id].valReal.speed,&rx_message.Data[4]);
			ELMOmotor[id].valReal.speed/=ELMOmotor[id].intrinsic.PULSE/60;
		}
		if(rx_message.Data[0]=='P'&&rx_message.Data[1]=='X'&&(rx_message.Data[3]&BIT6)!=1)
		{
      s32 pulse;
			DecodeS32Data(&pulse,&rx_message.Data[4]);
      ELMOmotor[id].valReal.angle=pulse*(360.f/ELMOmotor[id].intrinsic.RATIO)/ELMOmotor[id].intrinsic.PULSE;
		}
		if(rx_message.Data[0]=='I'&&rx_message.Data[1]=='Q'&&(rx_message.Data[3]&BIT6)!=1)
		{
			DecodeS32Data(&ELMOmotor[id].valReal.current,&rx_message.Data[4]);
		}
		ELMOmotor[id].argum.lastRxTim=OSTimeGet();
	}
	if(((rx_message.StdId >= 0x81) && (rx_message.StdId <= 0x88))&&(rx_message.RTR == CAN_RTR_Data))//ELMO错误反馈
  {
    u8 id = rx_message.StdId-0x81;
    insertError(error.head, ELMOERROR|((id+1)<<4)|EMERGENCY);
  }
#endif
#ifdef USE_VESC
	if((rx_message.IDE == CAN_ID_EXT)&&(rx_message.RTR == CAN_RTR_Data))
	{
		int32_t ind=0;
    u8 id = rx_message.ExtId&0xff-1;
		if((rx_message.ExtId>>8)==CAN_PACKET_STATUS)
		{
			VESCmotor[id].valReal.speed=get_s32_from_buffer(rx_message.Data,&ind)/VESCmotor[id].instrinsic.POLE_PAIRS;
			VESCmotor[id].valReal.current=buffer_16_to_float(rx_message.Data,1e3,&ind);
			VESCmotor[id].valReal.angle=buffer_16_to_float(rx_message.Data,1e1,&ind);
      //位置计算
      ChangeData(&rx_message.Data[6],&rx_message.Data[7]);
      DecodeU16Data(&VESCmotor[id].argum.angleNow,&rx_message.Data[6]);
      VESCmotor[id].argum.distance=VESCmotor[id].argum.angleNow-VESCmotor[id].argum.anglePrv;
      if(VESCmotor[id].argum.fistPos) {VESCmotor[id].argum.fistPos=false;VESCmotor[id].argum.distance=0;}
      VESCmotor[id].argum.anglePrv=VESCmotor[id].argum.angleNow;
      if(ABS(VESCmotor[id].argum.distance)>1800) VESCmotor[id].argum.distance -= SIG(VESCmotor[id].argum.distance)*3600;
      VESCmotor[id].valReal.position += VESCmotor[id].argum.distance;
      
      //锁点记录
      if(VESCmotor[id].begin) 
      {
        VESCmotor[id].argum.lockAngle=VESCmotor[id].valReal.angle;
        VESCmotor[id].argum.lockPosition=VESCmotor[id].valReal.position;
      }
		}
    VESCmotor[id].argum.lastRxTim=OSTimeGet();
	}
#endif
#ifdef USE_EPOS
    if((rx_message.IDE == CAN_ID_EXT)&&(rx_message.RTR == CAN_RTR_Data))
    {
      u8 id = rx_message.StdId-0x581;
      if(rx_message.Data[0]==0x4B&&rx_message.Data[1]==0x41&&rx_message.Data[2]==0x60)
      {
        DecodeS32Data(&EPOSmotor[id].status.statusword,&rx_message.Data[4]);
      }
    }
#endif
#if USE_TMOTOR
  if(rx_message.StdId==0x00)
  {
    Rcv_motor(rx_message, &tmotor[rx_message.Data[0] - 1]);
  }
#endif
  }
}

static CanTxMsg DJ_tx_message;
/****DJ电机电流输入****/
void currentInput(u8 id)
{
  PEAK(motor[id].valueSet.current,motor[id].intrinsic.CURRENT_LIMIT);
  if(!motor[id].enable) motor[id].valueSet.current=0;
  if (id < 4) DJ_tx_message.StdId = 0x200;
  else DJ_tx_message.StdId = 0x1FF;
  DJ_tx_message.RTR = CAN_RTR_Data;
  DJ_tx_message.IDE = CAN_Id_Standard;
  DJ_tx_message.DLC = 8;
  u8 temp=2*(id&0x0B);
  EncodeS16Data(&motor[id].valueSet.current,&DJ_tx_message.Data[temp]);
  ChangeData(&DJ_tx_message.Data[temp],&DJ_tx_message.Data[temp+1]);
  if((id==3)||(id==7)) CAN_Transmit(CAN2, &DJ_tx_message);
}

/****电磁阀控制****/
void valveCtrl(u8 ID, bool status)
{
	CanTxMsg tx_message;
	tx_message.ExtId = 0x00030101;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = 3;
	tx_message.Data[0] = 'V';
	tx_message.Data[1] = 'L';
	tx_message.Data[2] = status;
	tx_message.Data[3] = ID;
	CAN_Transmit(CAN2,&tx_message);
	flag.valve[1] = status;
}
