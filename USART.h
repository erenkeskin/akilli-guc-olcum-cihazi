// System Libraries
#include "stdint.h"

// User Defined Libraries
#include "circularBuffer.h"

void 		USART1_init(uint32_t BAUDRATE);
void 		USART1_writeChar(uint8_t character);
void 		USART1_writeString(uint8_t * string);
uint8_t USART1_readString(uint8_t * string);
void 		USART1_IRQHandler(void);
