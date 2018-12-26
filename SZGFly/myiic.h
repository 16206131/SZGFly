#ifndef _MYIIC_H
#define _MYIIC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include "delay.h"
#include <stm32f4xx_hal.h>
	
#define u8 uint8_t
#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
//#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_PIN_8			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_PIN_9	  	/* 连接到SDA数据线的GPIO */
#define I2C_SDA_MODE_NUM 9					//num is eqaul to the number of  I2C_SDA_PIN (GPIO_PIN_9) 
#define SDA_IN()  {GPIOB->MODER&=~(3<<(I2C_SDA_MODE_NUM*2));GPIOB->MODER|=0<<I2C_SDA_MODE_NUM*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(I2C_SDA_MODE_NUM*2));GPIOB->MODER|=1<<I2C_SDA_MODE_NUM*2;} //PB9输出模式
//IO操作
	
#define I2C_SCL_1()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				    /* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);//GPIO_PORT_I2C->BSRR = (uint32_t)0x01000000   //= ((uint32_t)I2C_SCL_PIN)<<16U;				/* SCL = 0 */
	
#define I2C_SDA_1()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);//GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);//GPIO_PORT_I2C->BSRR = (uint32_t)0x02000000 //((uint32_t)I2C_SCL_PIN)<<16U;				/* SDA = 0 */
	
#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */


//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

