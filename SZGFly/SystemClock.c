#include <SystemClock.h>

/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/*            System clock source = HSE / PLL_M *  PLL_N / PLL_P              */
/******************************************************************************/
void setSystemClock(void)
{
#ifndef HSE_STARTUP_TIMEOUT
	uint32_t HSE_STARTUP_TIMEOUT = 1000;
#endif // !HSE_STARTUP_TIMEOUT

	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	uint32_t	PLL_M = 25,
				PLL_N = 336,
				PLL_P = 2,
				PLL_Q = 7,
				PLLI2S_N = 302,
				PLLI2S_R = 2;
	
	/* Enable HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while (HSEStatus == 0);// && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		HSEStatus = (uint32_t)0x01;
	}
	else
	{
		HSEStatus = (uint32_t)0x00;
	}

	if (HSEStatus == (uint32_t)0x01)
	{
		/* Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz */
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
		//		if ((PWR->CR &PWR_CSR_VOSRDY) == 1)	
		//		{
					/* HCLK = SYSCLK / 1*/ 
					RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    //168MHz
      
					/* PCLK2 = HCLK / 2*/
					RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;    //84MHz
    
					/* PCLK1 = HCLK / 4*/
					RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;    //42MHz

					    /* Configure the main PLL */
				RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) |
				               (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
		
		RCC->PLLI2SCFGR = (PLLI2S_R << 28) + (PLLI2S_N << 6);

		/* Enable the main PLL and PLLI2S*/
		RCC->CR |= RCC_CR_PLLON | RCC_CR_PLLI2SON;

		/* Wait till the main PLL is ready */
		while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}
   
		/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
		FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

		/* Select the main PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		/* Wait till the main PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
			;
		{
		}
	}
		
	//	}
		else
	{
		/* If HSE fails to start-up, the application will have wrong clock
	       configuration. User can add here some code to deal with this error */
	}

}

 uint32_t SysTickInit(uint32_t Ticks)
{
	if ((Ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)           //超出滴答定时器的范围;          
	{
		return (1UL);                                                   /* Reload value impossible */
	}

	SysTick->LOAD  = (uint32_t)(Ticks - 1UL); /* set reload register */
	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	SysTick->VAL   = 0UL; /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |                  //AHB
	                 SysTick_CTRL_TICKINT_Msk   |					  //Enable SysTick IRQ      
	                 SysTick_CTRL_ENABLE_Msk;						// Enable  SysTick Timer 
	return (0UL);                                                     /* Function successful */
}
void GetSystemHclkAPB12Clock(void)
{
	SystemClock = HAL_RCC_GetSysClockFreq();
	HCLKClock = HAL_RCC_GetHCLKFreq();
	APB1Clock = HAL_RCC_GetPCLK1Freq();
	APB2Clock = HAL_RCC_GetPCLK2Freq();
}
