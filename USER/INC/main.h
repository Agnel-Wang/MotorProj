#ifndef _MAIN_H
#define _MAIN_H

#include "elmo.h"
#include "led.h"
#include "beep.h"
#include "param.h"
#include "led8.h"
#include "usart1.h"
#include "usart2.h"
#include "tim3.h"
#include "tim2.h"
#include "can1.h"
#include "visual_scope.h"
#include "key.h"
#include "common_can.h"

/****UCOSII任务设置****/
//START任务
#define START_TASK_PRIO         5
#define START_STK_SIZE      	256
__align(8) OS_STK START_TASK_STK[START_STK_SIZE];
static void Task_Start(void *pdata);

//LCD任务
#define LCD_TASK_PRIO       	25
#define LCD_STK_SIZE     		512
__align(8) OS_STK LCD_TASK_STK[LCD_STK_SIZE];
static void Task_Lcd(void *pdata);

//Flag任务
#define FLAG_TASK_PRIO			30
#define FLAG_STK_SIZE			128
__align(8) OS_STK FLAG_TASK_STK[FLAG_STK_SIZE];
static void Task_Flag(void *pdata);

//电机动作执行任务
#define	MOTOR_TASK_PRIO		    40
#define MOTOR_STK_SIZE		    1024
__align(8) OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
static void Task_Motor(void *pdata);

//虚拟示波器
#define SCOPE_TASK_PRIO			55
#define SCOPE_STK_SIZE			128
__align(8) OS_STK SCOPE_TASK_STK[SCOPE_STK_SIZE];
static void Task_Scope(void *pdata);

OS_EVENT *RUN;

#endif
