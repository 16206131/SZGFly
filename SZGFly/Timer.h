#ifndef TIMER_H
#define TIMER_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#include <stm32f4xx_hal.h>
	
#define CR1_CEN_ENABLE 0x0001
#define ARPE_Position 7U
#define CR1_ARPE_ENABLE (1<<ARPE_Position)	
#define CR1_URS_Position 2U
#define URS_SET (1<<CR1_URS_Position)
#define EGR_UG 0x0001
	
	uint32_t CCR1_Value;
	uint32_t CCR2_Value;
	uint32_t CCR3_Value;
	uint32_t CCR4_Value;
	uint16_t CCR1_1, CCR1_2;
	uint16_t CCR2_1, CCR2_2;
	uint16_t CCR3_1, CCR3_2;
	uint16_t CCR4_1, CCR4_2;
	uint16_t PWM1_Value, PWM2_Value, PWM3_Value, PWM4_Value;
	
	void HAL_TIM3_Capture_Init(void);
	
	void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
	
	void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

	void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);
	
	void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
	
	void TIM7_Init(void);
	
	uint32_t GetPitch_Value(void);
	uint32_t GetRoll_Value(void);
	uint32_t GetYaw_Value(void);
	uint32_t GetHeight_Value(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // !LED_H

	
//	TIM_Base_InitTypeDef TIM_InitStructure;
//	TIM_InitStructure.ClockDivision = 0x00000000U;
//	TIM_InitStructure.Prescaler = 31;
//	TIM_InitStructure.CounterMode = 0x00000000U;
//	TIM_InitStructure.Period = 5000;
//	TIM_Base_SetConfig(TIM3, &TIM_InitStructure);
//	TIM_Base_SetConfig(TIM3, &TIM_InitStructure);
//	/*全局中断配置*/
//	 HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(TIM3_IRQn);
//	/*GPIO配置*/
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
//
//	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	/*时基配置*/
//	TIM3->CR1 = 0x0001;   
//	TIM3->ARR = 0xffff;
//	TIM3->PSC = 83;
//	/*输入配置*/
//	TIM3->CCMR1 = 0x0101;
//	TIM3->SMCR = 0x0000;
//	/*中断使能*/
//	TIM3->DIER |= 0x001e;
//	
//	
//	/*输入捕获使能*/
//	TIM3->CCER = 0x1111;