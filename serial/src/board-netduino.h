
#ifndef BOARD_H_
#define BOARD_H_

/* netduino has a AT91SAM7X512 onboard */
#include "AT91SAM7X512.h"

/* Frequency of crystal connected to the microcontroller on the board */
#define BOARD_MAINCK       18432000		// Main Clock (MAINCK) generated by Main Ocillator from external crystal of 18.432 MHz

/* Startup time of main oscillator (in number of slow clock ticks). */
#define BOARD_OSCOUNT   (AT91C_CKGR_OSCOUNT & (0x40 << 8)) 	// 0x40 means 64 slow clock ticks

/* PLL frequency range. */
#define BOARD_CKGR_PLL  AT91C_CKGR_OUT_0

/* PLL startup time (in number of slow clock ticks). */
#define BOARD_PLLCOUNT  (16 << 8)		//16 slow clock ticks

/* PLL MUL & DIV values. */
/* We have 18.432 MHz Crystal onboard the netduino and we want to use USB so lets target getting 96 MHz*/
/* We will devie this 96 MHz by 2 to 48 MHz to supply clock to the USB section*/
#define PLL_MULTIPLIER 	72
#define PLL_DIVIDER 	14
#define BOARD_PLLCK     ( BOARD_MAINCK * ((PLL_MULTIPLIER+1)/PLL_DIVIDER) ) //Comes to 96.10971429 MHz for 18.432 MHz Crystal
#define BOARD_MUL       (AT91C_CKGR_MUL & (PLL_MULTIPLIER << 16))
#define BOARD_DIV       (AT91C_CKGR_DIV & PLL_DIVIDER)

/* USB PLL divisor value to obtain a 48MHz clock. */
#define BOARD_USBDIV    AT91C_CKGR_USBDIV_1 	//PLLCK output will be divided by 2. Comes to 48.05485714 MHz for 18.432 MHz Crystal

/* Master Clock Prescaler value (PRES field of PMC_MCKR). */
#define PRESCALER_CALC(X) ((X == 0) ? 0 : ((X == 2) ? 1 : ((X == 4) ? 2 : ((X == 8) ? 3 : ((X == 16) ? 4 : ((X == 32) ? 5 : ((X == 64) ? 6 : 0 )))))))
#define BOARD_PRESCALER_VALUE		2		//Prescaler whole number value
#define BOARD_PRESCALER (PRESCALER_CALC(BOARD_PRESCALER_VALUE) << 2)	//Bit pattern to be set in the register

/* Final Master Clock output by the Master Clock Controller */
#define BOARD_MCK     (BOARD_PLLCK/BOARD_PRESCALER_VALUE)   //Comes to 48.05485714 MHz for 18.432 MHz Crystal

#endif /* BOARD_H_ */
