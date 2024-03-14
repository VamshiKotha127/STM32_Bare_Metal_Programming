#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

//function declarations
void uart2_rxtx_init(void);
static void uart2_set_baudrate(USART_TypeDef* USARTx,uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart2_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);
int __io_putchar(int ch);
char uart2_read(void);


#endif /* UART_H_ */
