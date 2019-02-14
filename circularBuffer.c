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
 *  circularBuffer.c
 *
 ***************************************************************/
#include "circularBuffer.h"

// Clear read and write variables
void circularBuffer_init(circularBuffer * buffer) 
{
    buffer->read 	= 0;
    buffer->write 	= 0;
}

// Control - Is circular buffer full?
uint8_t circularBuffer_full(circularBuffer * buffer) 
{
    int16_t difference = buffer->write - buffer->read;

    if(difference < 0) 
    {
        difference += BUFFER_SIZE;
    }

    if(difference == (BUFFER_SIZE - 1))
    {
    	return 1;
    } else {
    	return 0;
    }

}
 
// Control - Is circular buffer empty?
uint8_t circularBuffer_empty(circularBuffer * buffer) 
{
	if((buffer->write == buffer->read))
    {
    	return 1;
    } else {
    	return 0;
    }
}

// Write the data to circular buffer
void circularBuffer_write(circularBuffer * buffer, int8_t character) 
{
    if(!(circularBuffer_full(buffer))) 
    {
        buffer->buf[buffer->write] = character;
        buffer->write++;

        if(buffer->write == BUFFER_SIZE)
        {
        	buffer->write = 0;
        }
    }
}

// Read data from circular buffer
int8_t circularBuffer_read(circularBuffer * buffer) 
{
    int8_t character = 0;

    if(!(circularBuffer_empty(buffer))) 
    {
        character = buffer->buf[buffer->read];
        buffer->read++;

        if(buffer->read == BUFFER_SIZE)
        {
        	buffer->read = 0;
        }
    }

    return character;
}
