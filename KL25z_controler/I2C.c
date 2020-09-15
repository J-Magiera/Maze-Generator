#include "I2C.h"

//macros for i2c (pooling)
#define I2C_START 	I2C0 -> C1 |= I2C_C1_MST_MASK;
#define I2C_STOP 		I2C0 -> C1 &=~I2C_C1_MST_MASK;
#define I2C_RESTART I2C0 -> C1 |= I2C_C1_RSTA_MASK;

#define I2C_TRAN 		I2C0 -> C1 |= I2C_C1_TX_MASK;
#define I2C_REC 		I2C0 -> C1 &= ~I2C_C1_TX_MASK;

#define BUSY_ACK 		while(I2C0 -> S & 0x01)
#define TRANS_COMP 	while(!(I2C0 -> S & IC2_S_TCF_MASK))
#define I2C_WAIT 		while((I2C0 -> S & I2C_S_IICIF_MASK)==0){}\
													I2C0 -> S |= I2C_S_IICIF_MASK;

#define NACK 				I2C0 -> C1 |= I2C_C1_TXAK_MASK;
#define ACK 				I2C0 -> C1 &= ~I2C_C1_TXAK_MASK;
	
#define pause				for(int i =0; i <50; i++){}
///
	
void I2C_init(){
	SIM -> SCGC4 |= SIM_SCGC4_I2C0_MASK;		/*Enable clock for I2C module*/
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;		/*Enable clock for PORTB module*/
	
	I2C0 -> F		|= I2C_F_MULT(0)
							|  I2C_F_ICR(7);
	I2C0 -> C1 	|= I2C_C1_IICEN_MASK;
						//	|  I2C_C1_IICIE_MASK;

	NVIC_ClearPendingIRQ(I2C0_IRQn);				/* Clear NVIC any pending interrupts on I2C */
	NVIC_EnableIRQ(I2C0_IRQn);							/* Enable NVIC interrupts source for I2C */
	NVIC_SetPriority (I2C0_IRQn, 0);			/* ToDo 7.2a: Set I2C interrupt priority level  */ 
	
	PORTE -> PCR[24] |= PORT_PCR_MUX(5);
	PORTE -> PCR[25] |= PORT_PCR_MUX(5);
}

void I2C_write(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t data){
	
	I2C_START;
	I2C_TRAN
	I2C0 -> D = SlaveAddr << 1;				//7-bit addres 1-bit R/W command (R/W = 0 - write)
	I2C_WAIT
	
	I2C0 -> D = RegAddr;
	I2C_WAIT
	
	I2C0 -> D = data;
	I2C_WAIT
	
	I2C_STOP;
	pause
}
uint8_t I2C_read(uint8_t SlaveAddr, uint8_t RegAddr){
	
	uint8_t recData = 0;
	
	I2C_START
	pause//
	I2C_TRAN
	I2C0 -> D = SlaveAddr << 1;  				//7-bit addres 1-bit R/W command (R/W = 0 - write)
	I2C_WAIT
	
	I2C0 -> D = RegAddr;
	I2C_WAIT
	
	I2C_RESTART
	pause//
	
	I2C0 -> D = (SlaveAddr << 1) | 0x1;	//7-bit addres 1-bit R/W command (R/W = 1 - read)
	I2C_WAIT
	
	I2C_REC
	NACK
	
	recData = I2C0 -> D;
	I2C_WAIT
	I2C_STOP
	I2C_REC
	recData = I2C0 -> D;
	pause
	
	return recData;
	
}

void I2C_multiRegRead(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t numOfReg, uint8_t *recMData){
	
	uint8_t i =0;
	
	I2C_START
	I2C_TRAN
	I2C0 -> D = SlaveAddr << 1;  				//7-bit addres, 1-bit R/W command (R/W = 0 - write)
	I2C_WAIT
	
	I2C0 -> D = RegAddr;
	I2C_WAIT
	
	I2C_RESTART
	
	I2C0 -> D = (SlaveAddr << 1) | 0x1;	//7-bit addres, 1-bit R/W command (R/W = 1 - read)
	I2C_WAIT
	
	I2C_REC
	ACK
	
	i = I2C0 -> D;
	I2C_WAIT
	
	for(i = 0; i < numOfReg - 2; i++){
		*recMData = I2C0 -> D;
		recMData++;
		I2C_WAIT
	}
	NACK
	*recMData = I2C0 -> D;
	numOfReg++;
	I2C_WAIT
	I2C_STOP
	*recMData = I2C0 -> D;
		pause
	
}

void I2C0_IRQHandler(){
	
}