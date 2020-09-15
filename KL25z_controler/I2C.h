#include "MKL25Z4.h"                    // Device header

//I2C function
void I2C_init();			/*I2C initialization function*/
void I2C_write(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t data);
uint8_t I2C_read(uint8_t SlaveAddr, uint8_t RegAddr);
void I2C_multiRegRead(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t numOfReg, uint8_t *recMData);
