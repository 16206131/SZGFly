#include <Timer.h>

TIM_HandleTypeDef htim7;

void HAL_TIM3_Capture_Init(void)
{
	
}
/**TIM3 GPIO Configuration    
		PA6     ------> TIM3_CH1
		PA7     ------> TIM3_CH2
		PB0     ------> TIM3_CH3
		PB1     ------> TIM3_CH4 
		*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (htim_base->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
	else if (htim_base->Instance == TIM3)
	{
		    __HAL_RCC_TIM3_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* TIM3 interrupt Init */
		HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}

}
/**TIM2 GPIO Configuration    
		  PA1     ------> TIM2_CH2
		  PA2     ------> TIM2_CH3
		  PA5     ------> TIM2_CH1
		  PB11     ------> TIM2_CH4 
		  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (htim->Instance == TIM2)
	{
		  
		GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}

}
/**TIM3 GPIO Configuration    
		PA6     ------> TIM3_CH1
		PA7     ------> TIM3_CH2
		PB0     ------> TIM3_CH3
		PB1     ------> TIM3_CH4 
		*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

	if (htim_base->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_DISABLE();

	}
	else if (htim_base->Instance == TIM3)
	{
	
		__HAL_RCC_TIM3_CLK_DISABLE();
  
		
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);

		/* TIM3 interrupt DeInit */
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		
	}

}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		CCR1_Value = TIM3->CCR1;
		if (GPIOA->IDR&GPIO_PIN_6)
		{
			CCR1_1 = CCR1_Value;             //上升沿CCR1的值;
			TIM3->CCER |= (1 << 1);          //转化为低电平检测;写1;
		}
		else
		{
			CCR1_2 = CCR1_Value;   		//下降沿CCR1的值;
			TIM3->CCER &= ~(1 << 1);       //转化为高电平检测;写0;
			if(CCR1_2 > CCR1_1)			//大于直接相减取值;
				PWM1_Value = CCR1_2 - CCR1_1;   
			else						//小于取互补值;
				PWM1_Value = 0xffff - CCR1_1 + CCR1_2;
		}
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		CCR2_Value = TIM3->CCR2;
		if (GPIOA->IDR&GPIO_PIN_7)        //高电平时间;
			{
				CCR2_1 = CCR2_Value;
				TIM3->CCER |= (1 << 5);    		//转化为低电平检测;
			}
		else
		{
			CCR2_2 = CCR2_Value;
			TIM3->CCER &= ~(1 << 5);  			//转化为高电平检测;
			if(CCR2_2 > CCR2_1)
			{
				PWM2_Value = CCR2_2 - CCR2_1;
			}
			else
			{
				PWM2_Value = 0xffff - CCR2_1 + CCR2_2;
			}
		}
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		CCR3_Value = TIM3->CCR3;
		if (GPIOB->IDR&GPIO_PIN_0)
		{
			CCR3_1 = CCR3_Value;
			TIM3->CCER |= (1 << 9);  				 //转化为低电平检测;
		}
		else
		{
			CCR3_2 = CCR3_Value;
			TIM3->CCER &= ~(1 << 9);				//转化为高电平检测;
			if(CCR3_2 > CCR3_1)
			{
				PWM3_Value = CCR3_2 - CCR3_1;
			}
			else
			{
				PWM3_Value = 0xffff - CCR3_1 + CCR3_2;
			}
		}
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		CCR4_Value = TIM3->CCR4;
		if (GPIOB->IDR&GPIO_PIN_1)
		{
			CCR4_1 = CCR4_Value;
			TIM3->CCER |= (1 << 13);     //转化为低电平检测;
		}
		else
		{
			CCR4_2 = CCR4_Value;
			TIM3->CCER &= ~(1 << 13); //转化为高电平检测;
			if (CCR4_2 > CCR4_1)
			{
				PWM4_Value = CCR4_2 - CCR4_1;
			}
			else
			{
				PWM4_Value = 0xffff - CCR4_1 + CCR4_2;
			}
		}
	}
}


void TIM7_Init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
	htim7.Instance = TIM7;
	htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim7.Init.Prescaler = 83;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 60000;
	
	HAL_TIM_Base_Init(&htim7);	
}
uint32_t GetPitch_Value(void)
{
	return PWM2_Value;
}
uint32_t GetRoll_Value(void)
{
	return PWM1_Value;
}
uint32_t GetYaw_Value(void)
{
	return PWM4_Value;
}
uint32_t GetHeight_Value(void)
{
	return PWM3_Value;
}