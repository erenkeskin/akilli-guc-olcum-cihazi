/****************************************************************
 *
 *  Thank you for the guidance Samet AK - s.ak.0793@gmail.com
 *
 *  Hasan Eren Keskin
 *  Electrical and Electronics Engineer
 *  email:      herenkeskin@gmail.com
 *  github:     @erenkeskin
 *  website:    http://herenkeskin.com
 *
 *  circularBuffer.h
 *
 ***************************************************************/
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdint.h>

#define BUFFER_SIZE		256

// Data Structures
typedef struct
{
    uint16_t write;
    uint16_t read;
    uint8_t buf[BUFFER_SIZE];
} circularBuffer;

// Functions Prototypes
void 		circularBuffer_init(circularBuffer * buffer);
uint8_t circularBuffer_full(circularBuffer * buffer);
uint8_t circularBuffer_empty(circularBuffer * buffer);
void 		circularBuffer_write(circularBuffer * buffer, int8_t character);
int8_t 	circularBuffer_read(circularBuffer * buffer);

#endif /* CIRCULARBUFFER_H_ */
