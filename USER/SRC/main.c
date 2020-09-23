#include "main.h"

int main(void)
{
  Delay_ms(3000);//��ʱ�������ȴ������ϵ�
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);	
	RCC->AHB1ENR |= 7<<0;//ʹ��PORTA��PORTB, PORTCʱ��
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
	OSTaskCreate(Task_Scope, (void *)0,(OS_STK *)&SCOPE_TASK_STK[SCOPE_STK_SIZE - 1], SCOPE_TASK_PRIO);
	//Beep_Show(2);//�ϵ�����
#ifdef SteeringMotor//���ʹ�ܷ���������������ʼ���
  #if ID_SELF==MOTOR_0_3  //ELMO/EPOS�����ж�
    #ifdef USE_ELMO
      NMT_Operational(CAN1);OSTimeDly(5);NMT_Operational(CAN1);OSTimeDly(5);NMT_Operational(CAN1);OSTimeDly(5);
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
      else  insertError(error.head, ELMOERROR|((id+1)<<4)|TIMEOUT);
    #elif defined USE_EPOS
      
    #endif
    motor[0].enable=ENABLE;motor[3].enable=ENABLE;
  #elif ID_SELF==MOTOR_1_2
    motor[1].enable=ENABLE;motor[2].enable=ENABLE;
  #elif ID_SELF==MOTOR_all
    motor[0].enable=ENABLE;motor[1].enable=ENABLE;motor[2].enable=ENABLE;
  #elif ID_SELF == MOROE_4_and_2
    motor[0].enable=ENABLE;motor[1].enable=ENABLE;motor[2].enable=ENABLE;motor[3].enable=ENABLE;
    //motor[4].enable=ENABLE;motor[5].enable=ENABLE;
  #endif
#else
  #if defined USE_ELMO | defined USE_EPOS
  NMT_Operational(CAN2);OSTimeDly(5);NMT_Operational(CAN2);OSTimeDly(5);NMT_Operational(CAN2);OSTimeDly(5);
  #endif
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
  ErrorList* _error;
  while (1) 
  {
    Key_Ctrl(); //������̼��
    
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

// �������ִ������
static void Task_Motor(void *pdata) 
{
  pdata = pdata;

/*
 *   0: �޶���
 *   1������ȡ��
 *   2: ��ѹ
 */
  u8 pawAction=0;
  bool pawActionOK;
  
  while (1) 
  {
#ifdef PassRobot
    #ifdef SteeringMotor
      if(pawAction==1)//���㣬ȡ��
      {
        motor[4].valueSet.angle=0;motor[5].valueSet.angle=0;
        motor[4].limit.posSPlimit=2000;motor[4].limit.posSPlimit=2000;
        motor[4].begin=true;motor[5].begin=true;
        pawAction=0;
      }
      else if(pawAction==2)//����
      {
        motor[4].valueSet.angle=backPos;motor[5].valueSet.angle=backPos;
        motor[4].limit.posSPlimit=2000;motor[4].limit.posSPlimit=2000;
        if(ABS(motor[4].valueSet.angle-motor[4].valueReal.angle)<30)
        {
          motor[4].limit.posSPlimit=500;motor[4].limit.posSPlimit=500;
        }
        if(motor[4].status.stuck||motor[4].status.stuck)
        {
          motor[4].begin=false;motor[5].begin=false;
          pawAction=0;
        }
      }
    #elif defined ActionMotor
    
    #endif
#elif defined TryRobot  
    #ifdef SteeringMotor
    
    #elif defined ActionMotor
    
    #endif  
#endif
    OSTimeDly(500);
  }
}

//ʾ��������
static void Task_Scope(void *pdata) 
{
  while (1) 
  {
    VS4Channal_Send(ELMOmotor[0].valReal.speed,0, 0, 0);
    OSTimeDly(300);
  }
}
