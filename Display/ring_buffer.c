/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	    
    ringBuffer-> tail = 0;
    ringBuffer-> head = 0;
    ringBuffer-> bufferSize = 0;
    
    ringBuffer-> dataInput = dataBuffer;
    ringBuffer-> dataSize = dataBufferSize;
    
    return true; 
    
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    ringBuffer -> head = 0;
	    ringBuffer -> tail = 0;
	    ringBuffer -> bufferSize = 0;
    
    return true;
    
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	if(0==ringBuffer->bufferSize)
	{
	    return true;
	}
	else
	    return false;
	    
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    return ringBuffer -> bufferSize;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return	ringBuffer -> dataSize;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if(ringBuffer-> bufferSize < ringBuffer-> dataSize)
		{
		    ringBuffer->dataInput[ringBuffer->head] = c;
		    ringBuffer->head = (ringBuffer->head + 1) % ringBuffer-> dataSize;
		    ringBuffer->bufferSize++;
		return true;
		}
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
      
		if(ringBuffer-> bufferSize > 0)
		{
		    *c = ringBuffer->dataInput[ringBuffer->tail];
		    ringBuffer->tail = (ringBuffer->tail + 1) % ringBuffer-> dataSize;
		    ringBuffer->bufferSize--;
		return true;
		}
	}
	return false;
}
