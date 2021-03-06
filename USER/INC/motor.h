#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f4xx.h"
#include "pid.h"
#include "stdbool.h"
#include "macro.h"
#include "can2.h"
#include "elmo.h"

#define DJ_MOTOR1_RX 0x201
#define DJ_MOTOR2_RX 0x202
#define DJ_MOTOR3_RX 0x203
#define DJ_MOTOR4_RX 0x204
#define DJ_MOTOR5_RX 0x205
#define DJ_MOTOR6_RX 0x206
#define DJ_MOTOR7_RX 0x207
#define DJ_MOTOR8_RX 0x208

extern bool motorSwitchOn;

/****电机模式****/
enum MOTOR_MODE { duty=0, current, RPM, brake, zero, position, vesc_postion_rpm, vesc_postion_I ,vesc_RPM_I};

/****电机限制保护结构体****/
typedef struct{
    bool isPosLimitON;//是否对最大位置进行限制，(轴前角度)
    float maxAngle;//最大旋转角度
    bool isPosSPLimitOn;//是否对位置模式进行限速
    double posSPlimit;//位置模式下的速度限制
    bool isRealseWhenStuck;//是否堵转时释放电机
    s16 zeroSP;//寻零模式下的速度
    u16 zeroCurrent;//寻零模式下的最大电流
}MotorLimit;

/****电机内参****/
typedef struct{
    u16 PULSE;//编码器线数
    u8  RATIO;//减速比
    u16 CURRENT_LIMIT;//最大输出电流限制
    double  GEARRATIO;//外部齿轮减速比
}MotorParam;

/****电机外参****/
typedef struct{
    vs16 angle;//轴前角度
    vs16 current;//电流
    volatile double speed;//速度 (rpm)
    vs32 pulse;//累计脉冲
    vs16 pulseRead;//反馈脉冲
}MotorValue;

/****电机状态****/
typedef struct{
    bool arrived;//位置模式下到达指定位置
    bool zero;//寻零完成
    bool stuck;//电机堵转
    bool timeout;//can报文反馈超时
    bool overspeed;//超速
    bool isSetZero;//是否设置当前位置为零点
}MotorStatus;

/****不需要关心的一些参数****/
typedef struct{
    u32 maxPulse;//最大旋转脉冲,[相关于最大旋转角度]
    vs32 lockPulse;//锁位置用当前位置记录，[相关于begin]
    s16 distance;//当前反馈脉冲与前次反馈脉冲差值，[相关于累计脉冲pulse]
    s32 difPulseSet;//当前累计脉冲与设定脉冲差值
    u16 zeroCnt;//寻零用计数值，[相关于寻零状态zero]
    vu16 stuckCnt;//堵转用计数值，[相关于堵转状态stuck]
    u8  timeoutCnt;//超时用计数值，[相关于超时状态timeout]
    u32 lastRxTim;//上次接收到报文的系统时间，[相关于超时状态timeout]
    u16 timeoutTicks;//判断超时用系统节拍数，[相关于超时状态timeout]
    u16 RM6025cnt;
}MotorArgum;

/****DJ电机总结构体****/
typedef struct{
    bool enable;//电机使能
    bool begin;//电机启动
    u8 mode;//电机模式
    MotorValue valueSet,valueReal,valuePrv;//外参设定值，实际值，前次值
    MotorStatus status;//电机状态
    PID_setTypeDef PIDx,PIDs;//PID参数
    MotorLimit limit;//限制保护
    MotorArgum argum;//计算所用间值
    MotorParam intrinsic;//电机内参
}DJmotor;

extern DJmotor motor[8];
extern MotorParam M2006instrin,M3508instrin,RM6025instrin;

void Motor_Init(void);
void speed_mode(s16 id);
void position_mode(s16 id);
void zero_mode(s16 id);
void pulse_caculate(void);

#endif
