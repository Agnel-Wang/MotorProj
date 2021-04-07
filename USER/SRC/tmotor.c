#include "tmotor.h"

TmotorTypedef tmotor[8];
TmotorParam AK80instrin;
TmotorLimit Tmotorlimit;

void Tmotorinit()
{
    u8 id = 0;
    //电机类别
    AK80instrin.RATIO = 6.f;
    AK80instrin.GEARRATIO = 1.f;
    
    //电机限制保护设置
    Tmotorlimit.isPosLimitON = false;
    
    /****0号电机初始化****/id = 0;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = ENABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = 0;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****1号电机初始化****/id = 1;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = DISABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = position;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****2号电机初始化****/id = 2;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = DISABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = position;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****3号电机初始化****/id = 3;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = ENABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = 0;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****3号电机初始化****/id = 4;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = ENABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = 0;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****3号电机初始化****/id = 5;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = DISABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = position;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****3号电机初始化****/id = 6;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = DISABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = position;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
    /****3号电机初始化****/id = 7;
    tmotor[id].intrinsic = AK80instrin;
    tmotor[id].enable = ENABLE;
    tmotor[id].begin = ENABLE;
    tmotor[id].mode = 0;
    tmotor[id].valSet.torque = 0.f;
    tmotor[id].valSet.speed = 0.f;
    tmotor[id].valSet.angle = 0.f;
    tmotor[id].kp = 100.f;
    tmotor[id].kd = 1.f;
    tmotor[id].limit = Tmotorlimit;
    
#if USE_TMOTOR
    for(int i=0; i<8; i++)
    {
        Tmotor_ControlCmd(i, CMD_MOTOR_MODE);
        Delay_ms(5);
    }
#endif 
}

void TmotorCaculate(u8 id, TmotorTypedef* motor)
{
    motor->valSet.position = motor->valSet.angle*motor->intrinsic.RATIO *PI/180.f;
    if(motor->status.isSetZero)
    {
        motor->status.isSetZero = false;
        motor->valReal.position=0;
        Tmotor_ControlCmd(id, CMD_ZERO_POSITION);
    }
}

u8 buf[8];
void Tmotor_ControlPara(u8 id, float f_p, float f_v, float f_kp, float f_kd, float f_t)
{
    u16 p, v, kp, kd, t;

        
    /* 根据协议，对float参数进行转换 */
    p = float2uint(f_p,      P_MIN,  P_MAX,  16);            
    v = float2uint(f_v,      V_MIN,  V_MAX,  12);
    kp = float2uint(f_kp,    KP_MIN, KP_MAX, 12);
    kd = float2uint(f_kd,    KD_MIN, KD_MAX, 12);
    t = float2uint(f_t,      T_MIN,  T_MAX,  12);
    
    /* 根据传输协议，把数据转换为CAN命令数据字段 */
    buf[0] = p>>8;
    buf[1] = p&0xFF;
    buf[2] = v>>4;
    buf[3] = ((v&0xF)<<4)|(kp>>8);
    buf[4] = kp&0xFF;
    buf[5] = kd>>4;
    buf[6] = ((kd&0xF)<<4)|(t>>8);
    buf[7] = t&0xff;
    
    /* 通过CAN接口把buf中的内容发送出去 */
    CanTransmit(id, buf, sizeof(buf));
}


void Tmotor_ControlCmd(u8 id, u8 cmd)
{
    uint8_t buf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
    switch(cmd)
    {
        case CMD_MOTOR_MODE:
            buf[7] = 0xFC;
            break;
        
        case CMD_RESET_MODE:
            buf[7] = 0xFD;
        break;
        
        case CMD_ZERO_POSITION:
            buf[7] = 0xFE;
        break;

        default:
        return; /* 直接退出函数 */
    }
    CanTransmit(id, buf, sizeof(buf));
}


void CanTransmit(u8 id, u8 *buf, uint8_t len)
{
	int i;
	CanTxMsg tx_message;
	tx_message.StdId = id+1;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.DLC = len;

	for(i=0;i<len;i++)
		tx_message.Data[i] = buf[i];

		CAN_Transmit(CAN2,&tx_message);
}

void Rcv_motor(CanRxMsg rx_message, TmotorTypedef* tmotor)
{
	u16 p_int = (rx_message.Data[1]<<8)|rx_message.Data[2];
	u16 v_int = (rx_message.Data[3]<<4)|(rx_message.Data[4]>>4);
	u16 i_int = (rx_message.Data[4]&0xF)<<8|rx_message.Data[5];
	
	tmotor->valReal.position = uint2float(p_int, P_MIN,  P_MAX,  16);
	tmotor->valReal.speed = uint2float(v_int, V_MIN,  V_MAX,  12);
	tmotor->valReal.torque = uint2float(i_int, T_MIN,  T_MAX,  12);
	tmotor->valReal.angle = 180.f * tmotor->valReal.position/PI/(tmotor->intrinsic.RATIO * tmotor->intrinsic.GEARRATIO);
}


