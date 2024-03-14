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
#define SR_RXNE					(1U << 5) // status register RXNE bit which tells whether current transmit data is empty or not


int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
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


