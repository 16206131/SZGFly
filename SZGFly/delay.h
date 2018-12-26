#ifndef DELAY_H
#define DELAY_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include "stm32f4xx_hal.h"
	void MX_TIM6_Init(void);
	void delay_ms(uint32_t ticks);
	void delay_us(uint32_t ticks);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 