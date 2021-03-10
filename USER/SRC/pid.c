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
void VESC_PID_Init(VESC_PID_setTypeDef* PID, float KP,float KI,float KD,float KD_Filter)
{
  PID->kp = KP;
  PID->ki = KI;
  PID->kd = KD;
  PID->kd_filter = KD_Filter;
}
void VESC_PID_Operation(VESC_PID_setTypeDef* PID, s32 setval, s32 curval)
{
  PID->SetVal = setval;
  PID->CurVal = curval;
  PID->error = PID->SetVal - PID->CurVal;
  
  // Too low RPM set. Reset state and return.
	if (ABS(PID->SetVal) < 20) {//TODO: ´Ë´¦20ÔÝÂÒ¸ø
		PID->i_term = 0.0f;
		PID->prv_error = PID->error;
		return;
	}
  
  // Compute parameters
  PID->p_term = PID->error * PID->kp / 100.f;
  PID->i_term += PID->error * PID->ki / 100.f;
  PID->d_term = (PID->error - PID->prv_error) * PID->kd / 100.f;
  
  // Filter D
  UTILS_LP_FAST(PID->d_filter, PID->d_term, PID->kd_filter);
	PID->d_term = PID->d_filter;  
  
 	// I-term wind-up protection
  
	utils_truncate_number(&PID->i_term, -1.0f, 1.0f);  
  
  // Store previous error
	PID->prv_error = PID->error;
  
	// Calculate output
	PID->output = PID->p_term + PID->i_term + PID->d_term;
	utils_truncate_number(&PID->output, -1.0f, 1.0f);  
}

void PID_Operation(PID_setTypeDef* PID)  
{
	PID->liEkVal[0] = PID->SetVal-PID->CurVal;
	PID->Udlt=PID->uKP_Coe*(PID->liEkVal[0] - PID->liEkVal[1])+PID->uKI_Coe * PID->liEkVal[0]+PID->uKD_Coe*(PID->liEkVal[0] + PID->liEkVal[2]-PID->liEkVal[1] * 2);
	PID->Udlt*=PID->uKS_Coe;
  PID->liEkVal[2] = PID->liEkVal[1];
	PID->liEkVal[1] = PID->liEkVal[0];
}
