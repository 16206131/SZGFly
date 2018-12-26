#ifndef MATH_H
#define MATH_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
	/*符号函数，x>0返回1，x=0返回0，x<0返回-1*/
	int sign(float x);
	/*绝对值函数*/
	float cb_abs(float x);
	/*开方函数,输入数若小于0，错误，则返回-1*/
	double cb_sqrt(double x);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // !LED_H
