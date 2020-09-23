#include "tim2.h"

void TIM2_Configuration(void)
{
  RCC->APB1ENR|=1<<0;//TIM2时钟使能
  TIM2->ARR=6999;//自动重载值7000
  TIM2->PSC=11;//预分频器值12
  /*  Tout=(7000*12)/84M=1ms  */
  TIM2->CR1 |= 1<<8;//预装载
  TIM2->SR &= ~(1<<0);//清除标志位
  TIM2->DIER |= 1<<0;//允许更新中断
  TIM2->CR1 |= 0x01;//使能TIM2
  MY_NVIC_Init(1,1,TIM2_IRQn,3);//中断优先级0,1；分组3
}

void ElmoAction(void);
void ElmoAskStatus(void);
void EposAction(void);
void EposAskStatus(void);
static vu8 AskTimeCnt = 0; 
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & (1<<0))//溢出中断
    {
#ifdef USE_VESC
        for(int i=0;i<4;i++)
        {
            if(VESCmotor[i].enable)
            {
                if(VESCmotor[i].begin)
                {
                    switch(VESCmotor[i].mode)
                    {
                        case current:   VESC_Set_Current(i+1, VESCmotor[i].valSet.current, 0);                                  break;
                        case duty:      VESC_Set_Duty_Cycle(i+1, VESCmotor[i].valSet.duty, 0);                                  break;
                        case RPM:       VESC_Set_Speed(i+1, VESCmotor[i].valSet.speed * VSCmotor[i].instrinsic.POLE_PAIRS, 0);  break;
                        case brake:     VESC_Set_Brake_Current(i+1, VESCmotor[id].limit.breakCurrent, 0);                       break;
                        default:break;
                    }
                }
                else VESC_Set_Brake_Current(i+1,VESCmotor[i].limit.breakCurrent,0);
            }
            else VESC_Set_Current(i+1,0.0,0);
            /* 反馈超时判断 */
            if(VESCmotor[id].enable&&((OSTimeGet()-VESCmotor[id].argum.lastRxTim)>VESCmotor[id].argum.timeoutTicks)) VESCmotor[id].argum.timeoutCnt++;
            else VESCmotor[id].argum.timeoutCnt=0;
            if(VESCmotor[id].argum.timeoutCnt>10) 
            {
              VESCmotor[id].status.timeout=true;
              insertError(error.head, VESCERROR|((id+1)<<4)|TIMEOUT);
            }
            else 
            {
              VESCmotor[id].status.timeout=false;
              deleteError(error.head, ErrorFind(error.head, VESCERROR|((id+1)<<4)|TIMEOUT));
            }
        }
#endif
//注意当ELMO、EPOS一起用时两次查询发送速度问题
#ifdef USE_ELMO
  ElmoAskStatus();
  ElmoAction();
#endif
#ifdef USE_EPOS
  EposAskStatus();
  ElmoAction();
#endif
    }
    TIM2->SR &= ~(1<<0);//清除中断标志位
}

/* ELMO 状态询问 */
void ElmoAskStatus(void)
{
   if(AskTimeCnt++>3)
  {
    AskTimeCnt=0;
   // VX(0, 0); 
    PX(0, GetData, 0);
  }
  /* 反馈超时判断 */
  for(int id=0;id<4;id++)
  {
    if(ELMOmotor[id].enable&&((OSTimeGet()-ELMOmotor[id].argum.lastRxTim)>ELMOmotor[id].argum.timeoutTicks)) ELMOmotor[id].argum.timeoutCnt++;//反馈超时判断
    else ELMOmotor[id].argum.timeoutCnt=0;
    if(ELMOmotor[id].argum.timeoutCnt>10) 
    {
      ELMOmotor[id].status.timeout=true;
      insertError(error.head, ELMOERROR|((id+1)<<4)|TIMEOUT);
    }
    else 
    {
      ELMOmotor[id].status.timeout=false;
      deleteError(error.head, ErrorFind(error.head, ELMOERROR|((id+1)<<4)|TIMEOUT));
    }
  }
}

uc8 KickTimes=5;//最多踢5次
/* ELMO执行动作计算 */
void ElmoAction(void)
{
  #ifdef ActionMotor
#ifdef TryRobot
  for(int i=0; i<2; i++)
  {
    if(kick[i].init)
    {   
      kick[i].init=false;
      kick[i].cnt=0;
      UM(i+1, SetData, 0, ELMOmotor[i].mode);
      PX(i+1, SetData, 0, 0);
      SP(i+1, SetData, 0, ELMOmotor[i].valSet.speed);
      kick[i].cnt++;
      ELMOmotor[i].valSet.angle= kick[i].cnt*360;
    }
    if(ELMOmotor[i].enable)
    {
      if(ABS(ELMOmotor[i].valReal.angle-ELMOmotor[i].valSet.angle)<10)
      {
        if(++kick[i].cnt>KickTimes)
          kick[i].ok=true;
        else
        {
          ELMOmotor[i].valSet.angle=kick[i].cnt*360;
          if(kick[i].isAutoKick)
          {
            if(kick[i].waitCnt++>kick[i].waittime)
            {
              kick[i].waitCnt=0;
              PA(i+1, SetData, 0, ELMOmotor[i].valSet.angle);
              BG(i+1, 0);
            }
          }
        }
      }
    }
  }
#elif defined PassRobot
  for(int i=0; i<3; i++)
  {
    if(kick[i].init)
    {
      kick[i].init=false;
      kick[i].cnt=0;
      UM(i+1, SetData, 0, ELMOmotor[i].mode);
      PX(i+1, SetData, 0, 0);
      SP(i+1, SetData, 0, ELMOmotor[i].valSet.speed);
      kick[i].cnt++;
      ELMOmotor[i].valSet.angle=kick[i].cnt*720;
    }
    if(ELMOmotor[i].enable)
    {
      if(!kick[i].prepareOK)
      {
        if(ABS(ELMOmotor[i].valReal.angle-preparePOS)<10)
        {
          kick[i].prepareOK=true;
          SP(i+1, SetData, 0, ELMOmotor[i].valSet.speed);
        }
      }
      if(kick[i].prepareOK&&(ABS(ELMOmotor[i].valReal.angle-ELMOmotor[i].valSet.angle)<10))
      {
        kick[i].cnt++;
          ELMOmotor[i].valSet.angle=kick[i].cnt*1440;
      }
    }
  }
#endif
#endif
}

void EposAskStatus(void)
{
  if(AskTimeCnt++>3)
  {
    AskTimeCnt=0;
    
  }
  
}

void EposAction()
{

}
