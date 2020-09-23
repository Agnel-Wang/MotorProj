#include "usart2.h"

void USART2_Configuration()
{
  RCC->APB1ENR |= 1<<17;             //使能USART2时钟
  RCC->AHB1ENR |= 1<<21;             //使能DMA1时钟
  GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
  GPIO_AF_Set(GPIOA,2,7);            //复用PA2，PA3到USART2
  GPIO_AF_Set(GPIOA,3,7);
  USART2->BRR = (u32)0x016C;         //Bound rate set, 115200,72Mhz
  USART2->CR1 = (u32)0x000C;         //USART2 setting
  USART2->SR &= ~(1<<5);             //清除RXNE标志位
  USART2->CR1 |= 1<<5;               //使能接收中断
  MY_NVIC_Init(4,1,USART2_IRQn,3);
  MYDMA_Config(DMA1_Stream6,4,(u32)&USART2->DR,(u32)usart.TxBuffer_USART2,USART2_Tx_BufferSize);
  DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);//使能传输完成中断
  MY_NVIC_Init(7,1,DMA1_Stream6_IRQn,3);
  USART2->CR3 |= 1<<7;               //DMA1使能发送接收器
  USART2->CR1 |= 1<<13;              //使能串口1
}

void DMA1_Stream6_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) 
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    flag.Usart2DMASendFinish = 0;
  }
}

