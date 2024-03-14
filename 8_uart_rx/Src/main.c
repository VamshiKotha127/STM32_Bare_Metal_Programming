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

#define RCC_GPIOAEN		(1U << 0);

#define PIN5			(1U << 5);

#define LED_PIN 		PIN5;


char key;

int main(void)
{
	uart2_rxtx_init();

//	while(1)
//	{
//		printf("Hello from STM32F411.......\n\r");
//	}

	RCC->AHB1ENR |= RCC_GPIOAEN;

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	while(1)
	{
		key=uart2_read();
		if(key == 1)
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
		}
	}

	return 0;
}
