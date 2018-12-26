#include <imu2.h>
#define RtA 		57.324841f				
#define AtR    		0.0174533f				
#define Acc_G 		0.0011963f				
#define Gyro_G 		0.0610351f				
#define Gyro_Gr		0.0010653f			
#define FILTER_NUM 	20

#define Kp 10.00f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f     
#define halfT 0.001f
#define ACCEL_1G 1000//???  1000mg
static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
static float exInt = 0, eyInt = 0, ezInt = 0;

//unit: radian

 

 
//unit: milli-Gauss                                                                                                                                                                                          

void AHRSupdate(T_float_xyz *gyro, T_float_xyz *acc, T_float_xyz *hmc, T_float_angle *angle) {

	float norm;  
	float hx, hy, hz, bx, bz; 
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;
 
	 
	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;
	float gx, gy, gz;
	float ax, ay, az;
	float mx, my, mz;
	 
	gx = gyro->x; gy = gyro->y; gz = gyro->z;
	ax = acc->x;ay = acc->y;az = acc->z;
	mx = hmc->x; my = hmc->y; mz = hmc->z;
	 
	gx *= Gyro_Gr;
	gy *= Gyro_Gr;
	gz *= Gyro_Gr;  
	
	//加速度和磁罗盘数据单位化;
	norm = sqrt(ax*ax + ay * ay + az * az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;
	
	norm = sqrt(mx*mx + my * my + mz * mz);
	mx = mx / norm;
	my = my / norm;
	mz = mz / norm;

 

	hx = 2 * mx*(0.5 - q2q2 - q3q3) + 2 * my*(q1q2 - q0q3) + 2 * mz*(q1q3 + q0q2);
	hy = 2 * mx*(q1q2 + q0q3) + 2 * my*(0.5 - q1q1 - q3q3) + 2 * mz*(q2q3 - q0q1);
	hz = 2 * mx*(q1q3 - q0q2) + 2 * my*(q2q3 + q0q1) + 2 * mz*(0.5 - q1q1 - q2q2);
	
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz;

 

	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2 * bx*(0.5 - q2q2 - q3q3) + 2 * bz*(q1q3 - q0q2);
	wy = 2 * bx*(q1q2 - q0q3) + 2 * bz*(q0q1 + q2q3);
	wz = 2 * bx*(q0q2 + q1q3) + 2 * bz*(0.5 - q1q1 - q2q2);

	 
	ex = (ay*vz - az * vy) + (my*wz - mz * wy);
	ey = (az*vx - ax * vz) + (mz*wx - mx * wz);
	ez = (ax*vy - ay * vx) + (mx*wy - my * wx);
    
 

	exInt = exInt + ex * Ki* (1.0f / 2*halfT);
	eyInt = eyInt + ey * Ki* (1.0f / 2*halfT);
	ezInt = ezInt + ez * Ki* (1.0f / 2*halfT);
	// adjusted gyroscope measurements ?????????

	gx = gx + Kp * ex + exInt;
	gy = gy + Kp * ey + eyInt;
	gz = gz + Kp * ez + ezInt;
   
	 

	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz)*halfT;
	q1 = q1 + (q0*gx + q2 * gz - q3 * gy)*halfT;
	q2 = q2 + (q0*gy - q1 * gz + q3 * gx)*halfT;
	q3 = q3 + (q0*gz + q1 * gy - q2 * gx)*halfT;

	
	norm = sqrt(q0*q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	
	//a = 2*(q1*q2 + q0*q3);b = q0*q0 + q1*q1 + q2*q2 + q3*q3;
	angle->yaw = atan2(2*(q1*q2 + q0*q3), q0*q0 + q1*q1 + q2*q2 + q3*q3) * 57.3;
	
	angle->pit = asin(-2 * q1 * q3 + 2 * q0* q2) * 57.2957795 - 0; 
	angle->rol  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * 57.2957795;
	    
}
