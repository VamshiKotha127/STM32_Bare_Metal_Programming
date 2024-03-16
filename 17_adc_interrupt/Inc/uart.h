#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

//function declarations
void uart2_rxtx_init(void);
void uart2_write(int ch);
int __io_putchar(int ch);
char uart2_read(void);


#endif /* UART_H_ */
