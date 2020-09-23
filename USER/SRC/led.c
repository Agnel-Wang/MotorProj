#include "led.h"

void LED_Configuration()
{
	GPIO_Set(GPIOA,PIN4|PIN5|PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_NONE);
}

void Led_Show(void)
{
	LED_RED_ON;OSTimeDly(2000);
	LED_YELLOW_ON;OSTimeDly(2000);
	LED_BLUE_ON;OSTimeDly(2000);
	LED_GREEN_ON;OSTimeDly(2000);
	LED_RED_OFF;OSTimeDly(2000);
	LED_YELLOW_OFF;OSTimeDly(2000);
	LED_BLUE_OFF;OSTimeDly(2000);
	LED_GREEN_OFF;OSTimeDly(2000);
}

void LedDisplay(u8 led, u8 num)
{
  for(int i=0; i<num; i++)
  {
    GPIOA->BSRRH = led;
    OSTimeDly(2000);
    GPIOA->BSRRL = led;
    OSTimeDly(2000);
  }
}
