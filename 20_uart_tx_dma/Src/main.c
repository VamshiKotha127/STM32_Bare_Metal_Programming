//here we want to have a uart tx using dma
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "timer.h"
#include "exti.h"

//the DMA allows data transfer to take place in background without the intervention of the cortex M processor.
//During this operation, the main processor can execute other tasks and it is only interrupted when a whole data block is available for processing
//large amounts of data can be transferred with no major impact on the system performance
//
//There are 2 DMA modules. each module implement 8 different streams.
//each stream is dedicated to managing memory access request from one or more peripherals
//each stream has up to eight selectable channels(requests) in total
//
//
//the DMA module has an arbiter which is used for handling the priority between the DMA streams. stream priority is software configurable.
//there are 4 priority levels
//if two or more DMA streams have the same software priority level, the hardware priority is used.
//hardware priority - stream 0 has priority over stream 1
//
//a DMA transfer is defined by a source address and a destination address.
//Both the source and destination should be in the AHB or APB memory ranges
//
//transfer modes:
//1. peripheral to memory
//2. memory to peripheral
//3. memory to memory - only available in DMA module
//
//transfer size has to be defined only when the DMA is the flow controller. this value defines the volume of data to be transferre from source to destination
//
//DMA can automatically increment source and destination addresses after each transfer
//
//for collecting the data continuously, use circular mode
//
//Each stream has an independent 4 word ( 4*32 bits) FIFO. The FIFO is used to temporarity store data coming from the source before transmitting it to the destination
//
//If DMA FIFO is enabled, data packing/unpacking and /or burst mode can be used
//
//we can know the status of FIFO at any time whether its half-full, or full etc.

#define LINE13			(1U << 13)

#define GPIOA_CLK_EN	(1U << 0)

#define PIN5			(1U << 5)

#define LED				(PIN5)

char key;

int main(void)
{
	char msg[31] = "Hello from stm2 DMA transfer \n\r";
	//enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOA_CLK_EN;

	//2. set PA5 as output

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	uart2_rxtx_init();

	dma1_stream6_init((uint32_t)msg, (uint32_t)((USART2->DR)), 32);
	while(1)
	{

	}

	return 0;
}

static void dma_callback(void)
{
	GPIOA->ODR ^= LED;
}

void DMA1_Stream6_IRQHandler(void)
{
	//check for transfer complete interrupt

	if(DMA1->HISR & HISR_TCIF6)
	{
		//interrupt has occurred.

		//clear interrupt flag and then do something
		DMA1->HIFCR |= HIFCR_CTCIF6;
		dma_callback();
	}
}

