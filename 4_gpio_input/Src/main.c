#include "stm32f4xx.h"

#define GPIOA_EN				(1U << 0)
#define PIN5					(1U << 5)
#define LED_PIN					PIN5

#define GPIOC_EN				(1U << 2)
#define PIN13					(1U << 13)
#define BTN_PIN					PIN13

int main(void)
{
	//enable clock access to gpioa and gpioc
	RCC->AHB1ENR |= GPIOA_EN;
	RCC->AHB1ENR |= GPIOC_EN;

	//set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~(1U << 11);

	//set PC13 as input pin
	GPIOC->MODER &=~(1U << 26);
	GPIOC->MODER &=~(1U << 27);

	while(1)
	{
		//check if button is pressed or not. button is active low means when pressed it sends low(0)
		if(GPIOC->IDR & BTN_PIN) // just doing bitwise-and with position 13 bit high
		{
			//set led. turning on led when button is released (by default button is released)
			GPIOA->BSRR = LED_PIN;
		}
		else
		{
			//reset led. turning off led when button is pressed
			GPIOA->BSRR = (1U << 21);
		}

	}
	return 0;
}
