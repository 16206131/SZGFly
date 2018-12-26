#ifndef __HMC_5883L_H
#define __HMC_5883L_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
	
#include "myiic.h"
#include "mpu6050.h"
#include "delay.h"

#define HMC5883_Address 0x1e //0x1e<<1==0x3C //定义器件在 IIC总线的地址
#define HMC5883_Register_A    0x00 
#define HMC5883_Register_B    0x01 
#define HMC5883_ModeReg       0x02  
#define HMC5883_XOUT_H        0x03 
#define HMC5883_XOUT_L        0x04 
#define HMC5883_ZOUT_H        0x05 
#define HMC5883_ZOUT_L        0x06 
#define HMC5883_YOUT_H        0x07 
#define HMC5883_YOUT_L        0x08  
#define HMC5883_StatusReg     0x09  
#define HMC5883_IdentificationReg_A 0x0A 
#define HMC5883_IdentificationReg_B 0x0B 
#define HMC5883_IdentificationReg_C 0x0C 
 
void HMC5883_ReadData(u8 reg_add,unsigned char*Read,u8 num);
 void HMC5883_WriteReg(u8 reg_add,u8 reg_dat);
void GetHMC5883Data(short *hcmData);
 void HMC5883_Init(void);
 uint8_t HMC5883_ReadOneReg(u8 reg_add);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

 #endif  /*__HMC_5883L_H*/
 
  




