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

int timestamp = 0;

//connect a jumper wire from pa5 to pa6.
//we are capturing the toggle rate from pa5 using pa6. and then we are printing that value using timestamp.
//as we set the
int main(void)
{
	//this generates a square wave (toggle) of 1's and 0's.
	timer2_pa5_output_compare();
	timer3_pa6_input_capture();

	while(1)
	{
		//we have to wait until edge is captured
		while(!(TIM3->SR & SR_CC1IF)){}

		//read captured values from timer3.TIMx_CCRx) are used to latch the value of the counter after a transition detected
		//this indicates the counter value of TIM3 CH1 when the led toggle(every 1s)
		//after deployment our timestamp variable value changes by 2000. means timer3counter value is changing 1000 while timer2 led toggles every 1sec.
		// as we have used rising edge counter values are updating by 2000.

		timestamp = TIM3->CCR1;

		//connect a jumper wire from pa5 to pa6
	}

	return 0;
}
