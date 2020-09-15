#include "kontroler.h"


void calculateXData(){//przód tyl
	uint16_t tempData = 0;
	tempData = X_axis();
	if((tempData > 300)&&(tempData < 1000)){
		dane = 'd';
	}
	else if((tempData < 16000)&&(tempData > 15000)){
		dane = 'u';
	}
}

void calculateYData(){
	uint16_t tempData = 0;
	tempData = Y_axis();
	/*print_num(tempData);
	print_word("\n\r");*/
	if((tempData > 400)&&(tempData < 1000)){
		dane = 'r';
	}
	else if((tempData < 16000)&&(tempData > 15000)){
		dane = 'l';
	}
}


void sendData(){
	calculateYData();
	calculateXData();
	print_char(dane);
	//print_word(dane);
	
	dane = 0;
}


