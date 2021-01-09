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
            VESC_caculate(&VESCmotor[i]);
            if(VESCmotor[i].enable)
            {
                if(VESCmotor[i].begin)
                {
                    switch(VESCmotor[i].mode)
                    {
                        case current:   VESC_Set_Current(i+1, VESCmotor[i].valSet.current, 0);                                  break;
                        case duty:      VESC_Set_Duty_Cycle(i+1, VESCmotor[i].valSet.duty, 0);                                  break;
                        case RPM:       VESC_Set_Speed(i+1, VESCmotor[i].valSet.speed * VESCmotor[i].instrinsic.POLE_PAIRS, 0);  break;
                        case brake:     VESC_Set_Brake_Current(i+1, VESCmotor[i].limit.breakCurrent, 0);                       break;
                        case position:  VESC_position_mode(i); break;
                        case vesc_angle:  VESC_angle_mode(i); break;
                      default:break;
                    }
                }
                else VESC_Set_Brake_Current(i+1,VESCmotor[i].limit.breakCurrent,0);
            }
            else VESC_Set_Current(i+1,0.0,0);
            
            /* 反馈超时判断 */
            if(VESCmotor[i].enable&&((OSTimeGet()-VESCmotor[i].argum.lastRxTim)>VESCmotor[i].argum.timeoutTicks)) VESCmotor[i].argum.timeoutCnt++;
            else VESCmotor[i].argum.timeoutCnt=0;
            if(VESCmotor[i].argum.timeoutCnt>10) 
            {
              VESCmotor[i].status.timeout=true;
              insertError(error.head, VESCERROR|((i+1)<<4)|TIMEOUT);
            }
            else 
            {
              VESCmotor[i].status.timeout=false;
              deleteError(error.head, ErrorFind(error.head, VESCERROR|((i+1)<<4)|TIMEOUT));
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
  EposAction();
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

/* ELMO执行动作计算 */
void ElmoAction(void)
{
  
  
}

void EposAskStatus(void)
{
  
}

void EposAction()
{
  
}