//串口屏
vu8 interface=0;
u8 USART2_RX_STA = 0;
void USART2_IRQHandler(void) 
{
  u8 temp = 0;
  if(USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET) 
  {
    USART_ReceiveData(USART2);
    USART2_RX_STA = 0;
    memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
  }
  else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
  {
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    temp = USART_ReceiveData(USART2);
    if ((USART2_RX_STA & 0x40) != 0) 
    {
      if ((USART2_RX_STA & 0x80) == 0)
      {
        if (temp == 0xff)  USART2_RX_STA |= 0x80;
        else 
        {
          usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = temp;
          USART2_RX_STA++;
          if ((USART2_RX_STA & 0X3F) > 40)  USART2_RX_STA = 0;
        }
      }
    }
    else if (temp == 0xee)   USART2_RX_STA |= 0x40;
    if (((USART2_RX_STA & 0x80) != 0))//串口屏指令判断
    {
      if(usart.RxBuffer_USART2[1] == 0x01)  interface=usart.RxBuffer_USART2[3];
      switch (usart.RxBuffer_USART2[3]) 
      {
        case 0x00: //待机界面
        {
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x08:RESET_PRO
              break;
            default:;
          }
        }break;
        case 0x01: //P车主界面
        {
          
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x02://急停
            {
              MO(1, SetData, 0, 0);MO(2, SetData, 0, 0);MO(3, SetData, 0, 0);
            }break;
            case 0x03://自动踢球
            {
              kick[kickBall-1].isAutoKick=usart.RxBuffer_USART2[8];
            }break;
            case 0x04://kickball ID
            {
              kickBall=atof((char*)(&usart.RxBuffer_USART2[7]));
            }break;
            case 0x06://设定速度
            {
              ELMOmotor[kickBall-1].valSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              SP(kickBall, SetData, 0, ELMOmotor[kickBall-1].valSet.speed);
            }break;
            case 0x07://设定位置
            {
              ELMOmotor[kickBall-1].valSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
            }break;
            case 0x08://初始化
            {
              kick[kickBall-1].init=usart.RxBuffer_USART2[8];
            }break;
            case 0x09://位置清零
            {
              PX(kickBall, SetData, 0, 0);
            }break;
            case 0x0A://BG
            {
              PA(kickBall, SetData, 0, ELMOmotor[kickBall-1].valSet.angle);
              
              BG(kickBall, 0);
            }break;
            case 0x0B://MO
            {
              MO(1, SetData, 0, usart.RxBuffer_USART2[8]);
            }break;
            case 0x0C://MO
            {
              MO(2, SetData, 0, usart.RxBuffer_USART2[8]);
            }break;
            case 0x0D://MO
            {
              MO(3, SetData, 0, usart.RxBuffer_USART2[8]);
            }break;
            default:;
          }
        }break;
        case 0x02: //T车主界面
        {
            switch (usart.RxBuffer_USART2[5])
            {
                case 0x01: ELMOmotor[kickBall-1].valSet.speed = atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
                  break;
                case 0x02: ELMOmotor[kickBall-1].valSet.angle = atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
                  break;
                case 0x03:PX(1, SetData, 0, 0);PX(2, SetData, 0, 0);Beep_Show(1);
                  break;
                case 0x04:MO(kickBall, SetData, 0, 1);Beep_Show(1);
                  break;
                case 0x05:Beep_Show(1);
                      BG(kickBall, 0);
                      ELMOmotor[kickBall-1].status.arrived=false;
                  break;
                case 0x06:Beep_Show(1);
                    if(kick[0].isAutoKick)
                    {
                      MO(1, SetData, 0, 0);MO(2, SetData, 0, 0);
                    }
                    else
                    MO(kickBall, SetData, 0, 0);
                  break;
                case 0x07:PA(kickBall, SetData, 0, ELMOmotor[kickBall-1].valSet.angle);Beep_Show(1);
                  break;
                case 0x08:SP(kickBall, SetData, 0, ELMOmotor[kickBall-1].valSet.speed);Beep_Show(1);
                  break;
                case 0x0B:kick[kickBall-1].init=true;kick[kickBall-1].cnt=0;kick[kickBall-1].ok=false;Beep_Show(1);
                  break;
                case 0x0C:RESET_PRO
                  break;
                case 0x0D:kickBall=usart.RxBuffer_USART2[8]+1;Beep_Show(kickBall);
                  break;
                case 0x0E:
                    if(usart.RxBuffer_USART2[8])
                    {
                      kick[0].isAutoKick=true;
                      kick[1].isAutoKick=true;
                      Beep_Show(2);
                    }
                    else
                    {
                      kick[0].isAutoKick=false;
                      kick[1].isAutoKick=false;
                      Beep_Show(1);
                    }
                break;
                default:;
            }
        }break;
        case 0x04://DJ界面
        {
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x03:
                for(int i=0;i<8;i++) motor[i].enable=false;
              break;
            case 0x04:
                motor[0].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x05:
                motor[1].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x06:
                motor[2].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x07:
                motor[3].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x08:
                motor[0].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x09:
                motor[1].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0A:
                motor[2].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0B:
                motor[3].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0C:
                motor[0].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0D:
                motor[1].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0E:
                motor[2].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0F:
                motor[3].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x11:
                motor[0].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x12:
                motor[1].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x13:
                motor[2].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x14:
                motor[3].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x15:
                motor[0].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x16:
                motor[1].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x17:
                motor[2].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x18:
                motor[3].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x19:
                motor[0].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1A:
                motor[1].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1B:
                motor[2].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1C:
                motor[3].begin=usart.RxBuffer_USART2[8];
              break;
            default:;
          }
        }break;
        case 0x05://DJ界面-副
        {
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x03:
                for(int i=0;i<8;i++) motor[i].enable=false;
              break;
            case 0x04:
                motor[4].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x05:
                motor[5].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x06:
                motor[6].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x07:
                motor[7].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x08:
                motor[4].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x09:
                motor[5].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0A:
                motor[6].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0B:
                motor[7].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0C:
                motor[4].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0D:
                motor[5].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0E:
                motor[6].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x0F:
                motor[7].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x11:
                motor[4].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x12:
                motor[5].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x13:
                motor[6].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x14:
                motor[7].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
              break;
            case 0x15:
                motor[4].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x16:
                motor[5].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x17:
                motor[6].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x18:
                motor[7].enable=usart.RxBuffer_USART2[8];
              break;
            case 0x19:
                motor[4].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1A:
                motor[5].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1B:
                motor[6].begin=usart.RxBuffer_USART2[8];
              break;
            case 0x1C:
                motor[7].begin=usart.RxBuffer_USART2[8];
              break;
            default:;
          }
        }break;
        case 0x07://EPOS界面
        {
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x02://Boot Up
            {
              EPOS_BootUP(1, 0);EPOS_BootUP(2, 0);EPOS_BootUP(3, 0);EPOS_BootUP(4, 0);
            }break;
            case 0x03://Set PVM
            {
              EPOS_SetMode(1, 3, 0);EPOS_SetMode(2, 3, 0);EPOS_SetMode(3, 3, 0);EPOS_SetMode(4, 3, 0);
            }break;
            case 0x04://Set PPM
            {
              EPOS_SetMode(1, 1, 0);EPOS_SetMode(2, 1, 0);EPOS_SetMode(3, 1, 0);EPOS_SetMode(4, 1, 0);
            }break;
            case 0x05://Clear Fault
            {
              EPOS_ClearFault(1, 0);EPOS_ClearFault(2, 0);EPOS_ClearFault(3, 0);EPOS_ClearFault(4, 0);
            }break;
            case 0x06://Start Motor
            {
              EPOS_StartMotor(1, 0);EPOS_StartMotor(2, 0);EPOS_StartMotor(3, 0);EPOS_StartMotor(4, 0);
            }break;
            case 0x07://Relieve
            {
              EPOS_RelieveMotor(1, 0);EPOS_RelieveMotor(2, 0);EPOS_RelieveMotor(3, 0);EPOS_RelieveMotor(4, 0);
            }break;
            case 0x08://Set Speed
            {
              EPOS_SetPVMspeed(1, 40, 0);EPOS_SetPVMspeed(2, -40, 0);EPOS_SetPVMspeed(3, -40, 0);EPOS_SetPVMspeed(4, 40, 0);
            }break;
            case 0x09://Enable Operation
            {
              EPOS_EnableOperation(1, 0);EPOS_EnableOperation(2, 0);EPOS_EnableOperation(3, 0);EPOS_EnableOperation(4, 0);
            }break;
            default:;
          }
        }break;
        default:;
      }
      USART2_RX_STA = 0;
      memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
    }
  }
}

