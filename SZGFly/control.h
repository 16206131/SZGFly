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
	
#define Motor_MAX 2000      //������ת��
#define Motor_MIN 1080      //������ת��

	
	
	void Actual(float PitAct, float RolAct, float YawAct, RC_IN Rc);
	void Motor_Lock(void);
	void Wait_Fly(void);    //����ģʽ;
	/*��ң������������Ϊ�����Ƕȣ�ʹ�ô���PID������̬*/
	void Angle_Control(RC_IN Rc, RC_Exp Exp_angle, T_float_angle angle, pid_struct *pid, short *Gyro);     		//��̬ģʽ;
	
	/*��ң������������Ϊ�������ٶȣ�ֱ��ʹ���ڻ�������PID����*/
	void AngleRate_Control(RC_IN Rc, RC_Exp Exp_rate, pid_struct *pid, short *Gyro);    //������ģʽ;
	
#ifdef __cplusplus
}

#endif /* __cplusplus */

#endif 