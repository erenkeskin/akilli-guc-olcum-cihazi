/******************************************************
*																											*
*	HASAN EREN KESKIN																		*
*	ELECTRICAL AND ELECTRONICS ENGINEERING STUDENT			*
*	herenkeskin@gmail.com																*
*	http://herenkeskin.com															*
*																											*
*	System Configration Functions												*
*	Versiyon: 0.1 																			*
*																											*
*	V0.2 - 31.08.2018																		*
*																											*
******************************************************/

#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H
	
#include <stm32f4xx.h>

// System Clocks
#define CPU_PLL_CLOCK			168000000		// 168 MHz
#define APB1_CLOCK				42000000		//  42 MHz
#define APB2_CLOCK				84000000		//  84 MHz

void System_clockConfig(void);
void SysTick_init(void);
void delay(uint32_t time);

#endif	// SYSTEM_CONFIG_H
