#include "main.h"

int main(void)
{
  Delay_ms(3000);//��ʱ�������ȴ������ϵ�
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);	
	RCC->AHB1ENR |= 7<<0;//ʹ��PORTA��PORTB,PORTCʱ��
	LED_Configuration();
	Beep_Configuration();
	Led8_Configuration();
	Key_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	USART1_Configuration();
	USART2_Configuration();
	TIM3_Configuration();
	param_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	OSInit();
	OSTaskCreate(Task_Start, (void *)0, &START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

/****��ʼ����****/
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL(); //�����ٽ������޷����жϴ�ϣ�
	RUN = OSSemCreate(0);
	OSTaskCreate(Task_Lcd, (void *)0, (OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE - 1],LCD_TASK_PRIO);
	OSTaskCreate(Task_Flag, (void *)0,(OS_STK *)&FLAG_TASK_STK[FLAG_STK_SIZE - 1], FLAG_TASK_PRIO);
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1],MOTOR_TASK_PRIO);
	OSTaskCreate(Task_Elmo, (void *)0, (OS_STK *)&ELMO_TASK_STK[ELMO_STK_SIZE - 1],ELMO_TASK_PRIO);
	OSTaskCreate(Task_Scope, (void *)0,(OS_STK *)&SCOPE_TASK_STK[SCOPE_STK_SIZE - 1], SCOPE_TASK_PRIO);
	Beep_Show(2);//�ϵ�����
#ifdef SteeringMotor
  #if ID_SELF==MOTOR_0_3  //ELMO�����ж�
	ELMO_Init(CAN1);OSTimeDly(5);ELMO_Init(CAN1);OSTimeDly(5);ELMO_Init(CAN1);OSTimeDly(5);
      for(int i=0;i<20;i++)
      {
        MO_CAN1(0,SetData,1);
        OSTimeDly(100);
      }
      if(ELMOmotor[0].enable&&ELMOmotor[1].enable&&ELMOmotor[2].enable&&ELMOmotor[3].enable)
      {
        MO_CAN1(0,SetData,0);
        MO_CAN1(0,SetData,0);
      }
      else  flag.led=error;
  #endif
#else
	ELMO_Init(CAN2);OSTimeDly(5);ELMO_Init(CAN2);OSTimeDly(5);ELMO_Init(CAN2);OSTimeDly(5);
#endif
	Led8DisData(0);
  OSTaskSuspend(START_TASK_PRIO); //������ʼ����
  OS_EXIT_CRITICAL();             //�˳��ٽ���
}

// LCD����
static void Task_Lcd(void *pdata) 
{
  while (1) 
  {
    UsartLCDshow();
    OSTimeDly(1000);
  }
}

//��־λ�ж�����
static void Task_Flag(void *pdata) 
{
  while (1) 
  {
    Key_Ctrl(); //������̼��
    
    switch (flag.led) 
    {
      case normal: Led_Show(); break;
      case error:
          LED_RED_ON;LED_YELLOW_TOGGLE;LED_BLUE_OFF;LED_GREEN_OFF;
        break;
    }
    OSTimeDly(1000);
  }
}

// �������ִ������
static void Task_Motor(void *pdata) 
{
  pdata = pdata;

  while (1) 
  {
    
    OSTimeDly(1000);
  }
}

// Elmo��ѯ����
static void Task_Elmo(void *pdata) 
{
  while (1) 
  {
    VX(0, 0); PX(0, GetData, 0); IQ(0, 0); MO(0, GetData, 0);
    OSTimeDly(2000);
  }
}

//ʾ��������
static void Task_Scope(void *pdata) 
{
  while (1) 
  {
    VS4Channal_Send(motor[1].valueReal.pulse, motor[1].valueReal.speed, 0, 0);
    OSTimeDly(30);
  }
}
