#include "MKL25Z4.h"                    // Device header
#include "MMA8451Q.h"

#define L_motor 0
#define R_motor 1
#define F_motor 0
#define Rear_motor 1

//#define start_frame = 0b11111011;
static uint8_t current_bit;// = 0;
static uint8_t tx_buff[18];// = {};
static uint8_t LR_power;// = 0;
static uint8_t LR_dir;// = 0;				//L=0, R=1
static uint8_t FR_power;// = 0;
static uint8_t FR_dir;// = 0;				//F=0, R=1
static int L_motor_power;// = 0;
static int R_motor_power;// = 0;
static uint8_t L_motor_dir;// = 0;	//0 - front 1 - back
static uint8_t R_motor_dir;// = 0;	//0 - front 1 - back
/*left right calculation*/
uint8_t get_LR_power();
uint8_t get_LR_dir();
void set_LR_power(uint16_t power);
void set_LR_dir(uint8_t dir);
/*front rear calculation*/
uint8_t get_FR_power();
uint8_t get_FR_dir();
void set_FR_power(uint16_t power);
void set_FR_dir(uint8_t dir);
/*engin power*/
void calcuate_motor_power();//uint8_t x_axis, uint8_t x_dir, uint8_t y_axis, uint8_t y_dir);
uint8_t get_L_motor_power();
uint8_t get_R_motor_power();
uint8_t get_L_motor_dir();
uint8_t get_R_motor_dir();
/*transmision*/	
void prepare_data();				//get x and y axis accelerate, calculate motor power fo each motor
void prepare_data_frame();	//prepare the 24-bit data frame
uint8_t get_byte_value(uint8_t byte_num);						//transmit bites 
/*Data to binary*/
void power2bin(uint8_t L_power, uint8_t R_power);		//Change motors power to binary array
/*
1.prepare_data
2.prepare_data_frame
3.transmit - every PIT interroup function send 1-bit of data by changing the value of pin
*/
