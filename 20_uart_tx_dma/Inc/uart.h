#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define SR_RXNE					(1U << 5) // status register RXNE bit which tells whether current transmit data is empty or not

#define HISR_TCIF6		    	(1U << 21) // transfer complete interrupt

#define HIFCR_CTCIF6			(1U << 21)

//function declarations
void uart2_rxtx_init(void);
void uart2_write(int ch);
int __io_putchar(int ch);
char uart2_read(void);
void uart2_rx_interrupt_init(void);
void dma1_stream6_init(uint32_t src, uint32_t dest, uint32_t len);


#endif /* UART_H_ */
