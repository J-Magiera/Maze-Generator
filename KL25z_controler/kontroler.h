#include "MKL25Z4.h"                    // Device header
#include "MMA8451Q.h"
#include "UART_0.h"

static uint8_t k_LR;
static uint8_t k_FR;
static char dane;

void calculateXData();
void calculateYData();
void sendData();
