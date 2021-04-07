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
static bool  sendt = 0;
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
        for(int i=0;i<1;i++)
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
                        case position:          VESC_position_mode_pos(i); break;
                        case vesc_postion_rpm:  VESC_position_mode_rpm(i); break;
                        case vesc_postion_I:    VESC_position_mode_I(i);   break;
                        case vesc_RPM_I:        VESC_RPM_mode_I(i);        break;
                      default:break;
                    }
                }
                else 
                  //VESC_Set_Brake_Current(i+1,VESCmotor[i].limit.breakCurrent,0);
                  VESC_position_mode_pos(i);
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
#if USE_TMOTOR
        static u16 overcnt;
        switch(overturn.process)
        {
            case 1:
                tmotor[TMOTOR_FR].valSet.angle = -overturn.angle[0][0];
                tmotor[TMOTOR_RR].valSet.angle = -overturn.angle[0][1];
                tmotor[TMOTOR_RL].valSet.angle = overturn.angle[0][1];
                tmotor[TMOTOR_FL].valSet.angle = overturn.angle[0][0];
                break;
            case 2:
                tmotor[TMOTOR_FL].valSet.angle = overturn.angle[1][0];
                tmotor[TMOTOR_FR].valSet.angle = -overturn.angle[1][0];
                tmotor[TMOTOR_RR].valSet.angle = -overturn.angle[0][1];
                tmotor[TMOTOR_RL].valSet.angle = overturn.angle[0][1];
                break;
            case 3:
                tmotor[TMOTOR_RR].valSet.angle = -overturn.angle[2][1];
                tmotor[TMOTOR_RL].valSet.angle = overturn.angle[2][1];
                tmotor[TMOTOR_FL].valSet.angle = overturn.angle[1][0];
                tmotor[TMOTOR_FR].valSet.angle = -overturn.angle[1][0];
                break;
            case 4:
                if(overcnt==0)
                {
                    overturn.delta[0] = overturn.angle[3][0]-tmotor[TMOTOR_FL].valReal.angle;
                    overturn.downangdelta[0] = overturn.delta[0]/overturn.downtim;
                    overturn.delta[1] = overturn.angle[3][1]-tmotor[TMOTOR_RL].valReal.angle;
                    overturn.downangdelta[1] = overturn.delta[1]/overturn.downtim;
                }
                if(overcnt<overturn.downtim)
                    overcnt++;
                else
                {    
                    overturn.process = 0;
                    overcnt = 0;
                }
                tmotor[TMOTOR_FR].valSet.angle -= overturn.downangdelta[0];
                tmotor[TMOTOR_FL].valSet.angle += overturn.downangdelta[0];
                tmotor[TMOTOR_RR].valSet.angle -= overturn.downangdelta[1];
                tmotor[TMOTOR_RL].valSet.angle += overturn.downangdelta[1];
                break;
            case 5:
                if(overcnt==0)
                {
                    overturn.delta[0] = -tmotor[TMOTOR_FL].valReal.angle;
                    overturn.downangdelta[0] = overturn.delta[0]/overturn.downtim;
                    overturn.delta[1] = -tmotor[TMOTOR_RL].valReal.angle;
                    overturn.downangdelta[1] = overturn.delta[1]/overturn.downtim;
                }
                if(overcnt<overturn.downtim)
                    overcnt++;
                else
                {    
                    overturn.process = 0;
                    overcnt = 0;
                }
                tmotor[TMOTOR_FR].valSet.angle -= overturn.downangdelta[0];
                tmotor[TMOTOR_FL].valSet.angle += overturn.downangdelta[0];
                tmotor[TMOTOR_RR].valSet.angle -= overturn.downangdelta[1];
                tmotor[TMOTOR_RL].valSet.angle += overturn.downangdelta[1];               
            default:break;
        }
        if(sendt)
        {
           sendt = false;
            for(int i=0; i<4; i++)
            {
                TmotorCaculate(i, &tmotor[i]);
                
                if(tmotor[i].enable)
                {
                    if(tmotor[i].begin)
                    {
                        switch(tmotor[i].mode)
                        {
                            case current:Tmotor_ControlPara(i, 0, 0, 0, 0,tmotor[i].valSet.torque);
                                break;
                            case RPM:Tmotor_ControlPara(i, 0,tmotor[i].valSet.speed, 0,tmotor[i].kd,tmotor[i].valSet.torque);
                                break;
                            case position:
                                    Tmotor_ControlPara(i, tmotor[i].valSet.position,tmotor[i].valSet.speed,tmotor[i].kp,tmotor[i].kd,tmotor[i].valSet.torque);     
                                    break;
                            default:Tmotor_ControlPara(i, 0, 0, 0, 0, 0);
                                break;
                        }
                    }
                }
            }
        }
        else
{sendt = true;
        for(int i=4; i<8; i++)
        {
            TmotorCaculate(i, &tmotor[i]);
            
            if(tmotor[i].enable)
            {
                if(tmotor[i].begin)
                {
                    switch(tmotor[i].mode)
                    {
                        case current:Tmotor_ControlPara(i, 0, 0, 0, 0,tmotor[i].valSet.torque);
                            break;
                        case RPM:Tmotor_ControlPara(i, 0,tmotor[i].valSet.speed, 0,tmotor[i].kd,tmotor[i].valSet.torque);
                            break;
                        case position:
                                Tmotor_ControlPara(i, tmotor[i].valSet.position,tmotor[i].valSet.speed,tmotor[i].kp,tmotor[i].kd,tmotor[i].valSet.torque);     
                                break;
                        default:Tmotor_ControlPara(i, 0, 0, 0, 0, 0);
                            break;
                    }
                }
            }
        }

}
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
