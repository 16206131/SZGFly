#include "control.h"

uint16_t Motor[4];
/*控制量输出*/
void Actual(float RolAct, float PitAct, float YawAct, RC_IN Rc)
{
	
	float throttle;
	
	throttle = Rc.P3;
	
	Motor[0] = (uint16_t)(throttle + RolAct + PitAct);
	Motor[1] = (uint16_t)(throttle + RolAct - PitAct);
	Motor[2] = (uint16_t)(throttle - RolAct - PitAct);
	Motor[3] = (uint16_t)(throttle - RolAct + PitAct);
	
	//电机输出限幅;
	if(Motor[0]>Motor_MAX)
		Motor[0] = Motor_MAX;
	else if (Motor[0]<Motor_MIN)
		Motor[0] = Motor_MIN;
	if (Motor[1] > Motor_MAX)
		Motor[1] = Motor_MAX;
	else if (Motor[1] < Motor_MIN)
		Motor[1] = Motor_MIN;
	if (Motor[2] > Motor_MAX)
		Motor[2] = Motor_MAX;
	else if (Motor[2] < Motor_MIN)
		Motor[2] = Motor_MIN;
	if (Motor[3] > Motor_MAX)
		Motor[3] = Motor_MAX;
	else if (Motor[3] < Motor_MIN)
		Motor[3] = Motor_MIN;
	//真正的电机输出;
	TIM2->CCR1 = Motor[0] ;
	TIM2->CCR2 = Motor[1];
	TIM2->CCR3 = Motor[2];
	TIM2->CCR4 = Motor[3];
}
//锁定电机;
void Motor_Lock(void)
{
	TIM2->CCR1 = 1000;
	TIM2->CCR2 = 1000;
	TIM2->CCR3 = 1000;
	TIM2->CCR4 = 1000;
}
//1050以下都不转,怠速模式;
void Wait_Fly(void)
{
	TIM2->CCR1 = 1080;
	TIM2->CCR2 = 1080;
	TIM2->CCR3 = 1080;
	TIM2->CCR4 = 1080;
}


float RolOut, PitOut, YawOut;
float RolError, PitError, YawError;
float RolRateError, PitRateError, YawRateError;
float RolOutRate, PitOutRate, YawOutRate;

void Angle_Control(RC_IN Rc, RC_Exp Exp_angle, T_float_angle angle, pid_struct *pid, short *Gyro)
{
	
	RolError = Exp_angle.Roll - angle.rol;
	RolOut = PID_Compute(&(pid->rol), RolError);
	RolRateError = RolOut - Gyro[0] * 0.06134969325f;
	RolOutRate = PID_Compute(&(pid->rol_rate), RolRateError);
	
	PitError = Exp_angle.Pitch - angle.pit;
	PitOut = PID_Compute(&(pid->pit), PitError);
	PitRateError = PitOut - Gyro[1] * 0.06134969325f;
	PitOutRate = PID_Compute(&(pid->pit_rate), PitRateError);
	
	YawError = Exp_angle.Yaw - angle.yaw;
	YawOut = PID_Compute(&(pid->yaw), YawError);
	YawRateError = YawOut - Gyro[2] * 0.06134969325f;
	YawOutRate = PID_Compute(&(pid->yaw_rate), RolRateError);
	
	Actual(RolOutRate, PitOutRate, YawOutRate, Rc);
}

void AngleRate_Control(RC_IN Rc,RC_Exp Exp_rate, pid_struct *pid, short *Gyro)
{
	
	RolRateError = Exp_rate.Roll - Gyro[0] * 0.06134969325f;
	RolOutRate = PID_Compute(&(pid->rol_rate), RolRateError);
	
	PitRateError = Exp_rate.Pitch - Gyro[1] * 0.06134969325f;
	PitOutRate = PID_Compute(&(pid->pit_rate), PitRateError);
	
	YawRateError = Exp_rate.Yaw - Gyro[2] * 0.06134969325f;
	YawOutRate = PID_Compute(&(pid->yaw_rate), RolRateError);
	
	Actual(RolOutRate, PitOutRate, YawOutRate, Rc);
}




