#include "delay.h"

TIM_HandleTypeDef htim6;

void MX_TIM6_Init(void)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	htim6.Instance = TIM6;
	htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim6.Init.Prescaler = 83;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 60000;
	
	HAL_TIM_Base_Init(&htim6);
		
}

void delay_ms(uint32_t ticks)
{
	uint16_t CNT;
	TIM6->CNT = 0;    //定时器清零
	TIM6->CR1 |= 0x0001;   //开定时器
	do
	{
		CNT = TIM6->CNT;
	}while ((CNT < ticks * 1000));
	TIM6->CR1 &= 0x0000;   //关定时器

}
void delay_us(uint32_t ticks)
{
	uint16_t CNT;
	TIM6->CNT = 0;     //定时器清零
	TIM6->CR1 |= 0x0001;    //开定时器
	do
	{
		CNT = TIM6->CNT;
	}while ((CNT < ticks ))
		;
	TIM6->CR1 &= 0x0000;    //关定时器
}
