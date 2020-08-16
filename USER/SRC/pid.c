#include "pid.h"

void PID_Init(PID_setTypeDef* PID,float KP,float KI,float KD,float KS,s32 Set)
{
	PID->CurVal = 0;
	PID->SetVal = Set;
	PID->liEkVal[0] = 0; 
	PID->liEkVal[1] = 0; 
	PID->liEkVal[2] = 0; 
	PID->uKD_Coe = KD;
	PID->uKP_Coe = KP;
	PID->uKI_Coe = KI;
	PID->uKS_Coe = KS;
}

void PID_Operation(PID_setTypeDef* PID)  
{
	PID->liEkVal[0] = PID->SetVal-PID->CurVal;
	PID->Udlt=PID->uKP_Coe*(PID->liEkVal[0] - PID->liEkVal[1])+PID->uKI_Coe * PID->liEkVal[0]+PID->uKD_Coe*(PID->liEkVal[0] + PID->liEkVal[2]-PID->liEkVal[1] * 2);
	PID->liEkVal[2] = PID->liEkVal[1];
	PID->liEkVal[1] = PID->liEkVal[0];
}
