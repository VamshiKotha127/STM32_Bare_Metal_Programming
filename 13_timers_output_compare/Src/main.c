//only one 12 bit(resolution) adc module(ADC1) is present. It has 16channels. It is connected to APB2 clock
//there is one internal temperature sensor. We can sample that for temperature of the chip
//configure PA1 as ADC1 pin for taking analog data as input
// Here in this module we are trying to turn off and on the led directly from the timer code without any explicit gpoia code
// we set the alternate function AF01 mode for PA5 led pin
//led goes on and off based on timer

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "timer.h"

int main(void)
{
	timer2_pa5_output_compare();

	while(1)
	{

	}

	return 0;
}
