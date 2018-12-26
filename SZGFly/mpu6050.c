#include <mpu6050.h>
#include <delay.h>
#include <myiic.h>
int r3 = 10;

void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{   
	IIC_Start();
	IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
	r3 = IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{  
	unsigned char i;
	
	IIC_Start();
	IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(MPU6050_SLAVE_ADDRESS+1);
	IIC_Wait_Ack();
	
	for(i=0;i<(num-1);i++){
		*Read=IIC_Read_Byte(1);
		Read++;
	}
	*Read=IIC_Read_Byte(0);
	IIC_Stop();
}

void MPU6050_Init(void)
{
  int i=0,j=0;
	//在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
	  for(i=0;i<1000;i++)
	  {
	    for(j=0;j<1000;j++)
	    {
	      ;
	    }
	  }
//		delay_ms(60);
	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在2G模式，不自检
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	//MPU6050_WriteReg(MPU6050_DLPF_CFG ,0X82);
//	  PMU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);     
//    PMU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);     
//    PMU6050_WriteReg(MPU6050_RA_CONFIG , 0x06); 
//    PMU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x08);   
//    PMU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);   
 
  
}
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	return Re;
}

void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
	  
}

void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
	  
}

void MPU6050_ReadAccGyroU8(uint8_t *data)
{
	
}

void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}



void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}
