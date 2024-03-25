#include "uart.h"

#define GPIOA_EN				(1U << 0)

#define UART2_CLK_EN			(1U << 17)

#define SYS_FREQ 				16000000 // 16 M cycles per seconds
#define APB1_CLK 				SYS_FREQ
#define UART_BAUDRATE			115200

#define CR1_TE					(1U << 3)
#define CR1_RE					(1U << 2)

#define UART2_MODULE_EN			(1U << 13)

#define SR_TXE					(1U << 7) // status register TXE bit which tells whether current transmit data is empty or not

#define CR1_RXNEIE				(1U << 5) // usart receive interrupt enable

#define DMA1_CLK_EN				(1U << 21)

#define DMA_CR_STREAM_EN		(1U << 0) // Present in DMA stream configuration register

#define CH_SELECT4				(1U << 27) // channel 4 select

#define MEM_INCREMENT			(1U << 10)

#define DMA_DIR_MEM_TO_PERIPH	(1U << 6)

#define DMA_CR_TCIE				(1U << 4)//transfer complete interrupt enable

#define UART_CR3_DMAT			(1U << 7)


static void uart2_set_baudrate(USART_TypeDef* USARTx,uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart2_bd(uint32_t PeriphClk, uint32_t BaudRate);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void dma1_stream6_init(uint32_t src, uint32_t dest, uint32_t len)
{
	//enable clock access to DMA
	RCC->AHB1ENR |= DMA1_CLK_EN;

	//disable dma1 stream6
	DMA1_Stream6->CR &= ~(DMA_CR_STREAM_EN);


	//wait until DMA1 stream is disabled
	while(DMA1_Stream6->CR & DMA_CR_STREAM_EN){}

	//clear all interrupt flags of stream 6 . Types of DMA interrupts ->  transfer complete interrupt, half complete interrupt, error interrupt
	//HIFCR --> high interrupt flag clear register

	//here we are clearing only stream6 interrupt positions

	DMA1->HIFCR |= (1U << 16);
	DMA1->HIFCR |= (1U << 17);
	DMA1->HIFCR |= (1U << 18);
	DMA1->HIFCR |= (1U << 19);
	DMA1->HIFCR |= (1U << 20);


	//set the destination buffer
	DMA1_Stream6->PAR = dest; // peripheral address register --> PAR

	//set the source buffer
	DMA1_Stream6->M0AR = src; // memory 0 address register

	//set the length
	DMA1_Stream6->NDTR = len;

	//select stream6 channel 4 for usart_tx
	DMA1_Stream6->CR = CH_SELECT4;

	//enable memory increment
	DMA1_Stream6->CR  |= MEM_INCREMENT;

	//transfer direction(mem to periph or mem to mem or periph to mem) here we want to move data from an array to usart peripheral using dma. so select mem to periph

	DMA1_Stream6->CR  |= DMA_DIR_MEM_TO_PERIPH;

	//enable atleast one interrupt. here enabling transfer complete interrupt
	DMA1_Stream6->CR |= DMA_CR_TCIE;

	//enable direct mode and disable fifo mode
	DMA1_Stream6->FCR = 0; //disabling fifo mode in fifo control register

	//enable dma1 stream6
	DMA1_Stream6->CR |= DMA_CR_STREAM_EN;

	//enable usart2 transmitter DMA
	//DMAT --> DMA transmitter

	USART2->CR3 |= UART_CR3_DMAT;

	//handle the DMA interrupt after completion of DMA transfer. enable the DMA interrupt
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void uart2_rxtx_init(void)
{
	//configure uart gpio pins
	//1.enable clock access to gpioa
	RCC->AHB1ENR  |= GPIOA_EN;

	//2.set PA2 to alternate function mode


	GPIOA->MODER &=(~(1U << 4));
	GPIOA->MODER |=(1U << 5);

	//3.set PA2 to alternate function type to UART TX (AF07 --> see alternate function table.
	//under Af7 and pa2 we can access usart2
	//AFRL0 --> All pin0's use AFRL0. all pin2's use AFRL2
	//AFR[0] -> for AFRL. AFR[1] -> for AFRH
	//AFRL controls pin0 to pin7
	//AFRH controls pin8 to pin15
	//here alternate function type is AF07

	GPIOA->AFR[0] |=(1U << 8);
	GPIOA->AFR[0] |=(1U << 9);
	GPIOA->AFR[0] |=(1U << 10);
	GPIOA->AFR[0] &=~(1U << 11);

	//set PA3 for UART_RX
	GPIOA->MODER &=(~(1U << 6));
	GPIOA->MODER |=(1U << 7);

	//afrl3
	GPIOA->AFR[0] |=(1U << 12);
	GPIOA->AFR[0] |=(1U << 13);
	GPIOA->AFR[0] |=(1U << 14);
	GPIOA->AFR[0] &=~(1U << 15);

	//configure usart module
	//1.enable clock access to uart2

	RCC->APB1ENR |= (UART2_CLK_EN);

	//2. configure baud rate

	uart2_set_baudrate(USART2, APB1_CLK,UART_BAUDRATE);

	//3. configure the transfer direction
	//see usart control registers

	USART2->CR1 = CR1_TE; // overriding.setting transmit
	USART2->CR1 |= CR1_RE; //no override //setting receiver

	//This makes all other CR1 register values as 0. This causes usart to work in default stage.
	//data width as 8. default parity 1 bit , default no. of start bit =1 and no. of stop bits = 1
	// even parity--> parity bit becomes 0 if there are even number of 1's

	//4. enable the uart2 module

	USART2->CR1 |= UART2_MODULE_EN; //here we cannot override. otherwise previous operation becomes waste


}

void uart2_rx_interrupt_init(void)
{
	//configure uart gpio pins
	//1.enable clock access to gpioa
	RCC->AHB1ENR  |= GPIOA_EN;

	//2.set PA2 to alternate function mode

	GPIOA->MODER &=(~(1U << 4));
	GPIOA->MODER |=(1U << 5);

	//3.set PA2 to alternate function type to UART TX (AF07 --> see alternate function table.
	//under Af7 and pa2 we can access usart2
	//AFRL0 --> All pin0's use AFRL0. all pin2's use AFRL2
	//AFR[0] -> for AFRL. AFR[1] -> for AFRH
	//AFRL controls pin0 to pin7
	//AFRH controls pin8 to pin15
	//here alternate function type is AF07

	GPIOA->AFR[0] |=(1U << 8);
	GPIOA->AFR[0] |=(1U << 9);
	GPIOA->AFR[0] |=(1U << 10);
	GPIOA->AFR[0] &=~(1U << 11);

	//set PA3 for UART_RX
	GPIOA->MODER &=(~(1U << 6));
	GPIOA->MODER |=(1U << 7);

	//afrl3
	GPIOA->AFR[0] |=(1U << 12);
	GPIOA->AFR[0] |=(1U << 13);
	GPIOA->AFR[0] |=(1U << 14);
	GPIOA->AFR[0] &=~(1U << 15);

	//configure usart module
	//1.enable clock access to uart2

	RCC->APB1ENR |= (UART2_CLK_EN);

	//2. configure baud rate

	uart2_set_baudrate(USART2, APB1_CLK,UART_BAUDRATE);

	//3. configure the transfer direction
	//see usart control registers

	USART2->CR1 = CR1_TE; // overriding.setting transmit
	USART2->CR1 |= CR1_RE; //no override //setting receiver

	USART2->CR1 |= CR1_RXNEIE; // enabled receive interrupt

	//enable uart2 interrupt in nvic
	NVIC_EnableIRQ(USART2_IRQn);

	//This makes all other CR1 register values as 0. This causes usart to work in default stage.
	//data width as 8. default parity 1 bit , default no. of start bit =1 and no. of stop bits = 1
	// even parity--> parity bit becomes 0 if there are even number of 1's

	//4. enable the uart2 module

	USART2->CR1 |= UART2_MODULE_EN; //here we cannot override. otherwise previous operation becomes waste

}

static void uart2_set_baudrate(USART_TypeDef* USARTx,uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart2_bd(PeriphClk, BaudRate);	// BRR -> baud rate register

}

static uint16_t compute_uart2_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return (PeriphClk  + (BaudRate/2U))/BaudRate;
}

void uart2_write(int ch)
{
	//make sure transmit data register is empty before transmitting anything
	//write to transmit data register

	while(!(USART2->SR & SR_TXE)){}

	USART2->DR = ch & (0x00ff); // as we want to transmit 8 bit data. DR -->data register

}

char uart2_read(void)
{
	//make sure receive data register is not empty before receiving anything
	while(!(USART2->SR & SR_RXNE)){}

	//read data
	return USART2->DR;

}


