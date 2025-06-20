// https://cdeblog.ru/simple-ring-buffer

#include "functions.h"

#define RING_SIZE 32

uint8_t rx_byte;

typedef struct
{
    uint8_t buffer[RING_SIZE];
    uint8_t idxIn;
    uint8_t idxOut;
} RING_buffer_t;

RING_buffer_t RingBuffer;

void RING_Put(uint8_t symbol)
{
  RingBuffer.buffer[RingBuffer.idxIn++] = symbol;
  if (RingBuffer.idxIn >= RING_SIZE) RingBuffer.idxIn = 0;
}

uint8_t RING_Pop()
{
  uint8_t retval = RingBuffer.buffer[RingBuffer.idxOut++];
  if (RingBuffer.idxOut >= RING_SIZE) RingBuffer.idxOut = 0;
  return retval;
}

uint8_t RING_GetCount()
{
  uint8_t retval = 0;
  if (RingBuffer.idxIn < RingBuffer.idxOut) retval = RING_SIZE + RingBuffer.idxIn - RingBuffer.idxOut;
  else retval = RingBuffer.idxIn - RingBuffer.idxOut;
  return retval;
}

uint8_t RING_ShowSymbol(uint8_t symbolNumber)
{
    uint8_t index = RingBuffer.idxOut + symbolNumber;
    uint8_t retval = -1;
    if (symbolNumber < RING_GetCount())
    {
        if (index > RING_SIZE) index -= RING_SIZE;
        retval = RingBuffer.buffer[index];
    }
    return retval;
}

void RING_Clear()
{
  RingBuffer.idxIn = 0;
  RingBuffer.idxOut = 0;
}
