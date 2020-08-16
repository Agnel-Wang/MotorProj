#include "led8.h"

const unsigned char LED_table[]={0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E,0xFF,0x00};//ȫ����ȫ��
/*
****************************************
* Name : Led8_configuration
* Describe : ��ʼ�������
* Input : None
* Output : None
* Return : None
****************************************
*/
void Led8_Configuration(void)
{
	GPIO_Set(GPIOB,PIN8|PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
	Set_LED8CLK();
	Reset_LED8DATA();
}

/*
****************************************
* Name : Led8DisData
* Describe : �����������ʾ����
* Input : Data  ��Ҫ�����������ʾ��ʮ���������֣���Χ��0��F��������ȫ��ȫ��
* Output : None
* Return : None
****************************************
*/		
void Led8DisData(unsigned char Data)
{
	unsigned char i,val;
	if(Data>17) Data=17;			//��ֹ�����βγ���LED_table���飬��ɷǷ�����
	val=LED_table[Data];
	for(i=0;i<8;i++)
	{
		Reset_LED8CLK();
		if(val & 0x01) Set_LED8DATA();
		else Reset_LED8DATA();
		Set_LED8CLK();
		val>>=1;
	}	
}


