#include "adc.h"

#define ADC1_CLK 			(1U << 8)

#define GPIOA_CLK			(1U << 0)

#define ADC_CH1				(1U << 0) // 0b 00001
#define ADC_SEQ_LEN_1		  (0x0) ///setting all bits to zero as we are not using any other bits of SQR1 register
#define CR2_AD_ON			(1U << 0)
#define CR2_SWSTART			(1U << 30)
#define SR_EOC				(1U << 1)
#define CR2_CONT 			(1U << 1)

void pa1_adc_init(void)
{
	//1.enable clock access to adc pin port i.e GPIO-PA1

	RCC->AHB1ENR |= GPIOA_CLK;

	//2. set the mode of PA1 to analog

	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U << 3);

	//3. enable the clock access adc module

	RCC->APB2ENR |= ADC1_CLK;

	//configure adc paramters

	//4. configure conversion sequence start
	//ADC_SQR3 --> ADC Sequence regular(without event triggered)

	//SQ1 is the position for first channel in the sequence
	//SQ2 is the position for second channel in the sequence
	//sequence means order in which you want to sample the data
	//If we configure ADC with 3 channels ch2,ch3,ch5
	// we want first channel to be ch5
	// second channel to be ch2
	// third channel to be ch3
	//so for the above mentioned example we need to put 5(101) in the SQ1 parameter in ADC_SQR3 register
	//we need to put 2(10) in the SQ2 parameter in ADC_SQR3 register
	//we need to put 3(11) in the SQ3 parameter in ADC_SQR3 register
	//here in this code, we are using only one channel. so we are putting 00001 in the SQ1
	//we are using SQ3 because SQ3 is supports for 1 to 6 conversions means one to six sensors. here we need only one sensor data conversion

	ADC1->SQR3 = ADC_CH1;

	//5. configure conversion sequence length
	// in SQR1 from (20 to 23 bits we set the  length of conversions/channels.0000 means 1 conversion, 0001 means 2 conversion

	ADC1->SQR1 = ADC_SEQ_LEN_1;

	//6. enable the adc module

	ADC1->CR2 |= CR2_AD_ON;

}

void start_conversion(void)
{
	//Enable continuous conversion
	ADC1->CR2 |= CR2_CONT;
	//start the ADC conversion
	//use control register CR2 SWST(software start means start the conversion from software)
	// SWST = 1 starts the conversion from regular channel
	// there are some other ways for triggering the conversion using timer as well

	ADC1->CR2 |= CR2_SWSTART;
}


uint32_t adc_read(void)
{
	//before reading we need to wait for the conversion to complete. Use status registers
	//ADC_SR register EOC parameter =1 means conversion done

	while(!(ADC1->SR & SR_EOC)) {}

	//read converted results
	return ADC1->DR;

}
