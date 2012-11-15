#include "AT91SAM7X512.h"


//LEDs Definition
#define AT91B_LED				(1<<23)       // AT91C_PIO_PB23 AT91C_PB23_PWM0 AT91C_PB23_TCLK1
#define AT91B_NB_LEB			1		   		// Number of LEDs
#define AT91B_LED_MASK			(AT91B_LED)
#define AT91D_BASE_PIO_LED		(AT91C_BASE_PIOB)

//Switch Definitions
#define AT91B_SWITCH1			(1<<21)  // PA21 User switch	  AT91C_PA21_TF  AT91C_PA21_NPCS10
#define AT91B_SW_MASK			(AT91B_SWITCH1)
#define AT91D_BASE_PIO_SW		(AT91C_BASE_PIOA)

//Clocks
#define AT91B_MAIN_OSC			18432000               // Main Oscillator MAINCK
#define AT91B_MCK				((18432000*73/14)/2)   // Output PLL Clock


void wait(void) {
	unsigned int n;
	for (n = 0; n < 500000; n++)
		;
}

int main(void) {
	// Enable the Clock of the PIO
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOA;
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB;

	// Configure the PIO Lines corresponding to LED as Output
	AT91C_BASE_PIOB->PIO_PER = AT91B_LED_MASK;
	AT91C_BASE_PIOB->PIO_OER = AT91B_LED_MASK;

	// Set the LED's. On the Board we must apply a "1" to turn on LEDs
	AT91C_BASE_PIOB->PIO_SODR = AT91B_LED_MASK;

	// Loop forever
	for (;;) {
		AT91C_BASE_PIOB->PIO_CODR = AT91B_LED_MASK;
		wait();
		AT91C_BASE_PIOB->PIO_SODR = AT91B_LED_MASK;
		wait();
	}
}

/*** EOF ***/
