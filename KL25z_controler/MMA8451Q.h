#include "MKL25Z4.h"                    // Device header
#include "I2C.h"

#define MMA_addr 0x1D

#define STATUS_REG            0x00		// STATUS Register 

#define OUT_X_MSB_REG         0x01		// [7:0] are 8 MSBs of the 14-bit X-axis sample
#define OUT_X_LSB_REG         0x02		// [7:2] are the 6 LSB of 14-bit X-axis sample
#define OUT_Y_MSB_REG         0x03		// [7:0] are 8 MSBs of the 14-bit Y-axis sample
#define OUT_Y_LSB_REG         0x04		// [7:2] are the 6 LSB of 14-bit Y-axis sample
#define OUT_Z_MSB_REG         0x05		// [7:0] are 8 MSBs of the 14-bit Z-axis sample
#define OUT_Z_LSB_REG         0x06		// [7:2] are the 6 LSB of 14-bit Z-axis sample

#define WHO_AM_I 0x0D
/*#define F_SETUP  0x09
#define TRIG_CFG 0x0A
#define SYSMOD	 0x0B
#define INT_SOURCE 		0x0C*/
#define XYZ_DATA_CFG	0x0E
/*#define PL_STATUS 		0x10
#define PL_CFG 				0x11
#define PL_COUNT 			0x12
#define PL_BF_ZCOMP 	0x13
#define P_L_THS_REG 				0x14
#define FF_MT_CFG 					0x15
#define FF_MT_SRC						0x16
#define FF_MT_THS 					0x17
#define FF_MT_COUNT 				0x18*/
#define CTRL_REG1 					0x2A
#define CTRL_REG2 					0x2B
#define CTRL_REG3 					0x2C
#define CTRL_REG4 					0x2D
#define CTRL_REG5 					0x2E
#define ASLP_COUNT_REG      0x29 

/*#define OFF_X_REG             0x2F    	// XYZ Offset Correction Registers 
#define OFF_Y_REG             0x30
#define OFF_Z_REG             0x31
#define TRANSIENT_CFG_REG     0x1D    	// TRANSIENT_CFG Transient Configuration Register 
#define TRANSIENT_SRC_REG     0x1E    	// TRANSIENT_SRC Transient Source Register 
#define TRANSIENT_THS_REG     0x1F    	// TRANSIENT_THS Transient Threshold Register 
#define TRANSIENT_COUNT_REG   0x20    	// TRANSIENT_COUNT Transient Debounce Counter Register
#define SENSITIVITY_2G		  4096*/
	
	
void ACCEL_init(void);
uint16_t X_axis();
uint16_t Y_axis();
uint16_t Z_axis();
uint16_t MMA_DATA(uint8_t* DATA_S);
