#include <led.h>
//����GPIOģʽ�����ţ�Ĭ�Ͽ�GPIODʱ��;
void LED_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)  
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_Pin;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStructure);
}
//���е���˸����;
void ALED_BLink(void)
{
	LED4_ON;
	LED3_ON;
	LED5_ON;
	LED6_ON;
	Hal_Delay(500);
	LED4_OFF;
	LED3_OFF;
	LED5_OFF;
	LED6_OFF;
	Hal_Delay(500);
	LED4_ON;
	LED3_ON;
	LED5_ON;
	LED6_ON;
	Hal_Delay(500);
	LED4_OFF;
	LED3_OFF;
	LED5_OFF;
	LED6_OFF;
	Hal_Delay(500);
}