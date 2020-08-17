#include "param.h"

/* 踢球常量定义 */
sc32 KickPulse = 100000;
sc32 KickSP = 120000;
sc16 KickWaitTim = 1000;
bool isAutoKick = false;

void param_Init(void)
{
    Flag_Init();
    Motor_Init();
}

void Flag_Init(void)
{
    flag.led=normal;
    flag.Can2CtrlList_Enable=true;
    
}

/* 改变16位数据高低顺序 */
 void ChangeData(u8* buff2,u8* buff1)
 {
	u8 temp=0;
    temp= *buff1;
	*buff1 = *buff2;	 
	*buff2 = temp;
 }

Usart_Struct usart;
FlagTypeDef flag;
