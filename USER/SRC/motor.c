#include "motor.h"

MotorParam M2006instrin,M3508instrin;
MotorLimit Motorlimit;
MotorArgum Motorargum;
DJmotor motor[8];

//电机参数初始化
void Motor_Init(void)
{
    {//电机内参
        M2006instrin.PULSE=8192;            M3508instrin.PULSE=8192;
        M2006instrin.RATIO=36;              M3508instrin.RATIO=19;
        M2006instrin.CURRENT_LIMIT=9000;    M3508instrin.CURRENT_LIMIT=14745;
    }
    {//电机限制保护设置
      #ifdef SteeringMotor
        Motorlimit.isPosLimitON=true;
        Motorlimit.maxAngle= 187*GearRatio;//轮毂最多旋转±0.5圈多一丢丢
      #else
        Motorlimit.isPosLimitON=false;
        Motorlimit.maxAngle= 500;//轮毂最多旋转±0.5圈多一丢丢
      #endif
        Motorlimit.isPosSPLimitOn=true;
        Motorlimit.posSPlimit=1000;
        Motorlimit.isRealseWhenStuck=true;    
        Motorlimit.zeroSP=1000;
        Motorlimit.zeroCurrent=2000;
    }
    {//电机其他参数设置
        Motorargum.timeoutTicks = 2000;//2000ms
    }
#ifdef PassRobot
    #ifdef SteeringMotor
      #if ID_SELF==MOTOR_0_3
    /****0号电机初始化****/
    motor[0].intrinsic=M3508instrin;
    motor[0].enable=ENABLE;
    motor[0].begin=false;
    motor[0].mode=position;
    motor[0].valueSet.angle=0;
    motor[0].valueSet.speed=100;
    motor[0].valueSet.current=100;
    PID_Init(&motor[0].PIDx, 5, 0.2, 0, 0.4, motor[0].valueSet.pulse);
    PID_Init(&motor[0].PIDs, 8, 0.3, 0, 1, motor[0].valueSet.speed);
    motor[0].limit=Motorlimit;

     /****3号电机初始化****/
    motor[3].intrinsic=M2006instrin;
    motor[3].enable=DISABLE;
    motor[3].begin=false;
    motor[3].mode=position;
    motor[3].valueSet.angle=0;
    motor[3].valueSet.speed=100;
    motor[3].valueSet.current=100;
    PID_Init(&motor[3].PIDx, 5, 0.2, 0, 0.4, motor[3].valueSet.pulse);
    PID_Init(&motor[3].PIDs, 8, 0.3, 0, 1, motor[3].valueSet.speed);
    motor[3].limit=Motorlimit;
      #elif ID_SELF==MOTOR_1_2
     /****1号电机初始化****/
    motor[1].intrinsic=M2006instrin;
    motor[1].enable=ENABLE;
    motor[1].begin=false;
    motor[1].mode=position;
    motor[1].valueSet.angle=0;
    motor[1].valueSet.speed=100;
    motor[1].valueSet.current=100;
    PID_Init(&motor[1].PIDx, 5, 0.2, 0, 0.4, motor[1].valueSet.pulse);
    PID_Init(&motor[1].PIDs, 8, 0.3, 0, 1, motor[1].valueSet.speed);
    motor[1].limit=Motorlimit;

     /****2号电机初始化****/
    motor[2].intrinsic=M2006instrin;
    motor[2].enable=ENABLE;
    motor[2].begin=false;
    motor[2].mode=position;
    motor[2].valueSet.angle=0;
    motor[2].valueSet.speed=100;
    motor[2].valueSet.current=100;
    PID_Init(&motor[2].PIDx, 5, 0.2, 0, 0.4, motor[2].valueSet.pulse);
    PID_Init(&motor[2].PIDs, 8, 0.3, 0, 1, motor[2].valueSet.speed);
    motor[2].limit=Motorlimit;
      #endif
    #elif defined ActionMotor

     /****4号电机初始化****/
    motor[4].intrinsic=M2006instrin;
    motor[4].enable=DISABLE;
    motor[4].begin=false;
    motor[4].mode=position;
    motor[4].valueSet.angle=0;
    motor[4].valueSet.speed=100;
    motor[4].valueSet.current=100;
    PID_Init(&motor[4].PIDx, 5, 0.2, 0, 0.4, motor[4].valueSet.pulse);
    PID_Init(&motor[4].PIDs, 8, 0.3, 0, 1, motor[4].valueSet.speed);
    motor[4].limit=Motorlimit;
    
     /****5号电机初始化****/
    motor[5].intrinsic=M2006instrin;
    motor[5].enable=DISABLE;
    motor[5].begin=false;
    motor[5].mode=position;
    motor[5].valueSet.angle=0;
    motor[5].valueSet.speed=100;
    motor[5].valueSet.current=100;
    PID_Init(&motor[5].PIDx, 5, 0.2, 0, 0.4, motor[5].valueSet.pulse);
    PID_Init(&motor[5].PIDs, 8, 0.3, 0, 1, motor[5].valueSet.speed);
    motor[5].limit=Motorlimit;

     /****6号电机初始化****/
    motor[6].intrinsic=M2006instrin;
    motor[6].enable=DISABLE;
    motor[6].begin=false;
    motor[6].mode=position;
    motor[6].valueSet.angle=0;
    motor[6].valueSet.speed=100;
    motor[6].valueSet.current=100;
    PID_Init(&motor[6].PIDx, 5, 0.2, 0, 0.4, motor[6].valueSet.pulse);
    PID_Init(&motor[6].PIDs, 8, 0.3, 0, 1, motor[6].valueSet.speed);
    motor[6].limit=Motorlimit;

     /****7号电机初始化****/
    motor[7].intrinsic=M2006instrin;
    motor[7].enable=DISABLE;
    motor[7].begin=false;
    motor[7].mode=position;
    motor[7].valueSet.angle=0;
    motor[7].valueSet.speed=100;
    motor[7].valueSet.current=100;
    PID_Init(&motor[7].PIDx, 5, 0.2, 0, 0.4, motor[7].valueSet.pulse);
    PID_Init(&motor[7].PIDs, 8, 0.3, 0, 1, motor[7].valueSet.speed);
    motor[7].limit=Motorlimit;
    
    #endif
#elif defined TryRobot
    #ifdef SteeringMotor

    #elif defined ActionMotor

    #endif
#endif
    for(int i=0;i<8;i++)
    {
        motor[i].argum=Motorargum;
        motor[i].argum.maxPulse=motor[i].limit.maxAngle*motor[i].intrinsic.RATIO*motor[i].intrinsic.PULSE/360.f;
        motor[i].valueSet.pulse=motor[i].valueSet.angle*motor[i].intrinsic.RATIO*motor[i].intrinsic.PULSE/360.f;
    }
}

