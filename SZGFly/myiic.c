#include "myiic.h"
#include "delay.h"

int r1, r2;
//IIC��ʼ��
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    GPIO_Initure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //���١�
    HAL_GPIO_Init(GPIO_PORT_I2C, &GPIO_Initure);
    
    
     I2C_SDA_1();
	 I2C_SCL_1(); 
	
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
    I2C_SDA_1();
	I2C_SCL_1();  
	delay_us(4);
	I2C_SDA_0();
	//BSRR = GPIOB->BSRR; //START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL_0();//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	I2C_SCL_0();
	I2C_SDA_0();//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2C_SCL_1(); 
	delay_us(4);
	I2C_SDA_1();//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	      //SDA����Ϊ����  
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
	I2C_SCL_0();//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
void IIC_NAck(void)
{    SDA_OUT();
	I2C_SCL_0();
	
	I2C_SDA_1();
	delay_us(4);
	I2C_SCL_1();
	delay_us(4);
	I2C_SCL_0();
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
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
		_ucByte <<= 1; /* ����һ��bit */
		delay_us(4);
		
		I2C_SCL_1();
		delay_us(4);	
		I2C_SCL_0();
		delay_us(4);
		if (i == 7)
		{
			I2C_SDA_1();  // �ͷ�����
		}
		
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	uint8_t i;
	unsigned char value;

	/* ������1��bitΪ���ݵ�bit7 */
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return value;
}


