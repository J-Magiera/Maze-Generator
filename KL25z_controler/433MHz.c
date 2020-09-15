#include "433MHz.h"

uint8_t get_LR_power(){
	return LR_power;
}

uint8_t get_LR_dir(){
	return LR_dir;
}

void set_LR_power(uint16_t power){
	if(power > 8000){
		set_LR_dir(L_motor);
		LR_power = (float)(16384 - power)/1024*50;
	}
	else{
		set_LR_dir(R_motor);
		LR_power = (float)power/1024*50;
	}
}

void set_LR_dir(uint8_t dir){
	LR_dir = dir;
}

uint8_t get_FR_power(){
	return FR_power;
}

uint8_t get_FR_dir(){
	return LR_dir;
}

void set_FR_power(uint16_t power){
	if(power > 8000){
		set_FR_dir(F_motor);
		FR_power = (float)(16384 - power)/1024*100;
	}
	else{
		set_FR_dir(Rear_motor);
		FR_power = (float)power/1024*100;
	}
}

void set_FR_dir(uint8_t dir){
		FR_dir = dir;
}
void calculate_motor_power(){//uint8_t x_axis, uint8_t x_dir, uint8_t y_axis, uint8_t y_dir){	
	L_motor_power = R_motor_power = FR_power;
	if(LR_dir == L_motor){
		L_motor_power -= LR_power;
		L_motor_dir = F_motor;
		if(L_motor_power < 0){		
			L_motor_dir = Rear_motor;
			L_motor_power *=(-1);
			}
		}
	else if(LR_dir == 1){
		R_motor_power -= LR_power;
		R_motor_dir = 0;			
		if(R_motor_power < 0){		
			R_motor_dir = 1;
			R_motor_power *=(-1);
			}
		}
	if(FR_dir == 1){//jazda do przodu
		L_motor_dir ^= 1;
		R_motor_dir ^= 1;
	}
}

uint8_t get_L_motor_power(){
	return L_motor_power;
}
uint8_t get_R_motor_power(){
	return R_motor_power;
}

uint8_t get_L_motor_dir(){
	return L_motor_dir;
}
uint8_t get_R_motor_dir(){
	return R_motor_dir;
}

void prepare_data(){
	set_FR_power(X_axis());
	set_LR_power(Y_axis());
	calculate_motor_power();
}

void prepare_data_frame(){
	tx_buff[0] = 0;		//start byte
	tx_buff[1] = get_L_motor_dir();		//L_dir bit

	tx_buff[9] = get_R_motor_dir();	//R_dir bit
	
	power2bin(get_L_motor_power(), get_R_motor_power());
	tx_buff[17] = 1;		//stop byte
}

void power2bin(uint8_t L_power, uint8_t R_power){
	tx_buff[8] 	=  L_power %2;		///////////	
	tx_buff[7] 	= (L_power >> 1)%2;
	tx_buff[6] 	= (L_power >> 2)%2;
	tx_buff[5] 	= (L_power >> 3)%2;	//data
	tx_buff[4] 	= (L_power >> 4)%2;
	tx_buff[3] 	= (L_power >> 5)%2;
	tx_buff[2] 	= (L_power >> 6)%2;	/////////
	tx_buff[16] =  R_power %2;	/////////
	tx_buff[15] = (R_power >> 1)%2;
	tx_buff[14] = (R_power >> 2)%2;
	tx_buff[13] = (R_power >> 3)%2;	//data
	tx_buff[12] = (R_power >> 4)%2;
	tx_buff[11] = (R_power >> 5)%2;
	tx_buff[10] = (R_power >> 6)%2;	/////////
}

uint8_t get_byte_value(uint8_t byte_num){
	return tx_buff[byte_num];
}

