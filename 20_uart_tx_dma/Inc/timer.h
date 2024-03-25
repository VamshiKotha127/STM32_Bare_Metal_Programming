#ifndef TIMER_H_
#define TIMER_H_


#include "stm32f4xx.h"
#include <stdint.h>
#define SR_UIF					(1U << 0) //UIF --> update interrupt flag. SR -- status register
#define SR_CC1IF				(1U << 1) // check the status whether there is inputer capture or not. If 0 then no capture
void timer2_1hz_init();
void timer2_pa5_output_compare();
void timer3_pa6_input_capture();

#endif /* TIMER_H_ */
