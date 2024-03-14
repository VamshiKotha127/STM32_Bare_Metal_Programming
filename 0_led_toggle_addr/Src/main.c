//led connection
//PA pin5(total 16 pins are there)
//clock through AHB bus
//buses transmit clock for gpio A
//rcc clock -- resetting clock control enables bus for transmitting clock.
//Enable AHB1 from RCC registers for GPIOA port
//use mode register for configuring gpioA pin5

#define PERIPH_BASE 			(0x40000000UL)
#define AHB1PERIPH_OFFSET  		(0x00020000UL)
#define AHB1PERIPH_BASE 		(PERIPH_BASE + AHB1PERIPH_OFFSET)

#define GPIOA_OFFSET 			(0x00000000UL)
#define GPIOA_BASE 				(AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET 				(0x00003800UL)
#define RCC_BASE 				(AHB1PERIPH_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET		(0x00000030UL)
#define RCC_AHB1EN_R			(*(volatile unsigned int *) (RCC_BASE + RCC_AHB1ENR_OFFSET))
#define GPIOAEN 				(1UL << 0) // 0x0000 0001

#define MODE_R_OFFSET 			(0x00UL) // 0x00000000UL
#define GPIOA_MODE_R			(*(volatile unsigned int *)(GPIOA_BASE+MODE_R_OFFSET))

#define OD_R_OFFSET 			(0x14UL)
#define GPIOA_OD_R				(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

#define PIN5					(1U << 5)
#define LED_PIN					(PIN5)


/*
 * (1U << 10) //set bit 10 to 1
 * &=~(1U << 11) // set bit 11 to 0
 *  */


int main(void)
{
	/* 1. Enable clock access to GPIOA*/
	RCC_AHB1EN_R |= GPIOAEN; // | used as RCC_AHB1EN_R has some previous values

	/* 2. Set PA5 as output pin */
	GPIOA_MODE_R |= (1U << 10);
	GPIOA_MODE_R &=~(1U << 11);

	/* 3. Set PA5 high */

	while(1)
	{

		//GPIOA_OD_R |= LED_PIN; // static LEN ON state

		GPIOA_OD_R ^= LED_PIN; // toggle
		for(int i=0; i < (int)1e6; i++)
		{
			//delay for seeing the LED toggle
		}
	}

	return 0;
}







