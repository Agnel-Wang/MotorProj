#ifndef _MACRO_H_
#define _MACRO_H_

#include "stdbool.h"
#include "stm32f4xx.h"

 /**
 * @description: set specific rotor to control for code selecting
 */
//#define PassRobot
#define TryRobot
//#define SteeringMotor
#define ActionMotor

//USE_SCOPE      是否调用示波器

/* selet progrom functional unit
 * VER						流程号是否开启
 * USE_VECS					是否使用VESC
 * USE_ELMO					是否使用ELMO
 * USE_DJ					是否使用大疆电机
 */
#ifdef PassRobot
	#ifdef SteeringMotor
		#define USE_ELMO
		#define USE_DJ
	#elif defined ActionMotor

	#endif
#elif defined TryRobot
	#ifdef SteeringMotor
		//nothing
	#elif defined ActionMotor
		#define USE_ELMO
	#endif
#endif

#ifdef SteeringMotor
		#define BROADCAST_ID 0x00010000
	#ifdef PassRobot
		#define MOTOR_0_3 0x305
		#define MOTOR_1_2 0x306
		#define ID_SELF MOTOR_0_3
		#define GearRatio (104.0f/17.0f)
	#elif defined TryRobot

	#endif
#elif defined ActionMotor
		#define ID_SELF 0x00010300
		#define ID_BACK 0x00030101
		#define BROADCAST_ID 0x00010000
	#ifdef PassRobot
		#define Motor_Rotate_PR 0//转向电机编号
		#define Motor_Pitch 1//俯仰电机编号
		#define Motor_Capture 2//手爪电机编号
	#elif defined TryRobot

	#endif
#endif

#define Can1ControlList 1
#define Can2ControlList 2

/* BIT编号，用于流程号判断 */
#define BIT0 (0x01<<0)
#define BIT1 (0x01<<1)
#define BIT2 (0x01<<2)
#define BIT3 (0x01<<3)
#define BIT4 (0x01<<4)
#define BIT5 (0x01<<5)
#define BIT6 (0x01<<6)
#define BIT7 (0x01<<7)

/* math function */
#define ABS(x)  ((x)>0? (x):(-(x)))
#define SIG(x)  ((x)>=0? 1:-1)
#define PEAK(A,B)   if(ABS(A)>B) A=SIG(A)*B; //此处默认B为正值

#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}
#define EncodeS16Data(f,buff)	{*(s16*)buff = *f;}
#define DecodeS16Data(f,buff)	{*f = *(s16*)buff;}
#define EncodeU16Data(f,buff)	{*(u16*)buff = *f;}
#define DecodeU16Data(f,buff)	{*f = *(u16*)buff;}
#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}

#endif
