#include "MKL25Z4.h"                    // Device header

#define PIT0_val 20971520/2//4Hz 
#define PIT_ALARM_INT_NBR (IRQn_Type) 22	//IRQn of PIT

void PIT_init(void);
