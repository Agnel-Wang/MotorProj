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
        }
#endif
      if(ELMOmotor[0].valReal.pulse>KickPulse)
      {
        SP(1, SetData, 50, 0);
        BG(1, 0);
      }
    }
    TIM2->SR &= ~(1<<0);//清除中断标志位
}
