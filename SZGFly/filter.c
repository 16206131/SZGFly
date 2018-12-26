#include <filter.h>
#define AVERAGE_NUM 500
T_float_angle Average_Filter(T_float_angle *angle)
{
	T_float_angle Result_Angle;
	float temrol=0, tempit=0;
	uint16_t num;
	
	for (num = 0; num < AVERAGE_NUM; num++)
	{
		tempit += angle->pit;
		temrol += angle->rol;
	}
	Result_Angle.pit = tempit / AVERAGE_NUM;
	Result_Angle.rol = temrol / AVERAGE_NUM;
	
	return Result_Angle;
	
}


//float Calman_Filter(float newAngle, float newRate, float looptime)
//{
//
//}
void CompensateFilter(short *Acc_in, short *Gyro_in, short *Acc_out, short *Gyro_out)
{
	
}