#include "timer.h"

//we are using timer2 for this experiment
//we want to generate timeout every second

#define TIMER2_CLK_EN			(1U << 0)

#define CR1_CEN					(1U << 0) // CE --> counter enable means timer enable

#define DIER_UIE				(1U << 0) // UIE --> update interrupt enable

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

void timer2_1hz_interrupt_init()
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

	//enable timer interrupt

	TIM2->DIER	|= DIER_UIE; // DIER -->DMA /Interrupt enable register

	//enable tim2 in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);

	//5. enable the timer

	TIM2->CR1	|= CR1_CEN;

}


