#include "systick.h"

#define SYSTICK_LOAD_VAL			(16000)

#define CTRL_ENABLE					(1U << 0)

#define CTRL_CLKSRC					(1U << 2)

#define CTRL_COUNTFLAG				(1U << 16)

#define ONE_SEC_LOAD				(16000000)

#define CTRL_TICK_INTERRUPT			(1U << 1)

void systickDelayMS(int delay)
{
	//MS --> milli seconds. input is number of ms delay needed
	//systick timer is the core peripheral of the cortex M architecture. See ARM Cortex M4 generic user guide documentation
	//systick is a 24 bit down counter driven by the processor clock

	//1. load value for systick (system timer). reload with number of clocks per millisec

	SysTick->LOAD = SYSTICK_LOAD_VAL;

	//2. clear systick current value register

	SysTick->VAL = 0;

	//3. Enable systick

	SysTick->CTRL |= CTRL_ENABLE;

	//4.select internal processor clock source

	SysTick->CTRL |= CTRL_CLKSRC;

	for(int i=0; i < delay; i++)
	{
		//wait until count flag is set.

		while(!(SysTick->CTRL & CTRL_COUNTFLAG)){}

	}

	SysTick->CTRL = 0; // disable systick after the usage.

}

void systick_1hz_interrupt(void)
{
	//this systick interrupt occurs every second

	SysTick->LOAD = ONE_SEC_LOAD -1;// as we count from zero

	//clear systick current value register

	SysTick->VAL = 0;

	//3. Enable systick

	SysTick->CTRL |= CTRL_ENABLE;

	//4.select internal processor clock source

	SysTick->CTRL |= CTRL_CLKSRC;

	//enable systick interrupt
	SysTick->CTRL |= CTRL_TICK_INTERRUPT;

	//there is no need of enabling systick in the nvic explicitly

}
