#include "System_Config.h"
#include "USART.h"

extern circularBuffer outputBuffer;
extern circularBuffer inputBuffer;

/*
	(Reference Manual 7.2 - Page: 218/1749)
	The timer clock frequencies are automatically set by hardware. There are two cases:
		1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
		that of the APB domain to which the timers are connected.
		2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
		timers are connected.

	APB1 Bus max 42 MHz
	Timer7 Clock max 84 MHz
*/
void TIM7_init(void)
{
	RCC->APB1ENR 	|= (1 << 5);    // Activate Timer 7 Clock (42 Mhz * 2)

	TIM7->CR1 		 = 0;           // Clear Control Register
	TIM7->CR1 		|= (1 << 7);    // Auto-Reloaded Selected

	/*
		Period = 1 / Frequency
		t = (1 / f)
		fCK_PSC / ( (PRESCALER + 1) * (AUTO-RELOADED-VALUE + 1) ) = 1 Hz

		This calculation gives us to 1 second timer interrupt
	*/
	// 1 us
	TIM7->PSC 		 = 41;
	TIM7->ARR 		 = 19;

	TIM7->DIER 		 = 0;			// Clear DMA/Interrupt Enable Register
	TIM7->DIER 		|= (1 << 8);	// UDE: Update DMA request enable
	TIM7->DIER 		|= (1 << 0);	// UIE: Update interrupt enable

	NVIC->ISER[1]  |= (1 << 23);  	// Activate Timer7 Global Interrupt in NVIC

	TIM7->CR1 		|= (1 << 0);	// Counter Enable
}

void TIM7_IRQHandler(void)
{
	TIM7->SR = 0;
}

void TIM2_init(void)
{
	// Timer 2 set-up
	RCC->APB1ENR |= (1 << 0);		// Enable clock for Timer 2
	
	// Update Event (Hz) = 84MHz / ((PSC + 1) * (ARR + 1)) = x Hz
	TIM2->ARR     = 420;         	// Auto Reload value: 8400 == 100us
	//TIM2->PSC 		 = 41;
	//TIM2->ARR 		 = 19;
	TIM2->CR2    |= (1 << 5);      	// Select TRGO to be update event (UE)
	TIM2->CR1    |= (1 << 0);      	// Enable Counting
}

/*
 *	PC5 using
 */
void ADC_init(void)
{
	RCC->AHB1ENR 	|= (1 <<  2);	// GPIO C clock enable
	RCC->APB2ENR	|= (1 <<  8);	// ADC1 Clock Active
	RCC->APB2ENR	|= (1 <<  9);	// ADC2 Clock Active

	// ADC set-up
	ADC1->CR1			&= ~(3 << 24);     	// Resolution 12 Bit (15 ADCCLK Cycles) 
	ADC1->SMPR1 	&= ~(7 << 15);			// Sampling Time 3 Cycles
	ADC->CCR      |= (6 << 0);   			// Regular simultaneous mode only
	
	ADC1->CR2      = 0;
	ADC1->CR2     |= (1 << 0);				// ADC1 On
	ADC1->SQR3 		 = 0;
	ADC1->SQR3 		|= (15 << 0);				// PC5 pass first conversion
	
	ADC2->CR2      = 0;
	ADC2->CR2     |= (1 << 0);      	// ADC2 ON
	ADC2->SQR3 		 = 0;
  ADC2->SQR3    |= (14 << 0);      	// PC4 pass first conversion
	
	GPIOC->MODER 	|= ((3 << 8) | (3 << 10));		// PC4, PC5 Select Analog Mode

	ADC1->CR2    |= (6 << 24);   			// Use TIM2, TRG0 as external event
	ADC1->CR2    |= (1 << 28);   			// Enable external trigger, rising edge
	ADC1->CR1    |= (1 << 5);   			// Enable ADC Interrupt for EOC

	//NVIC_EnableIRQ(ADC_IRQn);     	// Enable IRQ for ADC in NVIC
	NVIC->ISER[0]  |= (1 << 18);			// Enable IRQ for ADC in NVIC

}


uint16_t ADC_Read(void)
{

	uint16_t deger = 0;

	ADC1->CR2 		|= (1 << 30);	// Start conversion of regular channels
	while(!(ADC1->SR & 0x00000002));// EOC - Conversion Complete

	deger = ADC1->DR;				// ADC Data Register

	return deger;
}


uint16_t i = 0;
uint16_t j = 0;
uint16_t k = 0;
uint16_t a = 0;
uint8_t dataGonder = 0;
float voltageValue = 0;
uint16_t ADCdata[1000];
uint16_t ADCdata2[1000];
uint16_t ADCstring[1000];
uint16_t ADCstring2[1000];
uint16_t UARTstring[1000];
uint16_t UARTstring2[1000];
#define ARRAY_SIZE	250

void ADC_IRQHandler(void)
{
	ADCdata2[j++] = ADC2->DR;
	ADCdata[i++] = ADC1->DR;
	if(i == ARRAY_SIZE) {
		i = 0;
		j = 0;
		dataGonder = 1;
	}
}

int main(void)
{

	circularBuffer_init(&outputBuffer);
	//circularBuffer_init(&inputBuffer);

	System_clockConfig();
	SysTick_init();
	USART1_init(115200);
	ADC_init();
	//TIM7_init();
	TIM2_init();

	while(1)
	{
		//result = ADC_Read();
		//ADCdata[i] = result;
		//timerValue = TIM7->CNT;
		//timerData[0] = timerValue;
		if(dataGonder) {
			ADC1->CR1    &= ~(1 << 5);			// Disable ADC Interrupt
			
			strcat(UARTstring, "PC5\r\n");
			for(a = 0; a < ARRAY_SIZE; a++) {
				sprintf(ADCstring, "%d ", ADCdata[a]);
				strcat(UARTstring, ADCstring);
				memset(ADCstring, 0, sizeof(ADCstring));
			}
			strcat(UARTstring, "\r\n\r\n");
			
			strcat(UARTstring, "PC4\r\n");
			for(a = 0; a < ARRAY_SIZE; a++) {
				sprintf(ADCstring2, "%d ", ADCdata2[a]);
				strcat(UARTstring2, ADCstring2);
				memset(ADCstring2, 0, sizeof(ADCstring2));
			}
			strcat(UARTstring2, "\r\n\r\n");
				
			USART1_writeString(UARTstring);
			USART1_writeString(UARTstring2);
			
			memset(UARTstring, 0, sizeof(UARTstring));
			memset(UARTstring2, 0, sizeof(UARTstring2));
			dataGonder = 0;
			delay(5000000);
			ADC1->CR1    |= (1 << 5);			// Enable ADC Interrupt
		}
		//voltageValue = result * (3.3 / 4096);		// 3.3 / 4096 = 0.0008056640625
	}

	return 0;
}

