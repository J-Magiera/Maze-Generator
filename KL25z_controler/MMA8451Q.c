#include "MMA8451Q.h"
#include "UART_0.h" 

uint8_t AccData[6];

void ACCEL_init(void){
	I2C_init();
	
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;				//Turn on PortA for MMA interrupt
	PORTA -> PCR[14] |= PORT_PCR_MUX(1)				//
										| PORT_PCR_ISF_MASK			//Clear interrupt falg
										| PORT_PCR_IRQC(0xB);		//Config for fallin edge
	
	NVIC_ClearPendingIRQ(I2C0_IRQn);				/* Clear NVIC any pending interrupts on PORTA14 */
	NVIC_EnableIRQ(I2C0_IRQn);							/* Enable NVIC interrupts source for PORTA14 */
	NVIC_SetPriority (I2C0_IRQn, 0);				/* PORTA14 interrupt priority level  */ 
	
	uint8_t reg_val = 0;
	
	I2C_write(MMA_addr, CTRL_REG2, 0x40);
	while(I2C_read(MMA_addr, CTRL_REG2) & 0x40){
		reg_val = I2C_read(MMA_addr, CTRL_REG2) & 0x40;
		
	}
	I2C_write(MMA_addr, CTRL_REG1, 0x00);
	I2C_write(MMA_addr, XYZ_DATA_CFG, 0x03);
	
	
	I2C_write(MMA_addr, CTRL_REG3, 0b11111000);
	I2C_write(MMA_addr, CTRL_REG4, 0x01);
	I2C_write(MMA_addr, CTRL_REG5, 0x00);
	I2C_write(MMA_addr, CTRL_REG1, 0x01);
}

uint16_t X_axis(){
	static uint8_t RAW_X_DATA[2];
	static uint16_t x_accel;
	RAW_X_DATA[0] = I2C_read(MMA_addr, OUT_X_MSB_REG);
	RAW_X_DATA[1] = I2C_read(MMA_addr, OUT_X_LSB_REG);
	x_accel = ((uint16_t)(RAW_X_DATA[0])<<6) | ((uint16_t)(RAW_X_DATA[1])>>2);
	return x_accel;
}
uint16_t Y_axis(){
	static uint8_t RAW_Y_DATA[2];
	static uint16_t y_accel;
	RAW_Y_DATA[0] = I2C_read(MMA_addr, OUT_Y_MSB_REG);
	RAW_Y_DATA[1] = I2C_read(MMA_addr, OUT_Y_LSB_REG);
	y_accel = ((uint16_t)(RAW_Y_DATA[0])<<6) | ((uint16_t)(RAW_Y_DATA[1])>>2);
	return y_accel;
}
uint16_t Z_axis(){//not used in this project
	static uint8_t RAW_Z_DATA[2];
	static uint16_t z_accel;
	RAW_Z_DATA[0] = I2C_read(MMA_addr, OUT_Z_MSB_REG);
	RAW_Z_DATA[1] = I2C_read(MMA_addr, OUT_Z_LSB_REG);
	z_accel = ((uint16_t)(RAW_Z_DATA[0])<<6) | ((uint16_t)(RAW_Z_DATA[1])>>2);
	return z_accel;
}
uint16_t MMA_DATA(uint8_t* DATA_S){
	I2C_multiRegRead(MMA_addr, OUT_X_MSB_REG, 6, DATA_S);
}
