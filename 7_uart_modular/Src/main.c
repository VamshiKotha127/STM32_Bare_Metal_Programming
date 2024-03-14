//usart uses pins for tx and rx. see alternate functions pins table
//STM32 default system clock is 16MHz
//system clock is divided by a value and we get clocks for AHB1/APB1 etc. But this divisor is 1 by default
//Here our peripheral clock is same as our sys_freq;

//usart2 is directly connected to usb. USB which was connected to the debugger for flashing the controller is also connected to usart2
//For usart1 module, we need separate usart to usb converter module

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"

int main(void)
{
	uart2_tx_init();

	while(1)
	{
		printf("Hello from STM32F411.......\n\r");
	}
	return 0;
}
