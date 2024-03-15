#include "timer.h"

//these are general purpose timer developed by STM(board manf.)
//we are using timer2 for this experiment
//we want to generate timeout every second

#define TIMER2_CLK_EN			(1U << 0)

#define CR1_CEN					(1U << 0) // CE --> counter enable means timer enable

#define CCER_CC1E				(1U << 0)

#define RCC_GPIOA				(1U << 0)

#define AFR5_TIM				(1U << 20) //AF01 --> 0001 from 20th to 23rd bit

void timer2_1hz_init()
{
	//1.enable clock access to timer2

	RCC->APB1ENR |= TIMER2_CLK_EN;

	//2. set the prescalar value

	//default system clock to apb1 is 16Mhz
	//we want to reduce this clock to 1Hz

	TIM2->PSC = 1600-1; // 16000000/1600 = 10000;

	//3. set ARR( auto relaod register value)
	TIM2->ARR = 10000 - 1; // (here -1 is added as we count from zero in the register. final clock will be 10000/10000 = 1HZ

	//4. clear timer counter

	TIM2->CNT = 0; //clear current counter before starting

	//5. enable the timer

	TIM2->CR1	|= CR1_CEN;

}

void timer2_pa5_output_compare()
{
	//enable clock access to gpioa and set PA5 to alternate function mode
	//1. enable clock access to pioa;
	RCC->AHB1ENR |= (RCC_GPIOA);

	//2.set pa5 to af mode
	GPIOA->MODER &= ~(1U << 10);
	GPIOA->MODER |= (1U << 11);

	//3.set alternate function type to timer2 channel i.e af01 from the table
	GPIOA->AFR[0] |= AFR5_TIM;

	//4.enable clock access to timer2

	RCC->APB1ENR |= TIMER2_CLK_EN;

	//5. set the prescalar value

	//default system clock to apb1 is 16Mhz
	//we want to reduce this clock to 1Hz

	TIM2->PSC = 1600-1; // 16000000/1600 = 10000;

	//6. set ARR( auto relaod register value)
	TIM2->ARR = 10000 - 1; // (here -1 is added as we count from zero in the register. final clock will be 10000/10000 = 1HZ

	// Set Output compare toggle mode
	// 7. set timer2 channel1 in compare mode
	// TIM2_OC1M --> output compare mode for channel of timer2
	// we need to set OC1M to toggle mode
	TIM2->CCMR1 |=  (1U << 4) | (1U << 5);// setting bit 4 and 5 to one for toggling

	//8. enable timer2 channel1 in compare mode
	//CC1E bit --> for channel 1, capture/compare 1 output enable

	TIM2->CCER	|= CCER_CC1E;

	//9. clear timer counter

	TIM2->CNT = 0; //clear current counter before starting

	//10. enable the timer

	TIM2->CR1	|= CR1_CEN;

}