/****设置当前位置为零点****/
static void setZero(DJmotor* motor)
{
    motor->status.isSetZero=false;
    motor->valueReal.pulse=0;
    motor->valueReal.angle=0;
}

/****速度模式****/
void speed_mode(s16 id)
{
    motor[id].PIDs.SetVal=motor[id].PIDs.uKS_Coe*motor[id].valueSet.speed;
    motor[id].PIDs.CurVal=motor[id].valueReal.speed;
    PID_Operation(&motor[id].PIDs);
    motor[id].valueSet.current+=motor[id].PIDs.Udlt;
}

/****位置模式****/
void position_mode(s16 id)
{
    motor[id].valueSet.pulse=motor[id].valueSet.angle*motor[id].intrinsic.RATIO*motor[id].intrinsic.PULSE/360.f;
    motor[id].PIDx.SetVal=motor[id].valueSet.pulse;
    if(!motor[id].begin) motor[id].PIDx.SetVal = motor[id].argum.lockPulse;//如果为锁电机状态，位置设定屏蔽，改为锁位置
    if(motor[id].limit.isPosLimitON) PEAK(motor[id].PIDx.SetVal,motor[id].argum.maxPulse);
    motor[id].PIDx.CurVal=motor[id].valueReal.pulse;
    PID_Operation(&motor[id].PIDx);
    motor[id].PIDs.SetVal=motor[id].PIDx.uKS_Coe*(motor[id].PIDs.SetVal+motor[id].PIDx.Udlt);
    if(motor[id].limit.isPosSPLimitOn)  PEAK(motor[id].PIDs.SetVal,motor[id].limit.posSPlimit);
    motor[id].PIDs.CurVal=motor[id].valueReal.speed;
    PID_Operation(&motor[id].PIDs);
    motor[id].valueSet.current+=motor[id].PIDs.Udlt;
    if(ABS(motor[id].argum.difPulseSet)<60) motor[id].status.arrived=true;//到达指定位置
    else motor[id].status.arrived=false;
}

/****寻零模式****/
void zero_mode(s16 id)
{
    motor[id].PIDs.SetVal=motor[id].limit.zeroSP;
    motor[id].PIDs.CurVal=motor[id].valueReal.speed;
    PID_Operation(&motor[id].PIDs);
    motor[id].valueSet.current+=motor[id].PIDs.Udlt;
    PEAK(motor[id].valueSet.current,Motorlimit.zeroCurrent);
    if((motor[id].argum.distance==0)&&motor[id].valueReal.pulse>5000)    motor[id].argum.zeroCnt++;
    else    motor[id].argum.zeroCnt=0;
    if(motor[id].argum.zeroCnt>250)//寻零完成，初始化零点，转换为位置模式锁零点
    {
        motor[id].argum.zeroCnt=0; motor[id].valueReal.pulse=0;
        motor[id].mode=position; motor[id].status.zero=1;
    }
}

/****位置计算****/
void pulse_caculate(void)
{
	for(int id=0;id<8;id++)
	{
		motor[id].argum.distance=motor[id].valueReal.pulseRead-motor[id].valuePrv.pulseRead;
        motor[id].valuePrv=motor[id].valueReal;
        if(ABS(motor[id].argum.distance)>4000) motor[id].argum.distance -= SIG(motor[id].argum.distance)*motor[id].intrinsic.PULSE;
		motor[id].valueReal.pulse+=motor[id].argum.distance;//累计脉冲计算
        motor[id].argum.difPulseSet=motor[id].valueSet.pulse-motor[id].valueReal.pulse;//更新误差

        /* 判断是否堵转 */
        if(motor[id].enable&&(ABS(motor[id].valueSet.current)>(motor[id].intrinsic.CURRENT_LIMIT-1)))//电流时刻保持限值
        {
            motor[id].argum.stuckCnt++;
            if(motor[id].argum.stuckCnt>1000)//判断时间为1S
            {
                motor[id].status.stuck=true; 
                if(motor[id].limit.isRealseWhenStuck) motor[id].enable=0;
            }
            else motor[id].status.stuck=false;
        }
        else motor[id].argum.stuckCnt=0;
        /* 锁点位置记录 */
        if(motor[id].begin) motor[id].argum.lockPulse=motor[id].valueReal.pulse;
        /* 判断是否需要重置零点 */
        if(motor[id].status.isSetZero) setZero(&motor[id]);
        /* 判断是否过速 */
        //TODO:
	}
}
