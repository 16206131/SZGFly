#ifndef FILTER_H
#define FILTER_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <imu.h>
	
	T_float_angle Average_Filter(T_float_angle *);
//	float Calman_Filter(float newAngle, float newRate, float looptime);
	T_float_angle HighFrequency_Filter(T_float_angle *);
	void CompensateFilter(short *Acc_in, short *Gyro_in, short *Acc_out, short *Gyro_out);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 
