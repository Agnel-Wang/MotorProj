#include "vesc.h"

VESCParam SUNNYSKYinstrin,VESC_U10;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESC_MOTOR VESCmotor[4];

//电机参数初始化
void VESCInit(void)
{
  u8 id=0;
	{//电机内参
		SUNNYSKYinstrin.POLE_PAIRS=7;
    VESC_U10.POLE_PAIRS=21;
	}
	{//电机限制保护
    VESClimit.isPosLimitOn=false;
    VESClimit.maxPosition=30000;
    VESClimit.isPosSPLimitOn=true;
    VESClimit.posSPlimit=1000;
	}
	{//间值参数设置
		VESCargum.timeoutTicks=2000;//2000ms
    VESCargum.fistPos=true;
	}
	/****0号电机初始化****/id=0;
	VESCmotor[id].instrinsic=VESC_U10;
	VESCmotor[id].enable=DISABLE;
	VESCmotor[id].begin=true;
	VESCmotor[id].mode=RPM;
	VESCmotor[id].valSet.current=10;
	VESCmotor[id].valSet.speed=-200;
	VESCmotor[id].valSet.position=0;
	VESCmotor[id].valSet.duty=1;
  PID_Init(&VESCmotor[id].PIDx, 0.1, 0.1, 0, 0, VESCmotor[id].valSet.position);
  
	/****1号电机初始化****/id=1;
	VESCmotor[id].instrinsic=VESC_U10;
	VESCmotor[id].enable=DISABLE;
	VESCmotor[id].begin=true;
	VESCmotor[id].mode=RPM;
	VESCmotor[id].valSet.current=10;
	VESCmotor[id].valSet.speed=-200;
	VESCmotor[id].valSet.position=0;
	VESCmotor[id].valSet.duty=1;
  PID_Init(&VESCmotor[id].PIDx, 0.1, 0.1, 0, 0, VESCmotor[id].valSet.position);

	for(int i=0;i<4;i++)
	{
		VESCmotor[i].limit = VESClimit;
		VESCmotor[i].argum = VESCargum;
	}
}

//VESC状态计算
void VESC_caculate(VESC_MOTOR* motor)
{
  if(motor->status.isSetZero) {motor->status.isSetZero=false;motor->valReal.position=0;}/* 重置零点 */
  /* 反馈超时判断 */
  
}

//位置模式
void VESC_position_mode(VESC_MOTOR* motor)
{
  motor->PIDx.SetVal=motor->valSet.position;
  if(!motor->begin) motor->PIDx.SetVal=motor->argum.lockPosition;
  if(!motor->limit.isPosLimitOn) PEAK(motor->PIDx.SetVal,motor->limit.maxPosition);
  motor->PIDx.CurVal=motor->valReal.position;
  PID_Operation(&motor->PIDx);
  motor->valSet.speed=motor->PIDx.Udlt;
  if(motor->limit.isPosSPLimitOn) PEAK(motor->valSet.speed,motor->limit.posSPlimit);
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
