/*
 * startup.c
 *
 *  Created on: Jun 4, 2020
 *      Author: Andrew
 */
#include "stm32f401xc.h"
#include "boardDefs.h"

//I dunno what this does but we need it
void _init(void){
	return;
}

void SystemInit(void){
	//disable WWDG

	//set core voltage
	PWR->CR = (PWR->CR & PWR_CR_VOS_Msk) | (2 << PWR_CR_VOS_Pos);

#ifdef HI_SPEED_CLK
	//setup to have 84Mhz sysclk off of 25Mhz HSE
	//setup for 5 wait states on flash memory
	FLASH->ACR = (FLASH->ACR & ~(FLASH_ACR_LATENCY_Msk)) | (5 << FLASH_ACR_LATENCY_Pos);

	while((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != (5 << FLASH_ACR_LATENCY_Pos)){}

	//enable HSE
	RCC->CR |= RCC_CR_HSEON;
	RCC->CR &= ~RCC_CR_PLLON;
	//wait for stabilize
	while(! (RCC->CR & RCC_CR_HSERDY)){}

	//set PLLM = 25, PLLN = 168, PLLP = 2
	RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC | 25 << RCC_PLLCFGR_PLLM_Pos | 168 << RCC_PLLCFGR_PLLN_Pos;

	RCC->CR |= RCC_CR_PLLON;
	//wait for stabilize
	while(! (RCC->CR & RCC_CR_PLLRDY)){}
	RCC->CFGR = 5 << RCC_CFGR_PPRE1_Pos;
	//switch sysclk
	RCC->CFGR |= 2 << RCC_CFGR_SW_Pos;
	while(! (RCC->CFGR & (2 << RCC_CFGR_SW_Pos))){}
#endif
	//enable FPU
	SCB->CPACR |= (0xF << 20);
	return;
}

