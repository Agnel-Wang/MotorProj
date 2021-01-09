#include "main.h"

int main(void)
{
  Delay_ms(3000);//延时启动，等待机构上电
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);	
	RCC->AHB1ENR |= 7<<0;//使能PORTA，PORTB, PORTC时钟
	LED_Configuration();
	Beep_Configuration();
	Led8_Configuration();
	Key_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	USART1_Configuration();
	USART2_Configuration();
	TIM3_Configuration();
  TIM2_Configuration();
	param_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	OSInit();
	OSTaskCreate(Task_Start, (void *)0, &START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

/****起始任务****/
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL(); //进入临界区（无法被中断打断）
	RUN = OSSemCreate(0);
	OSTaskCreate(Task_Lcd, (void *)0, (OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE - 1],LCD_TASK_PRIO);
	OSTaskCreate(Task_Flag, (void *)0,(OS_STK *)&FLAG_TASK_STK[FLAG_STK_SIZE - 1], FLAG_TASK_PRIO);
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1],MOTOR_TASK_PRIO);
	OSTaskCreate(Task_Scope, (void *)0,(OS_STK *)&SCOPE_TASK_STK[SCOPE_STK_SIZE - 1], SCOPE_TASK_PRIO);
  OSTimeDly(100);
  /****初始工作****/
  
  
  
  
  
  /****************/
	Led8DisData(0);
	Beep_Show(2);//上电提醒，初始完成
  OSTaskSuspend(START_TASK_PRIO); //挂起起始任务
  OS_EXIT_CRITICAL();             //退出临界区
}

// LCD任务
static void Task_Lcd(void *pdata) 
{
  while (1) 
  {
    UsartLCDshow();
    OSTimeDly(1000);
  }
}

//标志位判断任务
static void Task_Flag(void *pdata) 
{
  ErrorList* _error;
  while (1) 
  {
    Key_Ctrl(); //矩阵键盘检测
    
    if(error.size == 0) Led_Show();
    else
    {
      LED_RED_ON;
      _error=error.head->next;
      for(int i=0; i<error.size; i++)
      {
        if((_error->code&0x000F)==0)
        {
          LED_GREEN_OFF;
          LedDisplay(LED_BLUE, (_error->code&0x00F0)>>4);
          LED_YELLOW_ON;
        }
        else
        {
          LedDisplay(LED_GREEN, _error->code&0x000F);
          LedDisplay(LED_BLUE, (_error->code&0x00F0)>>4);
          LedDisplay(LED_YELLOW, (_error->code&0x0F00)>>8);
        }
        _error = _error->next;
        OSTimeDly(1000);
        LED_BLUE_OFF;LED_GREEN_OFF;LED_YELLOW_OFF;
        OSTimeDly(6000);
      }
    }
    OSTimeDly(1000);
  }
}

// 电机动作执行任务
static void Task_Motor(void *pdata) 
{
  pdata = pdata;
  NMT_Operational(CAN1);OSTimeDly(5);NMT_Operational(CAN1);OSTimeDly(5);NMT_Operational(CAN1);OSTimeDly(5);
#ifdef USE_ELMO
  for(int i=0;i<50;i++)
  {
    MO_CAN1(0,SetData,1);
    OSTimeDly(1500);
  }
  if(ELMOmotor[0].enable&&ELMOmotor[1].enable&&ELMOmotor[2].enable&&ELMOmotor[3].enable)//选择要检测哪几个ELMO
  {
    Beep_Show(2);
    Led8DisData(1);
  }
  else 
  {
    Beep_Show(4);
    Led8DisData(2);
  }
    MO_CAN1(0,SetData,0);
    MO_CAN1(0,SetData,0);
#elif defined USE_VESC
  
  
#endif
  while (1) 
  {
    //摩擦轮发射
    if(VESC_fire)
    {
      if(motor[1].status.arrived)
      {
        VESCmotor[0].valSet.speed=-VESC_fire_speed;VESCmotor[1].valSet.speed=-VESC_fire_speed;
        OSTimeDly(20000);
        motor[1].valueSet.angle+=360;
        VESC_fire=false;
        OSTimeDly(20000);
        VESCmotor[0].valSet.speed=0;VESCmotor[1].valSet.speed=0;
      }
    }
    OSTimeDly(500);
  }
}

//示波器任务
static void Task_Scope(void *pdata) 
{
  while (1) 
  {
    VS4Channal_Send(0, 0, 0, 0);
    OSTimeDly(300);
  }
}
