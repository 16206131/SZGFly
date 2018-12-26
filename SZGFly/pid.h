#ifndef PID_H
#define PID_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
#include <cbmath.h>
	
#define INTEGRAL_MAX 200   //积分最大值；
	
	typedef struct {          
		float ActualOut;        
		float err;                
		float err_last;           
		float Kp, Ki, Kd;                   
		float integral; 
		float differential;
	}pid;
	typedef struct {
		pid rol;
		pid rol_rate;
		pid pit;
		pid pit_rate;
		pid yaw;
		pid yaw_rate;
	}pid_struct;
	void PID_init(pid *pid, float p, float i, float d);
	void PID_Struct_Init(pid_struct *pid);   //调用完PID_init()之后自动调用PID_Struct_Init();
	float PID_Compute(pid *pid_c, float error);
		
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // 
