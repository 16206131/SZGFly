#ifndef IMU2_H_
#define IMU2_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <stm32f4xx_hal.h>
#include <math.h>
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
	void AHRSupdate(T_float_xyz *, T_float_xyz *, T_float_xyz *, T_float_angle *angle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
