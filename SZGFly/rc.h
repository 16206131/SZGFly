#ifndef RC_H
#define RC_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
#include "led.h"
#include "Timer.h"
#include "delay.h"
	
#define Arm_Time 1000        
#define DisArm_Time 2000     //解锁时间，单位为1毫秒；	
#define Cele_Time  10000     //在上电的10秒钟内为电机校准时间
	typedef struct
	{
		int P1;
		int P2;
		int P3;
		int P4;
	}RC_IN;
	typedef struct
	{
		int Roll;
		int Pitch;
		int Yaw;
		int Height;
	}RC_Exp;
	typedef struct
	{
		int *pre;
		uint32_t RC_Value;
		int *next;
	}RC_LINK;		//接收遥控器数据链表;
	void GetRcValue(RC_IN *Rc, RC_Exp *Exp);
	void DerictOutput(void);
	
	uint8_t Unlock_Lock(RC_IN *);
	uint8_t Celebrate_Time(void);
	void RcExp_Init(void);
	void RC_Celebrate(void);
	void Celebrate(void);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 