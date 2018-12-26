#include "myiic.h"
#include "delay.h"

int r1, r2;
//IIC初始化
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    GPIO_Initure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速‘
    HAL_GPIO_Init(GPIO_PORT_I2C, &GPIO_Initure);
    
    
     I2C_SDA_1();
	 I2C_SCL_1(); 
	
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
    I2C_SDA_1();
	I2C_SCL_1();  
	delay_us(4);
	I2C_SDA_0();
	//BSRR = GPIOB->BSRR; //START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL_0();//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	I2C_SCL_0();
	I2C_SDA_0();//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2C_SCL_1(); 
	delay_us(4);
	I2C_SDA_1();//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	      //SDA设置为输入  
	I2C_SDA_1();
	SDA_IN(); 
	delay_us(4);	   
	I2C_SCL_1();
	delay_us(10);
	
	while (I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
		delay_us(10);
	}
	I2C_SCL_0();//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	
	SDA_OUT();
	I2C_SCL_0();
	I2C_SDA_0();
	delay_us(4);
	I2C_SCL_1();
	delay_us(4);
	I2C_SCL_0();
}
//不产生ACK应答		    
void IIC_NAck(void)
{    SDA_OUT();
	I2C_SCL_0();
	
	I2C_SDA_1();
	delay_us(4);
	I2C_SCL_1();
	delay_us(4);
	I2C_SCL_0();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 _ucByte)
{                        
    u8 i;   
	SDA_OUT();
	I2C_SCL_0();
	//delay_us(10);
	
	for (i = 0; i < 8; i++)
	{		
		if ((_ucByte & 0x80)==0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		_ucByte <<= 1; /* 左移一个bit */
		delay_us(4);
		
		I2C_SCL_1();
		delay_us(4);	
		I2C_SCL_0();
		delay_us(4);
		if (i == 7)
		{
			I2C_SDA_1();  // 释放总线
		}
		
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	uint8_t i;
	unsigned char value;

	/* 读到第1个bit为数据的bit7 */
	value = 0x00;
	SDA_IN();
	for (i = 0; i < 8; i++)
	{
		
		I2C_SCL_1();
		delay_us(4);
		value <<= 1;
		if (I2C_SDA_READ())
		{
			value+=0X01;
			delay_us(4);
		}
		
	I2C_SCL_0();
	delay_us(4);
	}	 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return value;
}


