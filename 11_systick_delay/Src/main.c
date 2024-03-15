//only one 12 bit(resolution) adc module(ADC1) is present. It has 16channels. It is connected to APB2 clock
//there is one internal temperature sensor. We can sample that for temperature of the chip
//configure PA1 as ADC1 pin for taking analog data as input

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "systick.h"

#define RCC_GPIOA			(1U << 0);

#define PIN5				(1U << 5);

#define LED_PIN 			PIN5;

int main(void)
{
	// Enable clock to GPIOA
	RCC->AHB1ENR |= RCC_GPIOA;

	// set PA5 as output pin
	GPIOA->MODER |=  (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	uart2_rxtx_init();
	while(1)
	{
		GPIOA->ODR ^= LED_PIN; // toggle
		systickDelayMS(1000);
		printf("A second passed !! \n\r"); // calls usart transmit
	}

	return 0;
}
