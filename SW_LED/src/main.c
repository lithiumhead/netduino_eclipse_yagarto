
#define __MAIN_C__


#include "AT91SAM7X512.h"


void wait(void) {
	unsigned int n;
	for (n = 0; n < 500000; n++)
		;
}

int main(void) {
	// Enable the Clock of the PIOA and PIOB
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOA;
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB;

	// Configure the PIO Lines corresponding to LED as Output
	// LED is connected to PB23
	AT91C_BASE_PIOB->PIO_PER = (1<<23);			//I/O Line or Peripheral Function Selection - Writing to PIO Enable Register
	AT91C_BASE_PIOB->PIO_OER = (1<<23);			//Output Control - Writing to Output Enable Register

	// Configure the PIO Line corresponding to Switch as Input
	// Switch is connected to PB29
	AT91C_BASE_PIOB->PIO_PER = (1<<29);			//I/O Line or Peripheral Function Selection - Writing to PIO Enable Register
	AT91C_BASE_PIOB->PIO_ODR = (1<<29);			//Output Control - Writing to Output Disable Register
	AT91C_BASE_PIOB->PIO_PER = (1<<29);			//Pull-up Resistor Control - Writing to Pull-up Enable Register

	// Apply 1 to turn on the LED
	AT91C_BASE_PIOB->PIO_SODR = (1<<23);

	// Loop forever
	for (;;) {
		//If switch is pressed
		if ((AT91C_BASE_PIOB->PIO_PDSR & (1<<29)) == 0)		//Check if Pin Data Status Register reads as zero.
		{
			//Turn the LED on
			AT91C_BASE_PIOB->PIO_SODR = (1<<23);	//Set Output Data Register
			wait();
			wait();
			wait();
		}
		else
		{
			//Turn the LED off
			AT91C_BASE_PIOB->PIO_CODR = (1<<23);	//Clear Output Data Register
			wait();
			wait();
			wait();
		}
	}
}

/*** EOF ***/
