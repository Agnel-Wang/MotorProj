#include "vesc.h"

VESCParam SUNNYSKY,VESC_U10;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESC_MOTOR VESCmotor[4];

void VESC_Set_Pos(u8 , float , u8 );

//电机参数初始化
void VESCInit(void)
{
  u8 id=0;
	{//电机内参
		SUNNYSKY.POLE_PAIRS=7;
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
	VESCmotor[id].instrinsic=SUNNYSKY;
	VESCmotor[id].enable=DISABLE;
	VESCmotor[id].begin=true;
	VESCmotor[id].mode=RPM;
	VESCmotor[id].valSet.current=10;
	VESCmotor[id].valSet.speed=-200;
	VESCmotor[id].valSet.position=0;
	VESCmotor[id].valSet.duty=1;
  PID_Init(&VESCmotor[id].PIDx, 0.5, 0.045, 0, 1.01, VESCmotor[id].valSet.position);
  
	/****1号电机初始化****/id=1;
	VESCmotor[id].instrinsic=VESC_U10;
	VESCmotor[id].enable=DISABLE;
	VESCmotor[id].begin=true;
	VESCmotor[id].mode=RPM;
	VESCmotor[id].valSet.current=10;
	VESCmotor[id].valSet.speed=-200;
	VESCmotor[id].valSet.position=0;
	VESCmotor[id].valSet.duty=1;
  PID_Init(&VESCmotor[id].PIDx, 0.5, 0.2, 0, 1, VESCmotor[id].valSet.position);

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

//位置模式--通过速度模式实现
void VESC_position_mode(u8 id)
{
  VESCmotor[id].PIDx.SetVal=VESCmotor[id].valSet.position;
  if(!VESCmotor[id].begin) 
    VESCmotor[id].PIDx.SetVal=VESCmotor[id].argum.lockPosition;
  if(VESCmotor[id].limit.isPosLimitOn) 
    PEAK(motor->PIDx.SetVal,VESCmotor[id].limit.maxPosition);
  VESCmotor[id].PIDx.CurVal=VESCmotor[id].valReal.position;
  PID_Operation(&VESCmotor[id].PIDx);
  if(motor->limit.isPosSPLimitOn) 
    PEAK(VESCmotor[id].PIDx.Udlt,motor->limit.posSPlimit);
  VESC_Set_Speed(id+1, VESCmotor[id].PIDx.Udlt * VESCmotor[id].instrinsic.POLE_PAIRS, 0);
}

//位置模式--通过VESC自带位置模式实现
void VESC_angle_mode(u8 id)
{
  if(ABS(VESCmotor[id].argum.difPosition)>1800)
  {
    //正转最大允许20000转速，反转最大允许10000转速
    VESCmotor[id].valSet.angle=VESCmotor[id].valReal.angle+SIG(VESCmotor[id].argum.difPosition)*120.0f;
    utils_norm_angle(&VESCmotor[id].valSet.angle);
    VESC_Set_Pos(id+1, VESCmotor[id].valSet.angle, 0);
  }
  else
  {
    float difpos=VESCmotor[id].argum.difPosition/1e1f;
    VESCmotor[id].valSet.angle=VESCmotor[id].valReal.angle+difpos;
    utils_norm_angle(&VESCmotor[id].valSet.angle);
    VESC_Set_Pos(id+1, VESCmotor[id].valSet.angle, 0);
  }
}

/**
  * @brief 设定VESC角度
  */
void VESC_Set_Pos(u8 controller_ID, float pos, u8 InConGrpFlag)
{
  int32_t send_index = 0;
  
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data, (int32_t)(pos * 1e6f), &send_index);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_POS << 8);		
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = send_index;				
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
	}
	Can2_Sendqueue.Rear=Rear2; 
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
