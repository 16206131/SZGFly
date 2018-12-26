#include "rc.h"
uint16_t Roll_Celebrate, Pitch_Celebrate, Yaw_Celebrate;
uint16_t Height_Celebrate;
uint16_t sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
void RcExp_Init(void)
{
	uint16_t num;
	
	for (num = 0; num < 10; num++)
	{
		sum1 = sum1 + PWM1_Value;
		sum2 = sum2 + PWM2_Value;
		sum3 = sum3 + PWM3_Value;
		sum4 = sum4 + PWM4_Value;
	}
	Roll_Celebrate = sum1 / num;
	Pitch_Celebrate = sum2 / num;
	Yaw_Celebrate = sum3 / num;
	Height_Celebrate = sum4 / num;
}
#define RC_CELE 2000   
uint32_t RC_Pre;
uint32_t RC_Post;
//��ʼ��;
int RC_PIT_MAX = 0;
int RC_PIT_MIN = 2500;
int RC_ROL_MAX = 0;
int RC_ROL_MIN = 2000;
int RC_YAW_MAX = 0;
int RC_YAW_MIN = 2000;
int RC_HEIGHT_MAX = 0;
int RC_HEIGHT_MIN = 2000;
int RC_PIT_MID = 0;
int RC_ROL_MID = 0;
int RC_YAW_MID = 0;
int RC_HEIGHT_MID = 0;
//ң����У׼����;
void RC_Celebrate(void)
{
	int RC[4];
	//У׼PIT���ֵ;
	LED3_ON;
	LED4_ON;
	LED5_ON;  
	LED6_ON;
	RC_Pre = HAL_GetTick();
	do
	{
		RC[3] = GetRoll_Value();
		RC[1] = GetPitch_Value();
		RC[2] = GetHeight_Value();
		RC[0] = GetYaw_Value();	
		
		
		if (RC_PIT_MAX < RC[1])
			RC_PIT_MAX = RC[1];   //ȡ���ֵ	
		if (RC_PIT_MIN > RC[1])
			RC_PIT_MIN = RC[1];     //ȡ��Сֵ
		
		if (RC_ROL_MAX < RC[0])
			RC_ROL_MAX = RC[0];
		if (RC_ROL_MIN > RC[0])
			RC_ROL_MIN = RC[0];
		
		if (RC_YAW_MAX < RC[3])
			RC_YAW_MAX = RC[3];
		if (RC_YAW_MIN > RC[3])
			RC_YAW_MIN = RC[3];	
		
		if (RC_HEIGHT_MAX < RC[2])
			RC_HEIGHT_MAX = RC[2];
		if (RC_HEIGHT_MIN > RC[2])
			RC_HEIGHT_MIN = RC[2];
		RC_Post = HAL_GetTick();
	} while (RC_Post - RC_Pre <= RC_CELE);   //2.5s��У׼
	RC_PIT_MID = (RC_PIT_MAX + RC_PIT_MIN) / 2;
	RC_ROL_MID = (RC_ROL_MAX + RC_ROL_MIN) / 2;
	RC_YAW_MID = (RC_YAW_MAX + RC_YAW_MIN) / 2;
	RC_HEIGHT_MID = (RC_HEIGHT_MAX + RC_HEIGHT_MIN) / 2;
	//У׼��ϣ��ĸ���ͬʱ��˸;
	ALED_BLink();
}
void GetRcValue(RC_IN *Rc, RC_Exp *Exp)
{
	Rc->P1 = PWM1_Value;
	Rc->P2 = PWM2_Value;
	Rc->P3 = PWM3_Value;
	Rc->P4 = PWM4_Value;
	//	Exp->Roll = PWM4_Value - RC_ROL_MID;  		//ң����У׼
	//	Exp->Pitch = PWM2_Value - RC_PIT_MID;
	//	Exp->Yaw = PWM1_Value - RC_YAW_MID;
	//	Exp->Height = PWM3_Value - RC_HEIGHT_MIN;
	Exp->Roll = -(PWM4_Value - 1519) / 11; //�����б��Ϊ45��,��ʣ��PWMֵ11�ȷ�Ϊ45�ݣ�ÿ�ݴ���1��;  		
	Exp->Pitch = (PWM2_Value - 1525) / 11;
	Exp->Yaw = (PWM1_Value - 1537) / 11;
	Exp->Height = PWM3_Value - 1029;
	
}
void DerictOutput(void)
{
	TIM2->CCR1 = PWM1_Value;
	TIM2->CCR2 = PWM2_Value;
	TIM2->CCR3 = PWM3_Value;
	TIM2->CCR4 = PWM4_Value;
}

/*���� ��������*/
uint8_t Flag_Arm = 0;        //������־Ĭ��Ϊ0������״̬��1Ϊ����״̬��
uint8_t Num_Arm = 1;         //Ĭ��Ϊ1
uint32_t Pre_Arm;
uint32_t Post_Arm;
uint32_t Pre_DisArm;
uint32_t Post_DisArm;


uint8_t Unlock_Lock(RC_IN *Rc)
{
	/*�������--��������*/
	if ((Rc->P2 < 1100)&(Rc->P3 < 1100))
	{
		/*�������*/
		if ((Rc->P1 > 2000)&(Rc->P4 < 1100))  //���ں�
			{
				/*�������ںϵı���ʱ��*/
				if (Num_Arm == 1)     // ��������״̬;
					{
						Post_Arm = HAL_GetTick();
						if (Post_Arm - Pre_Arm >= Arm_Time)
						{
							Flag_Arm = 0;
						}
						
					}
				else if (Num_Arm == 0)		//֮ǰ�Ѿ��������
				{
					Pre_Arm = HAL_GetTick();
					Num_Arm = 1;      //��1����������״̬;
				}
			}
		/*�������*/
		else if((Rc->P1 < 1100)&(Rc->P4 > 2000))  //���⿪
		{
			/*�������⿪�ı���ʱ��*/
			if (Num_Arm == 0)     // �������״̬;
				{
					Post_DisArm = HAL_GetTick();
					if (Post_DisArm - Pre_DisArm >= DisArm_Time)
					{
						Flag_Arm = 1;
					}	
				}
			else if (Num_Arm == 1)  //ȷ��������״̬
				{
					Pre_DisArm = HAL_GetTick();
					Num_Arm = 0;       //�������״̬;
				}
		}
	}	
	return Flag_Arm;
}


///*���У׼����*/
	uint32_t Cele_Pre;
	uint32_t Cele_Post;
	uint8_t Cele_Flag;
void Celebrate(void)
{
	TIM2->CCR1 = PWM3_Value;
	TIM2->CCR2 = PWM3_Value;
	TIM2->CCR3 = PWM3_Value;
	TIM2->CCR4 = PWM3_Value;
}
uint8_t Celebrate_Time(void)
{
	
	Cele_Pre = HAL_GetTick();
	do
	{
		Celebrate();	
		Cele_Post = HAL_GetTick();
	} while (Cele_Post - Cele_Pre <= Cele_Time);
}