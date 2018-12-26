#include "imu.h"
#include "math.h"

#define RtA 		57.324841f				
#define AtR    		0.0174533f				
#define Acc_G 		0.0011963f				
#define Gyro_G 		0.0610351f				
#define Gyro_Gr		0.0010653f			
#define FILTER_NUM 	10
#define CELEBRATE_NUM 20
float 	AngleOffset_Rol = 0.0f, AngleOffset_Pit = 0.0f;

float temAccX = 0.0f, temAccY = 0.0f, temAccZ = 0.0f;
uint8_t Read_Acc[6];
uint8_t Read_Gyro[6];

void Prepare_Data(short *Acc,short *Gyro)
{
	uint8_t num;
	
	MPU6050_ReadData(MPU6050_ACC_OUT, Read_Acc, 6);     //从ACCEL_XOUT_H开始读，连续读6个数据，包括三轴加速度
	Acc[0] = (Read_Acc[0] << 8) | Read_Acc[1];
	Acc[1] = (Read_Acc[2] << 8) | Read_Acc[3];
	Acc[2] = (Read_Acc[4] << 8) | Read_Acc[5];

	MPU6050_ReadData(MPU6050_GYRO_OUT, Read_Gyro, 6);
	Gyro[0] = (Read_Gyro[0] << 8) | Read_Gyro[1];
	Gyro[1] = (Read_Gyro[2] << 8) | Read_Gyro[3];
	Gyro[2] = (Read_Gyro[4] << 8) | Read_Gyro[5];
	
}
////////////////////////////////////////////////////////////////////////////////
#define Kp 1.35f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period???????

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;     // scaled integral error

void IMUupdate(T_float_xyz *gyr, T_float_xyz *acc, T_float_angle *angle)
{
	float ax = acc->x, ay = acc->y, az = acc->z;
	float gx = gyr->x, gy = gyr->y, gz = gyr->z;
	float norm;
	//  float hx, hy, hz, bx, bz;
	float vx, vy, vz;  // wx, wy, wz;
	float ex, ey, ez;

	// 把要用的数据先算出来;不用的注释掉；
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
//	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
//	float q1q2 = q1*q2;
    float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if (ax*ay*az == 0)
		return;
		
	gx *= Gyro_Gr;
	gy *= Gyro_Gr;
	gz *= Gyro_Gr;

	norm = sqrt(ax*ax + ay*ay + az*az);        //acc?????
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;
	
	//	if(norm>16500)
	//	{Rc_C.ARMED=0;}
	
	  // estimated direction of gravity and flux (v and w)              重力的四元数表示  上一次的计算值
	vx = 2*(q1q3 - q0q2); 												//????xyz???
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy);                            					 //计算这次加速度测量的量与上一次四元数姿态解析的量之间的误差;
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	exInt = exInt + ex * Ki; 											 //三个轴误差的积分;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	// adjusted gyroscope measurements									 //用误差的PI值来修正陀螺仪漂移
	gx = gx + Kp*ex + exInt; 					   							
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt; 				   							//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减
	// integrate quaternion rate and normalise						    //一阶龙格塔库法求解四元数微分方程;
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;							//gx,gy,gz都是修正后的值;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;
	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);						
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	
	//将四元数转化为欧拉角;
	angle->yaw += (gyr->z)*Gyro_G * 0.002f;
	
//	angle->yaw = atan2(2*(q1*q2 + q0*q3), q0*q0 + q1*q1 + q2*q2 + q3*q3) * 57.3;
	
    angle->pit = asin(-2 * q1 * q3 + 2 * q0* q2) * 57.295779513 - AngleOffset_Pit;   // pitch
	
	angle->rol = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * 57.295779513 - AngleOffset_Rol;    // roll
	
}

short hmcdata[3];
T_float_xyz acc;
T_float_xyz gyro;

short Prepare_Acc[3] = { 0 }, Prepare_Gyro[3] = { 0 };
T_float_xyz Cele_Gyr, Cele_Acc; 
T_float_angle Cele_Angle;
float tempit = 0, temrol = 0;

void Celebrate_Angle(void)
{
	uint8_t num;
	uint8_t i;
	for (num = 0; num < CELEBRATE_NUM;num++)
	{
		Prepare_Data(Prepare_Acc, Prepare_Gyro);
		Cele_Acc.x = Prepare_Acc[0];
		Cele_Acc.y = Prepare_Acc[1];
		Cele_Acc.z = Prepare_Acc[2];
		Cele_Gyr.x = Prepare_Gyro[0];
		Cele_Gyr.y = Prepare_Gyro[1];
		Cele_Gyr.z = Prepare_Gyro[2];
		
		IMUupdate(&Cele_Gyr, &Cele_Acc, &Cele_Angle);
	
		tempit += Cele_Angle.pit;
		temrol += Cele_Angle.rol;
	}
	AngleOffset_Pit = tempit / CELEBRATE_NUM;
	AngleOffset_Rol = temrol / CELEBRATE_NUM;

}
T_float_angle GetAngle(short *Acc, short *Gyro)
{
	T_float_angle angle;
	uint8_t i;
	//赋值操作;
	acc.x = Acc[0];
	acc.y = Acc[1];
	acc.z = Acc[2];
	gyro.x = Gyro[0];
	gyro.y = Gyro[1];
	gyro.z = Gyro[2];

	IMUupdate(&gyro, &acc, &angle);

	return angle;
}
