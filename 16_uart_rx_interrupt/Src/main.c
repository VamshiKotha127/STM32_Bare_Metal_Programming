//here we want to have a uart rx interrupt and do something when interrupt occurs
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "timer.h"
#include "exti.h"

#define LINE13			(1U << 13)

#define GPIOA_CLK_EN	(1U << 0)

#define PIN5			(1U << 5)

#define LED				(PIN5)

char key;

int main(void)
{
	//enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOA_CLK_EN;

	//2. set PA5 as output

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	pc13_exti_init();

	uart2_rx_interrupt_init();

	while(1)
	{

	}

	return 0;
}

static void exti_callback(void)
{
	printf("button pressed....\n\r");
	//toggle led when button is pressed
	GPIOA->ODR ^= LED;

}

static void uart_callback(void)
{
	key = USART2->DR;
	//taking the value from usart data register when an interrupt occurred
	if(key == '1')
	{
		GPIOA->ODR |= LED;
	}
	else
	{
		GPIOA->ODR &= ~LED;
	}
}

void USRT2_IRQHandler(void)
{
	//see line number 184 in the startup .s file
	// this gets called automatically after interrupt occurs

	//check if RXNE is set.checking the usart register
	if(USART2->SR & SR_RXNE)
	{
		//interrupt has occurred.
		uart_callback();
	}
}

void EXTI15_IO_IRQHandler(void)
{
	// this gets called automatically after interrupt occurs
	//this function name is by default mentioned in the vector table.
	//open startup folder under project folder/ . then open startup_stm32f411retx.s. look at line 186
	//  .word	EXTI15_10_IRQHandler         			/* EXTI Line[15:10] interrupts                                        */
	// for all lines from 15 to 10 this same function is executed. so we need to differentiate them if all are enabled.

	//EXTI_PR --> pending register. make PR13 as 1 which means selected trigger request has just occurred
	//1: selected trigger request occurred
	//This bit is set when the selected edge event arrives on the external interrupt line.
	//This bit is cleared by programming it to ‘1’.

	if((EXTI->PR & LINE13) != 0)
	{
		//this is done only to differentiate line13 from others
		//interrupt occurred from line 13.
		//clear the PR flag to rearm it
		EXTI->PR |= (LINE13);
		//do something ...
		exti_callback();
	}
}
