#include <Button.h>

void Button_Config(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{

	switch (GPIOx->MODER)	
	{
	case(0x00):__GPIOA_CLK_ENABLE(); break;
	case(0x04):__GPIOB_CLK_ENABLE();break;
	case(0x08):__GPIOC_CLK_ENABLE();break;
	case(0x0C):__GPIOD_CLK_ENABLE(); break;
	case(0x10):__GPIOE_CLK_ENABLE(); break;
	case(0x14):__GPIOF_CLK_ENABLE(); break;
	case(0x18):__GPIOG_CLK_ENABLE(); break;
	case(0x1C):__GPIOH_CLK_ENABLE(); break;
	default:__GPIOI_CLK_ENABLE();	break;
	}

	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
	
	GPIO_InitStructure.Pin = GPIO_Pin;
	
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOx, &GPIO_InitStructure);
	
}