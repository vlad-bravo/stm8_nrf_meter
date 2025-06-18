#include "functions.h"

uint16_t debounceCounter;

void checkButtonDebounced(void)
{
  if (debounceCounter)
  {
    debounceCounter--;
  }
  else
  {
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == RESET)
    {
      flags |= _FL_KEY;
      debounceCounter = 30000;
    }
  }
}
