#include "MKL25Z4.h"                    // Device header
#include "MMA8451Q.h"
#include "UART_0.h"

static uint8_t k_LR;
static uint8_t k_FR;
static char dane;

//zakresy wartosci wychylen z akcelerometru
#define Down_min_range 300
#define Down_max_range 1000
#define Up_min_range 15000
#define Up_max_range 16000
#define Right_min_range 400
#define Right_max_range 1000
#define Left_min_range 15000
#define Left_max_range 16000

void calculateXData();
void calculateYData();
void sendData();
