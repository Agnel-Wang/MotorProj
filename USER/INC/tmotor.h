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

/*���������Χ*/
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

/****������****/
typedef struct{
    volatile float angle;//��ǰ�Ƕ�
    volatile float torque;//����
    volatile float speed;//�ٶ� (rpm)
    volatile float position;//�ۼ�����
}TmotorValue;

/****���״̬****/
typedef struct{
    bool arrived;//λ��ģʽ�µ���ָ��λ��
    bool zero;//Ѱ�����
    bool stuck;//�����ת
    bool timeout;//can���ķ�����ʱ
    bool overspeed;//����
    bool isSetZero;//�Ƿ����õ�ǰλ��Ϊ���
}TmotorStatus;

/****����ڲ�****/
typedef struct{
    float  RATIO;//���ٱ�
    float  GEARRATIO;//�ⲿ���ּ��ٱ�
}TmotorParam;

/****������Ʊ����ṹ��****/
typedef struct{
    bool isPosLimitON;//�Ƿ�����λ�ý������ƣ�(��ǰ�Ƕ�)
    float maxAngle;//�����ת�Ƕ�
    bool isPosSPLimitOn;//�Ƿ��λ��ģʽ��������
    double posSPlimit;//λ��ģʽ�µ��ٶ�����
    bool isRealseWhenStuck;//�Ƿ��תʱ�ͷŵ��
    s16 zeroSP;//Ѱ��ģʽ�µ��ٶ�
    u16 zeroCurrent;//Ѱ��ģʽ�µ�������
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
