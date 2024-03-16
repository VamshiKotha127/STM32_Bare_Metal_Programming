#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define SR_RXNE					(1U << 5) // status register RXNE bit which tells whether current transmit data is empty or not
//function declarations
void uart2_rxtx_init(void);
void uart2_write(int ch);
int __io_putchar(int ch);
char uart2_read(void);
void uart2_rx_interrupt_init(void);


#endif /* UART_H_ */
