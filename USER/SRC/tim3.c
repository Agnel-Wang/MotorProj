#include "tim3.h"

void TIM3_Configuration(void)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能
    TIM3->ARR=11999;//自动重载值1500
    TIM3->PSC=70-1;//预分频器值7
	TIM3->CR1 |= 1<<8;//预装载
    TIM3->SR &= ~(1<<0);//清除标志位
 	TIM3->DIER |= 1<<0;//允许更新中断
	TIM3->CR1 |= 0x01;//使能TIM3
    MY_NVIC_Init(1,0,TIM3_IRQn,3);//中断优先级0,1；分组3
}

void TIM3_IRQHandler(void)
{
  if(TIM3->SR & (1<<0))//溢出中断
  {
    #ifdef USE_DJ
    pulse_caculate();
    for(int i=0;i<8;i++)
		{  
      if(motor[i].enable)
      {
        if(motor[i].begin)
        {
          switch(motor[i].mode)
          {
            case current:   ;    break;
            case RPM:       speed_mode(i);      break;
            case position:  position_mode(i);   break;
            case zero:      zero_mode(i);       break;
            default:break;
          }
        }
        else position_mode(i);
      }
      currentInput(i);
		}
    #endif
        Can_DeQueue(CAN2,&Can2_Sendqueue);
        if(flag.Can2CtrlList_Enable)
          TraversalControlList(Can2_MesgSentList,&Can2_Sendqueue,CAN_2);
  }
  TIM3->SR &= ~(1<<0);//清除中断标志位
}
