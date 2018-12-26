#ifndef BUTTON_H
#define BUTTON_H


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

void Button_Config(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);

#endif
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */