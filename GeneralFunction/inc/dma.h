#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
#include "stm32f4xx_dma.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DMA ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u8 chx,u32 par,u32 mar,u16 ndtr);//����DMAx_CHx
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//ʹ��һ��DMA����		   
#endif