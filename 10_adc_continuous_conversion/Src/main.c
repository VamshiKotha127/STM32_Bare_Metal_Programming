//only one 12 bit(resolution) adc module(ADC1) is present. It has 16channels. It is connected to APB2 clock
//there is one internal temperature sensor. We can sample that for temperature of the chip
//configure PA1 as ADC1 pin for taking analog data as input

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"

uint32_t sensor_value;
int main(void)
{
	uart2_rxtx_init();
	pa1_adc_init();
	start_conversion(); // this is single channel continuous mode as we have enabled continuous conversion in the ADC1->CR2 register. without that line, this would be single channel single conversion mode only.
	while(1)
	{
		sensor_value=adc_read();
		printf("Sensor value : %d \n\r",(int)sensor_value); // %d works with int not with unit32_t. if we dont typecast we will a get warning

		//for getting the analog data. use jumper wires and connect pa1 to gnd. sensor value output on the serial window should be 0;
		//if we connect the jumper wire between pa1 and 3v3. then the output should be nearly 4095. because resolution is 12 bit.2 power 12 is 4096. here its considering 3.3v as voltage
		//or else we can use a potentiometer. connect first endpoint of pot to pa1.second endpoint to gnd and third endpoint to 3.3v.
		//when you turn the pot, we can see the analog value is changing
	}

	return 0;
}
