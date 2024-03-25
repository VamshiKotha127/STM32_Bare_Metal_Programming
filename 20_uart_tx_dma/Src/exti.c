#include "exti.h"

#define GPIOC_CLK_EN				(1U << 2)

#define SYSCFG_CLK_EN				(1U << 14)

void pc13_exti_init()
{
	//all pin 13s of A,B,C,D.... are connected to EXTi13 line. All are connected as a multiplexer in the EXTI controller
	//Pins 10 to 15 of all ports share the same IRQ inside NVIC and therefore are serviced by the same ISR(Interrupt service routine)
	//we cant have PA13 and PB13 input interrupt at the same time.
	//some interrupt priorities are defined by ARM. low number means high priority
	// IPR0 (interrupt priority register) holds the priorities of IRQ0, IRQ1, IRQ2 and IRQ3
	//each interrupt request uses 8 bit inside IRP register. In STM we only use 4 bits instead of 8 bits per IRQ means only upper nibble
	// there are 60 interrupt priority registers
	// as we only use 4 bits there are only 16 priority levels. //0 -high priority, 16-low priority

	//Disable global interrupts(optional)
	//use an intrinsic function that comes with cortex m4.h files
	//__disable_irq--> to temporarily prevent the CPU from responding to interrupts

	__disable_irq();

	//1. enable clock access for gpioc pin13

	RCC->AHB1ENR |= GPIOC_CLK_EN;

	//2. explicitly set pc13 as input pin

	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);

	//3. exti is part of SYSCFG. so enable clock access to syscfg

	RCC->APB2ENR |= SYSCFG_CLK_EN;

	//4. select portc for exti13.
	//for connecting exti13 to portc. we need to put 0010 in bit range(4 to 7) in SYSCFG_EXTICR4
	//here EXTICR[3] is used as we want to use SYSCFG_EXTICR4
	//configuration registers will be in the form of array.

	SYSCFG->EXTICR[3] |= (1U << 5); // by default all other bits are 0;

	//5. un-mask/uncover exti13
	//EXTI_IMR --> MR13 =1 means uncovered. as we are interested in line 13 use MR13. IMR --> Interrupt mask register
	//Maskable interrupts are those that may be accepted or ignored by the CPU based on their priority. In contrast, the CPU must accept the non-maskable interrupts. A hardware interrupt that could be ignored by setting a bit in the interrupt mask register's (IMR) bit-mask is referred to as a maskable interrupt
	//The RESET interrupt is the prime example of an interrupt that shouldn't be masked. RESET is a very special case, in that it ends execution of the current program, and starts over from scratch.

	EXTI->IMR |= (1U << 13);

	//6. select falling edge trigger. interrupt to be triggered by falling edge

	EXTI->FTSR |= (1U << 13);   //FTSR --> falling trigger select register

	//7. enable exti13 line in nvic
	//use function which comes with cortexm4 .h file

	 NVIC_EnableIRQ(EXTI15_10_IRQn);// need to pass interrupt number // Interrupt number for line 13 is 40. but there is a symbolic name for this --> EXTI15_10_IRQn
	 //EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts

	//8. enable global interrupts. otherwise no interrupt will work
	__enable_irq();

}

