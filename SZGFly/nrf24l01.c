#include "nrf24l01.h"
 
extern SPI_HandleTypeDef hspi2;
const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 };    //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 };    //���յ�ַ

//���NRF24L01�޸�SPI1����
void NRF24L01_SPI_Init(void)
{
	__HAL_SPI_DISABLE(&hspi2);                   //�ȹر�SPI2
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;      //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;          //����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
	HAL_SPI_Init(&hspi2);
	__HAL_SPI_ENABLE(&hspi2);                    //ʹ��SPI2
}

//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	__HAL_RCC_GPIOB_CLK_ENABLE();  	
	GPIO_InitTypeDef GPIO_Initure;	
    /*CE ��ʼ��*/
	GPIO_Initure.Pin =  GPIO_PIN_0; 	
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;      //�������
    GPIO_Initure.Pull = GPIO_PULLUP;              //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;         //����
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);        //��ʼ��
	
	/*CSN ��ʼ��*/
	GPIO_Initure.Pin = GPIO_PIN_12;  			
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);         //��ʼ��
	/*IRQ ��ʼ��*/
	GPIO_Initure.Pin = GPIO_PIN_1;  	
	GPIO_Initure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);        //��ʼ��
		   		
    NRF24L01_SPI_Init();                       //���NRF���ص��޸�SPI������	
//	SCK(0);
	CE(0);    			                //ʹ��24L01
	CSN(1);     			                //SPIƬѡȡ��	
	
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;
//1��ʧ��	

uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5] = { 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	uint8_t i;
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_8);    //spi�ٶ�Ϊ10.5Mhz����24L01�����SPIʱ��Ϊ10Mhz,�����һ��û��ϵ��    	 
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5);   //д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR, buf, 5);    //����д��ĵ�ַ  
	for(i = 0 ; i < 5 ; i++)
	{
		if (buf[i] != 0XA5)
			break;	
	}
	if (i != 5)
		return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	


//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	
	u8 status;	
	CSN(0);                  //ʹ��SPI����
	status = SPI2_ReadWriteByte(reg);  //���ͼĴ����� 
	SPI2_ReadWriteByte(value);        //д��Ĵ�����ֵ
	CSN(1);                //��ֹSPI����	   
	return(status);         		    //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	CSN(0);               //ʹ��SPI����		
	SPI2_ReadWriteByte(reg);      //���ͼĴ�����
	reg_val = SPI2_ReadWriteByte(0XFF);  //��ȡ�Ĵ�������
	CSN(1);              //��ֹSPI����		    
	return(reg_val);              //����״ֵ̬
}	

void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));    //�ж���Ч��
	__HAL_SPI_DISABLE(&hspi2);                 //�ر�SPI
	hspi2.Instance->CR1 &= 0XFFC7;               //λ3-5���㣬�������ò�����
	hspi2.Instance->CR1 |= SPI_BaudRatePrescaler;     //����SPI�ٶ�
	__HAL_SPI_ENABLE(&hspi2);                  //ʹ��SPI
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;	       
	CSN(0);             //ʹ��SPI����
	status = SPI2_ReadWriteByte(reg);  //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ 
	for(u8_ctr = 0 ; u8_ctr < len ; u8_ctr++)pBuf[u8_ctr] = SPI2_ReadWriteByte(0XFF);  //��������
	CSN(1);           //�ر�SPI����
	return status;               //���ض�����״ֵ̬
}

uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&hspi2, &TxData, &Rxdata, 1, 1000);       
	return Rxdata;          		    //�����յ�������		
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;	   
	CSN(0);               //ʹ��SPI����
  	status = SPI2_ReadWriteByte(reg);  //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr = 0 ; u8_ctr < len ; u8_ctr++)SPI2_ReadWriteByte(*pBuf++);   //д������	 
	CSN(1);             //�ر�SPI����
	return status;                //���ض�����״ֵ̬
}		


//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_32);   //spi�ٶ�Ϊ6.75Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	CE(0);
	NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);  //д���ݵ�TX BUF  32���ֽ�   //�Դ�CSN(0);
	CE(1);                                               //��������	
	delay_us(10);
//	while(READ_IRQ() != 0);                  //�ȴ��������
	sta = NRF24L01_Read_Reg(STATUS);            //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta);   //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)                          //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xff);    //���TX FIFO�Ĵ��� 
		return 2; 
	}
	if (sta&TX_OK)                           //�������
		{
			return 1;
		}
	return 0; 
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_32);   //spi�ٶ�Ϊ6.75Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta = NRF24L01_Read_Reg(STATUS);            //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta);   //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		
		NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH);  //��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);    //���RX FIFO�Ĵ��� 
		return 0;
		
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
	CE(0);	  
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);   //дRX�ڵ��ַ
	  
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x00);         //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);     //ʹ��ͨ��0�Ľ��յ�ַ  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);  	        //����RFͨ��Ƶ��		  
	NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);  //ѡ��ͨ��0����Ч���ݿ�� 	    
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);      //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	CE(1);  //CEΪ��,�������ģʽ 
	//delay_us(200);
	
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	CE(0);	    
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);  //дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);     //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);       //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);   //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x00);  //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);         //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);    //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);      //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CE(1); //CEΪ��,10us����������
	//delay_us(15);
}
void floattou8(float num, unsigned char *buf)
{
	//	unsigned char *p;
	//	p = (unsigned char *)&num;
	//	buf[0] = *p;
	//	buf[1] = *(p + 1);
	//	buf[2] = *(p + 2);
	//	buf[3] = *(p + 3);
//	short temp;
//	uint8_t cnt;
//	temp = (int)(num * 100);
//	buf[cnt++] = BYTE1(temp);
//	buf[cnt++] = BYTE0(temp);
	for(int i = 0 ; i < 4 ; i++)
	{
		buf[i] = ((unsigned char *)&num)[i];
	}
}
