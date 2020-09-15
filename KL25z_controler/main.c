#include "MKL25Z4.h"                    // Device header
#include "I2C.h" 
//#include "UART_0.h" 
#include "MMA8451Q.h"
#include "PIT_timer.h"
#include "kontroler.h"

int ii = 0xaffff;// delay value
uint16_t A_DATA = 0;
uint8_t flag = 0;

void main(void){
	UART_init(9600);
	//print_char('U');
	I2C_init();
	print_char('F');
	ACCEL_init(); 
	print_word("\n\rInit Uart, I2C, Accel done \n\r");
	A_DATA = I2C_read(MMA_addr, WHO_AM_I);
	print_word("Who I am: \n\r");
	print_num(A_DATA);
	PIT_init();
	
	while(1){
		if(1 == flag){
			sendData();
			flag = 0;
		}
	}
	
};


void PIT_IRQHandler(void)										//PIT interrupt handler function
{
	flag = 1;
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;						//Clear PIT0 falg
}

/*void PORTA_IRQHandler()
{
	PORTA_PCR14 |= PORT_PCR_ISF_MASK;			// Clear the interrupt flag 
	print_word("\rInterroupt");
	DataReady = 1;	
}*/

