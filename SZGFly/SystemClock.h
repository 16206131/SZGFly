#ifndef SYSTEMCLOCK_H
#define SYSTEMCLOCK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
	
#define SysTick_CTRL_CLKSOURCE_DisMsk         (0UL << SysTick_CTRL_CLKSOURCE_Pos)            /*!< SysTick CTRL: CLKSOURCE DisMask */	

	void setSystemClock(void);
	//SystemCoreClockUpdate();    use this after setSystemClock to update the SystemCoreClock;
    uint32_t SysTickInit(uint32_t Ticks);  //Init by SysTick_CTRL_CLKSOURCE_DisMsk  in 8ms timer basic;
	uint32_t SystemClock;
	uint32_t HCLKClock;
	uint32_t APB1Clock;
	uint32_t APB2Clock;
	void GetSystemHclkAPB12Clock(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // !LED_H
