#ifndef MATH_H
#define MATH_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
	/*���ź�����x>0����1��x=0����0��x<0����-1*/
	int sign(float x);
	/*����ֵ����*/
	float cb_abs(float x);
	/*��������,��������С��0�������򷵻�-1*/
	double cb_sqrt(double x);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // !LED_H
