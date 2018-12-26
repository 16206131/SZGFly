#ifndef LED_H
#define LED_H
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	


#define LED4_ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET)
#define LED4_OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET)

#define LED3_ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define LED3_OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)

#define LED5_ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define LED5_OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)

#define LED6_ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define LED6_OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)

void LED_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
	void LED3_Blink(void);
	void LED6_Blink(void);
	void ALED_BLink(void);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // !LED_H