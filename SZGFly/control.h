#ifndef CONTROL_H
#define CONTROL_H
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include "Timer.h"
#include "mpu6050.h"
#include "imu.h"
//#include "imu2.h"
#include "rc.h"
#include "pid.h"
	
#define Motor_MAX 2000      //电机最高转速
#define Motor_MIN 1080      //电机最低转速

	
	
	void Actual(float PitAct, float RolAct, float YawAct, RC_IN Rc);
	void Motor_Lock(void);
	void Wait_Fly(void);    //怠速模式;
	/*将遥控器的输入作为期望角度，使用串级PID控制姿态*/
	void Angle_Control(RC_IN Rc, RC_Exp Exp_angle, T_float_angle angle, pid_struct *pid, short *Gyro);     		//姿态模式;
	
	/*将遥控器的输入作为期望角速度，直接使用内环角速率PID控制*/
	void AngleRate_Control(RC_IN Rc, RC_Exp Exp_rate, pid_struct *pid, short *Gyro);    //角速率模式;
	
#ifdef __cplusplus
}

#endif /* __cplusplus */

#endif 