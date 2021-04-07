#ifndef _TMOTOR_H
#define _TMOTOR_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stdbool.h"
#include "motor.h"

#define TMOTOR_FR 4
#define TMOTOR_FL 3
#define TMOTOR_RR 7
#define TMOTOR_RL 0

#define CMD_MOTOR_MODE      0x01
#define CMD_RESET_MODE      0x02
#define CMD_ZERO_POSITION   0x03

/*电机参数范围*/
#define P_MIN -95.5f    // Radians
#define P_MAX 95.5f        
#define V_MIN -30.0f    // Rad/s
#define V_MAX 30.0f
#define KP_MIN 0.0f     // N-m/rad
#define KP_MAX 500.0f
#define KD_MIN 0.0f     // N-m/rad/s
#define KD_MAX 5.0f
#define T_MIN -18.0f
#define T_MAX 18.0f

#define PI (3.1415926)

/****电机外参****/
typedef struct{
    volatile float angle;//轴前角度
    volatile float torque;//电流
    volatile float speed;//速度 (rpm)
    volatile float position;//累计脉冲
}TmotorValue;

/****电机状态****/
typedef struct{
    bool arrived;//位置模式下到达指定位置
    bool zero;//寻零完成
    bool stuck;//电机堵转
    bool timeout;//can报文反馈超时
    bool overspeed;//超速
    bool isSetZero;//是否设置当前位置为零点
}TmotorStatus;

/****电机内参****/
typedef struct{
    float  RATIO;//减速比
    float  GEARRATIO;//外部齿轮减速比
}TmotorParam;

/****电机限制保护结构体****/
typedef struct{
    bool isPosLimitON;//是否对最大位置进行限制，(轴前角度)
    float maxAngle;//最大旋转角度
    bool isPosSPLimitOn;//是否对位置模式进行限速
    double posSPlimit;//位置模式下的速度限制
    bool isRealseWhenStuck;//是否堵转时释放电机
    s16 zeroSP;//寻零模式下的速度
    u16 zeroCurrent;//寻零模式下的最大电流
}TmotorLimit;

typedef struct{
    bool enable;
    bool begin;
    u8 mode;
    TmotorValue valSet, valReal, valPrv;
    TmotorStatus status;
    float kp, kd;
    TmotorLimit limit;
    TmotorParam intrinsic;
}TmotorTypedef;

extern TmotorTypedef tmotor[8];

void Tmotorinit();
void TmotorCaculate(u8 id, TmotorTypedef* motor);
void CanTransmit(u8 id, u8 *buf, uint8_t len);
void Tmotor_ControlCmd(u8 id, u8 cmd);
void Tmotor_ControlPara(u8 id, float f_p, float f_v, float f_kp, float f_kd, float f_t);
void Rcv_motor(CanRxMsg rx_message, TmotorTypedef* tmotor);

#endif
