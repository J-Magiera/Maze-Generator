#include "PIT_timer.h"

void PIT_init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;	//Enable clock to PIT
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;		//Module disable OFF
	PIT_TCTRL0 = PIT_TCTRL_TIE_MASK;	//Timer interrupt enable
	PIT_LDVAL0 = PIT0_val;						//LD val
	PIT ->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_CHN_MASK);
	NVIC_ClearPendingIRQ(PIT_ALARM_INT_NBR);	//
	NVIC_EnableIRQ(PIT_ALARM_INT_NBR);				//Interrupt clera,enable,priority
	NVIC_SetPriority(PIT_ALARM_INT_NBR,0);		//
	
	//PIT -> CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;					//Enable timer to start
}

//uint16_t i =0;
//uint16_t num =0;
//void PIT_IRQHandler(void)										//PIT interrupt handler function
//{ 
/*	i++;
	if (200 == i)
	{PTB -> PSOR |= (1UL << 10);}
	else if (400 == i)
	{PTB -> PCOR |= (1UL << 10);
	i=0;}*/
	//PIT_TFLG0 |= PIT_TFLG_TIF_MASK;						//Clear PIT0 falg
//}
