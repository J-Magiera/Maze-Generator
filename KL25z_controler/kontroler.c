#include "kontroler.h"

//obliczanie wartosci wychlenia w osi X - odpowiedzialnej za ruchy przód tyl
void calculateXData(){//przód tyl
	uint16_t tempData = 0;
	tempData = X_axis();
	if((tempData > Down_min_range)&&(tempData < Down_max_range)){
		dane = 'd';
	}
	else if((tempData < Up_max_range)&&(tempData > Up_min_range)){
		dane = 'u';
	}
}


//obliczanie wartosci wychlenia w osi Y - odpowiedzialnej za ruchy lewo prawo
void calculateYData(){
	uint16_t tempData = 0;
	tempData = Y_axis();
	if((tempData > Right_min_range)&&(tempData < Right_max_range)){
		dane = 'r';
	}
	else if((tempData < Left_max_range)&&(tempData > Left_min_range)){
		dane = 'l';
	}
}


void sendData(){
	calculateYData();
	calculateXData();
	print_char(dane);
	dane = 0;
}


