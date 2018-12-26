#include <pid.h>
pid pid_rol, pid_pit, pid_yaw, pid_heght;
pid  pid_rol_rate, pid_pit_rate, pid_yaw_rate, pid_heght_rate;
void PID_init(pid *pid_c, float p, float i, float d) {  

	pid_c->err = 0.0;  
	pid_c->err_last = 0.0;  
	pid_c->integral = 0.0;  
	pid_c->Kp = p;             
	pid_c->Ki = i;             
	pid_c->Kd = d;              
}  

void PID_Struct_Init(pid_struct *pid){
	//在这里设置PID参数;
	PID_init(&pid_rol, 2.8, 0, 0);
	PID_init(&pid_rol_rate, 2.5, 0.05, 40);
	PID_init(&pid_pit, 2.8, 0, 0);
	PID_init(&pid_pit_rate, 2.5, 0.05, 40);
	PID_init(&pid_yaw, 3.8, 0, 0);
	PID_init(&pid_yaw_rate, 1, 0, 0);
	
	pid->rol = pid_rol;
	pid->rol_rate = pid_rol_rate;
	pid->pit = pid_pit;
	pid->pit_rate = pid_pit_rate;
	pid->yaw = pid_yaw;
	pid->yaw_rate = pid_yaw_rate;
}
float error_p, error_i, error_d;
int n;     //用于积分清零的数量，取决于姿态解析的频率;
float PID_Compute(pid *pid_c, float error) {     
	pid_c->err = error;
	
	pid_c->differential = (pid_c->err - pid_c->err_last); 
	/*积分分离*/
//	if (pid_c->differential <= 1)
//	{
//		pid_c->integral += pid_c->err; 
//	}
//	if (pid_c->differential >= -1)
//	{
//		pid_c->integral += pid_c->err; 
//	}
	/*积分限幅*/
	if (pid_c->integral >= INTEGRAL_MAX)
	{
		pid_c->integral = INTEGRAL_MAX;
	}
	if (pid_c->integral <= -INTEGRAL_MAX)
	{
		pid_c->integral = -INTEGRAL_MAX;
	}

	error_p = (pid_c->Kp)*(pid_c->err);
	error_i = (pid_c->Ki)*(pid_c->integral);
	error_d =   (pid_c->Kd)*(pid_c->differential);
	
	pid_c->err_last = pid_c->err;    //保存上一时刻的误差;   
	   
	pid_c->ActualOut = error_p + error_i + error_d;  
	
	return pid_c->ActualOut;                 
}  