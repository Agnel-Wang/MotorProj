#include "vesc.h"

VESCParam SUNNYSKYinstrin;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESC_MOTOR VESCmotor[4];

//电机参数初始化
void VESCInit(void)
{
	{//电机内参
		SUNNYSKYinstrin.POLE_PAIRS=7;
	}
	{//电机限制保护

	}
	{//间值参数设置
		VESCargum.timeoutTicks=2000;//2000ms
	}
	/****0号电机初始化****/
	VESCmotor[0].instrinsic=SUNNYSKYinstrin;
	VESCmotor[0].enable=false;
	VESCmotor[0].begin=false;
	VESCmotor[0].mode=RPM;
	VESCmotor[0].valSet.current=10;
	VESCmotor[0].valSet.speed=1000;
	VESCmotor[0].valSet.position=0;
	VESCmotor[0].valSet.duty=1;

	/****1号电机初始化****/
	VESCmotor[1].instrinsic=SUNNYSKYinstrin;
	VESCmotor[1].enable=false;
	VESCmotor[1].begin=false;
	VESCmotor[1].mode=RPM;
	VESCmotor[1].valSet.current=10;
	VESCmotor[1].valSet.speed=1000;
	VESCmotor[1].valSet.position=0;
	VESCmotor[1].valSet.duty=1;

	/****2号电机初始化****/
	VESCmotor[2].instrinsic=SUNNYSKYinstrin;
	VESCmotor[2].enable=false;
	VESCmotor[2].begin=false;
	VESCmotor[2].mode=RPM;
	VESCmotor[2].valSet.current=10;
	VESCmotor[2].valSet.speed=1000;
	VESCmotor[2].valSet.position=0;
	VESCmotor[2].valSet.duty=1;

	/****3号电机初始化****/
	VESCmotor[3].instrinsic=SUNNYSKYinstrin;
	VESCmotor[3].enable=false;
	VESCmotor[3].begin=false;
	VESCmotor[3].mode=RPM;
	VESCmotor[3].valSet.current=10;
	VESCmotor[3].valSet.speed=1000;
	VESCmotor[3].valSet.position=0;
	VESCmotor[3].valSet.duty=1;

	for(int i=0;i<4;i++)
	{
		VESCmotor[i].limit = VESClimit;
		VESCmotor[i].argum = VESCargum;
	}
}

/** 
  * @brief 设定VESC占空比
  */
void VESC_Set_Duty_Cycle(u8 controller_ID,float duty_cycle,u8 InConGrpFlag)
{
	int32_t send_index = 0;
	
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, (int32_t)(duty_cycle * 100000), &send_index);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_DUTY << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;				
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;	
}
/** 
	* @brief 设定VESC转速
	*/
void VESC_Set_Speed(u8 controller_ID,s32 speed,u8 InConGrpFlag)
{
	int32_t send_index = 0;
	
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, speed, &send_index);	
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID =0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_RPM << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;				
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;	
}
/** 
	* @brief 设定VESC电流
	*/
void VESC_Set_Current(u8 controller_ID,float current,u8 InConGrpFlag)
{
	int32_t send_index = 0;
	
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, (int32_t)(current * 1000), &send_index);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;				
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;	
}
/** 
	* @brief 设定VESC刹车电流
	*/
void VESC_Set_Brake_Current(u8 controller_ID,float brake_current,u8 InConGrpFlag)
{
	int32_t send_index = 0;
	
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, (int32_t)(brake_current * 1000), &send_index);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID =0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;	
}
/** 
	* @brief 设定VESC锁电机电流
	*/
void VESC_Set_Handbrake_Current(u8 controller_ID,float handbrake_current,u8 InConGrpFlag)
{
	int32_t send_index = 0;
	
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, (int32_t)(handbrake_current * 1000), &send_index);
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID =0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;			
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2;	
}
