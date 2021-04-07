#include "elmo.h"

ELMOParam Flat90, U10, EC_4P_22, EC_4P_30;
ELMILimit ELMOlimit;
ELMOArgum ELMOargum;
ELMO_Motor ELMOmotor[5];

/*ELMO电机参数初始化*/
void ELMO_Motor_Init(void)
{
	{//电机内参
		Flat90.PULSE=16384;		Flat90.RATIO=1;
		U10.PULSE=16384;		U10.RATIO=1;
    	EC_4P_22.PULSE=4000;  	EC_4P_22.RATIO=3.7f;//MAXON Gear 166930
		EC_4P_30.PULSE=4000;	EC_4P_30.RATIO=169.f/9;
	}
	{//电机限制保护
		ELMOlimit.isPosLimitON=false;
		ELMOlimit.maxAngle=720.f;
		ELMOlimit.isPosSPLimitOn=false;
		ELMOlimit.posSPlimit=1000;
		ELMOlimit.isRealseWhenStuck=false;
		ELMOlimit.zeroSP=500;
		ELMOlimit.zeroCurrent=1000;
	}
	{//电机其他参数设置
		ELMOargum.timeoutTicks = 2000;//2000ms
	}
	/****0号电机初始化****/
	ELMOmotor[0].intrinsic=EC_4P_30;
	ELMOmotor[0].enable=DISABLE;
	ELMOmotor[0].mode=position;
  ELMOmotor[0].valSet.angle=0;
  ELMOmotor[0].valSet.speed=100;
  ELMOmotor[0].valSet.current=100;

	for(int i=0;i<5;i++)
	{
		ELMOmotor[i].limit=ELMOlimit;
		ELMOmotor[i].argum=ELMOargum;
  }
}

/*ELMO初始化*/
void ELMO_Init(CAN_TypeDef* CANx)
{
	CanTxMsg tx_message;
	tx_message.StdId=0x000;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.DLC=8;
	tx_message.Data[0]=0x01;
	tx_message.Data[1]=0x00;
	tx_message.Data[2]=0x00;
	tx_message.Data[3]=0x00;
	tx_message.Data[4]=0x00;
	tx_message.Data[5]=0x00;
	tx_message.Data[6]=0x00;
	tx_message.Data[7]=0x00;
	CAN_Transmit(CANx,&tx_message);
}

/*设置加速度*/
void AC(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag = va_arg(param, int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)//没用QueueFull函数，宏定义总比函数快
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*开始运动*/
void BG(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='B';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='G';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}

/*设置减速度*/
void DC(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*设置持续速度*/
void JV(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='J';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='V';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			s32 data_set;
      data_set=data*ELMOmotor[ID-1].intrinsic.PULSE/60;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='J';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='V';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*获取有功电流*/
void IQ(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='I';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='Q';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x80;//float
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}

/*电机启动/禁用*/
void MO(u32 ID,u8 cmd,...)
{
	va_list ap;
	int InConGrpFlag;
	s32 data;
	va_start(ap,cmd);
	InConGrpFlag=va_arg(ap,int);
	data=va_arg(ap,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='O';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x40;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='O';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=data;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0;
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(ap);
}

void MO_CAN1(u8 ID, u8 cmd, u8 status)
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.StdId = 0x300+ID;
	if(cmd==GetData)
	{
		tx_message.DLC = 4;
		tx_message.Data[0]='M';
		tx_message.Data[1]='O';
		tx_message.Data[2]=0;
		tx_message.Data[3]=0x00;
	}
	else if(cmd==SetData)
	{
		tx_message.DLC = 8;
		tx_message.Data[0]='M';
		tx_message.Data[1]='O';
		tx_message.Data[2]=0;
		tx_message.Data[3]=0;
		tx_message.Data[4]=status;
		tx_message.Data[5]=0;
		tx_message.Data[6]=0;
		tx_message.Data[7]=0;
	}
	CAN_Transmit(CAN1, &tx_message);
}
/*绝对位置*/
void PA(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag = 0;
	s32 data = 0;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
      s32 data_set;
      data_set=data*ELMOmotor[ID-1].intrinsic.PULSE*(ELMOmotor[ID-1].intrinsic.RATIO/360.f);
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*获取位置误差*/
void PE(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='E';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}

/*获得相对位置*/
void PR(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='R';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}

/*查询电机主位置*/
void PX(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*停止时的减速度（计数/秒2）*/
void SD(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag = va_arg(param,int);
	data=va_arg(param, s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*设置PTP模式速度*/
void SP(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x40;
		}
		if(cmd==SetData)
		{
      s32 data_set;
      data_set=data*ELMOmotor[ID-1].intrinsic.PULSE/60;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*停止运动*/
void ST(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='T';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}

/*设定电流*/
void TC(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==SetData)
		{
			s32 data_set;
			data_set=data/100.f;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='T';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&data_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*运动模式*/
void UM(u32 ID,u8 cmd,...)
{
	va_list param;
	int InConGrpFlag;
	s32 data;
	va_start(param,cmd);
	InConGrpFlag=va_arg(param,int);
	data=va_arg(param,s32);
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='U';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='U';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=data;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0;
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
	va_end(param);
}

/*查询电机转速*/
void VX(u32 ID,u8 InConGrpFlag)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+ID;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='V';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;
}
