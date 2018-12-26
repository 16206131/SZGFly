#ifndef _IMU_H_
#define _IMU_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <stm32f4xx_hal.h>
#include <math.h>
#include <mpu6050.h>
#include "Timer.h"
	typedef struct T_int16_xyz
	{
		int x;
		int y;
		int z;
	}T_int16_xyz;
	typedef struct T_float_xyz
	{
		float x;
		float y;
		float z;
	}T_float_xyz;
	typedef struct T_float_angle
	{
		float rol;
		float pit;
		float yaw;
	}T_float_angle;
	
	//	void Prepare_Data(T_int16_xyz *acc_in, T_int16_xyz *acc_out);
	
	void Prepare_Data(short *, short *);
	void Celebrate_Angle(void);
	
	void IMUupdate(T_float_xyz *gyr, T_float_xyz *acc, T_float_angle *angle);
	
	T_float_angle GetAngle(short *, short *);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
