#include "stm32f4xx.h"

#define GPIOA_EN				(1U << 0)
#define PIN5					(1U << 5)
#define LED_PIN					PIN5

int main(void)
{
	RCC->AHB1ENR |= GPIOA_EN;

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~(1U << 11);

	while(1)
	{
		GPIOA->ODR ^=LED_PIN;
		for(int i=0; i< 100000; i++)
		{

		}
	}
	return 0;
}
