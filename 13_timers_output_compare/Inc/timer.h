#ifndef TIMER_H_
#define TIMER_H_


#include "stm32f4xx.h"
#include <stdint.h>
#define SR_UIF					(1U << 0) //UIF --> update interrupt flag. SR -- status register

void timer2_1hz_init();
void timer2_pa5_output_compare();

#endif /* TIMER_H_ */
