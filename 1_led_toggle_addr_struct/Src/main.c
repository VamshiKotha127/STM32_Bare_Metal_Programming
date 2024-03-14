//led connection
//PA pin5(total 16 pins are there)
//clock through AHB bus
//buses transmit clock for gpio A
//rcc clock -- resetting clock control enables bus for transmitting clock.
//Enable AHB1 from RCC registers for GPIOA port
//use mode register for configuring gpioA pin5
//AHB (advanced high performance bus) takes 1 clock cycle for accessing peripherals
//APB (advanced peripheral bus) take minimum 2 clock cycles for accessing peripherals
// clock source --> 1. on-chip RC(resistor capacitor oscillator) -- least precise
//					2. connect external crystal --	Most precise
//					3. phased locked loop (PLL)	-- program the clock(takes input clock and reconfigures the output clock based on the circuitary)

#include <stdint.h>

#define PERIPH_BASE 			(0x40000000UL)
#define AHB1PERIPH_OFFSET  		(0x00020000UL)
#define AHB1PERIPH_BASE 		(PERIPH_BASE + AHB1PERIPH_OFFSET)

#define GPIOA_OFFSET 			(0x00000000UL)
#define GPIOA_BASE 				(AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET 				(0x00003800UL)
#define RCC_BASE 				(AHB1PERIPH_BASE + RCC_OFFSET)


#define GPIOAEN 				(1UL << 0) // 0x0000 0001

#define PIN5					(1U << 5)
#define LED_PIN					(PIN5)

#define __IO volatile

#pragma pack(push)
#pragma pack(4)
typedef struct
{
	__IO uint32_t MODER; 		// mode register									offset : 0x00
	__IO uint32_t OTYPER;		// output type register								offset : 0x04
	__IO uint32_t OSPEEDR;		// output speed register							offset : 0x08
	__IO uint32_t PUPDR;		// pull-up/pull down register						offset : 0x0C
	__IO uint32_t IDR;			// input data register								offset : 0x10
	__IO uint32_t ODR;			// output data register								offset : 0x14
	__IO uint32_t BSRR;			// bit set/reset register							offset : 0x18
	__IO uint32_t LCKR;			// configuration lock register						offset : 0x1C
	volatile uint32_t AFR[2];		// alternate function low and high register		offset : 0x20 - 0x24

}GPIOA_TypeDef;
#pragma pack(pop)

typedef struct
{
	volatile uint32_t DUMMY[12]; 	// other unwanted registers
	volatile uint32_t AHB1ENR;		// AHB1 enable register

}RCC_TypeDef;

// make structures starting address as base
#define RCC   ((RCC_TypeDef*)(RCC_BASE))
#define GPIOA ((GPIOA_TypeDef*)(GPIOA_BASE))

int main(void)
{
	/* 1. Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/* 2. Set PA5 as output pin */

	GPIOA->MODER  |= (1U << 10);
	GPIOA->MODER  &=~(1U << 11);

	/* 3. Set PA5 high */
	while(1)
	{
		GPIOA->ODR ^= LED_PIN;
		for(int i=0; i < (int)1e6; i++)
		{
			//delay for seeing the LED toggle
		}
	}

	return 0;
}







