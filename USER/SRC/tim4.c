#include "tim4.h"

void TIM4_Configuration(void)
{
  RCC->APB1ENR|=1<<0;//TIM2ʱ��ʹ��
  TIM4->ARR=699;//�Զ�����ֵ700
  TIM4->PSC=11;//Ԥ��Ƶ��ֵ12
  /*  Tout=(700*12)/84M=0.1ms  */
  TIM4->CR1 |= 1<<8;//Ԥװ��
  TIM4->SR &= ~(1<<0);//�����־λ
  TIM4->DIER |= 1<<0;//��������ж�
  TIM4->CR1 |= 0x01;//ʹ��TIM2
  MY_NVIC_Init(3,1,TIM4_IRQn,3);//�ж����ȼ�0,1������3
}
