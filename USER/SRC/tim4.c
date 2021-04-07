#include "tim4.h"

void TIM4_Configuration(void)
{
  RCC->APB1ENR|=1<<0;//TIM2时钟使能
  TIM4->ARR=699;//自动重载值700
  TIM4->PSC=11;//预分频器值12
  /*  Tout=(700*12)/84M=0.1ms  */
  TIM4->CR1 |= 1<<8;//预装载
  TIM4->SR &= ~(1<<0);//清除标志位
  TIM4->DIER |= 1<<0;//允许更新中断
  TIM4->CR1 |= 0x01;//使能TIM2
  MY_NVIC_Init(3,1,TIM4_IRQn,3);//中断优先级0,1；分组3
}
