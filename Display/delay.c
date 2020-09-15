#include "stm32f4xx_hal.h"
#include "delay.h"

static volatile int globalTimer = 0;

void delay(int n)
{
    int stop = globalTimer + n;
    
    while(globalTimer < stop);
}