static void USART2_Send(u8 count)
{
    if(!flag.Usart2DMASendFinish)//若上一次传输未完成，则舍弃本次传输
    {
      DMA_SetCurrDataCounter(DMA1_Stream6,count);
      DMA_Cmd(DMA1_Stream6, ENABLE);
      flag.Usart2DMASendFinish=1;
    }
}

char str_temp[32];
void UsartLCDshow(void)
{
    u8 i = 0;//用于串口数据包的下标

      /*****主界面****/
    usart.TxBuffer_USART2[i++]=0xee;
    usart.TxBuffer_USART2[i++]=0xb1;
    usart.TxBuffer_USART2[i++]=0x10;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x05;
    usart.TxBuffer_USART2[i++]=0x01;

    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xfc;
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xff;
  
    switch(interface)
    {
    case 0x01:/****P车主界面****/
    {
      usart.TxBuffer_USART2[i++]=0xee;
      usart.TxBuffer_USART2[i++]=0xb1;	
      usart.TxBuffer_USART2[i++]=0x12;	
      usart.TxBuffer_USART2[i++]=0x00;	
      usart.TxBuffer_USART2[i++]=0x01;
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x03;
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x01;
      usart.TxBuffer_USART2[i++]=kick[kickBall-1].isAutoKick;
    
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x04;
      usart.TxBuffer_USART2[i++]=0x00;
      sprintf(str_temp,"%d",kickBall);
      usart.TxBuffer_USART2[i++]=strlen(str_temp);
      strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
      i += strlen(str_temp); 
            
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x05;
      usart.TxBuffer_USART2[i++]=0x00;
      sprintf(str_temp,"%d",ELMOmotor[kickBall-1].valReal.angle);
      usart.TxBuffer_USART2[i++]=strlen(str_temp);
      strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
      i += strlen(str_temp); 
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x06;
      usart.TxBuffer_USART2[i++]=0x00;
      sprintf(str_temp,"%d",ELMOmotor[kickBall-1].valSet.speed);
      usart.TxBuffer_USART2[i++]=strlen(str_temp);
      strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
      i += strlen(str_temp); 
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x07;
      usart.TxBuffer_USART2[i++]=0x00;
      sprintf(str_temp,"%d",ELMOmotor[kickBall-1].valSet.angle);
      usart.TxBuffer_USART2[i++]=strlen(str_temp);
      strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
      i += strlen(str_temp); 
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x0B;
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x01;
      usart.TxBuffer_USART2[i++]=ELMOmotor[0].enable;
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x0C;
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x01;
      usart.TxBuffer_USART2[i++]=ELMOmotor[1].enable;
      
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x0D;
      usart.TxBuffer_USART2[i++]=0x00;
      usart.TxBuffer_USART2[i++]=0x01;
      usart.TxBuffer_USART2[i++]=ELMOmotor[2].enable;
      
      usart.TxBuffer_USART2[i++]=0xff;
      usart.TxBuffer_USART2[i++]=0xfc;
      usart.TxBuffer_USART2[i++]=0xff;
      usart.TxBuffer_USART2[i++]=0xff; 
    }break;
    case 2:/****T车主界面****/
    {
    usart.TxBuffer_USART2[i++]=0xee;
    usart.TxBuffer_USART2[i++]=0xb1;	
    usart.TxBuffer_USART2[i++]=0x12;	
    usart.TxBuffer_USART2[i++]=0x00;	
    usart.TxBuffer_USART2[i++]=0x02;

    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%5d",ELMOmotor[kickBall-1].valSet.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);

    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x02;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%5d",ELMOmotor[kickBall-1].valSet.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);

    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0A;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%5d",ELMOmotor[kickBall-1].valReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);

    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xfc;
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xff;
  }break;
    case 3:
    {
    
    }break;
    case 4:/****DJ界面****/
    {
    usart.TxBuffer_USART2[i++]=0xee;
    usart.TxBuffer_USART2[i++]=0xb1;	
    usart.TxBuffer_USART2[i++]=0x12;	
    usart.TxBuffer_USART2[i++]=0x00;	
    usart.TxBuffer_USART2[i++]=0x04;
    
    if(motor[0].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x04;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[0].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x08;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[0].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0C;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[0].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
     
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x11;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[0].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
        
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x15;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[0].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x19;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[0].begin;
    }
    if(motor[1].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x05;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[1].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
      
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x09;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[1].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
 
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0D;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[1].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x12;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[1].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x16;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[1].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1A;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[1].begin;
    }
    if(motor[2].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x06;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[2].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0A;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[2].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);   
      
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0E;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[2].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x13;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[2].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x17;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[2].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1B;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[2].begin;
    }
    if(motor[3].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x07;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[3].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
      
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0B;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[3].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);  
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0F;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[3].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x14;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[3].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x18;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[3].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1C;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[3].begin;
    }
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x10;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%x",error.head->next->code);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xfc;
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xff;  
  }break;
    case 5:/****DJ界面-副****/
    {
    usart.TxBuffer_USART2[i++]=0xee;
    usart.TxBuffer_USART2[i++]=0xb1;	
    usart.TxBuffer_USART2[i++]=0x12;	
    usart.TxBuffer_USART2[i++]=0x00;	
    usart.TxBuffer_USART2[i++]=0x05;
    
    if(motor[4].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x04;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[4].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x08;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[4].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0C;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[4].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x11;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[4].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x15;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[4].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x19;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[4].begin;
    }
    if(motor[5].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x05;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[5].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);  

    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x09;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[5].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
        
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0D;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[5].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x12;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[5].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x16;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[5].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1A;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[5].begin;
    }
    if(motor[6].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x06;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[6].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0A;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[6].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0E;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[6].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x13;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[6].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x17;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[6].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1B;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[6].begin;
    }
    if(motor[7].mode != 0)
    {
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x07;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[7].mode);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);    
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0B;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[7].valueReal.speed);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);  
   
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x0F;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[7].valueReal.angle);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x14;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%d",motor[7].limit.posSPlimit);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x18;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[7].enable;
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x1C;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x01;
    usart.TxBuffer_USART2[i++]=motor[7].begin;
    }
    
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x10;
    usart.TxBuffer_USART2[i++]=0x00;
    sprintf(str_temp,"%x",error.head->next->code);
    usart.TxBuffer_USART2[i++]=strlen(str_temp);
    strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
    i += strlen(str_temp);
    
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xfc;
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xff;
  }break;
    default:;
    }
    USART2_Send(i);
}
