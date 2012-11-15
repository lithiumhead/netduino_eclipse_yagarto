#include "board-netduino.h"
#include "usart0.h"

#define BR    115200                            		/* Baud Rate */
#define BRD  ((BOARD_MCK/16)/BR)                  		/* Baud Rate Divisor */

AT91S_USART * pUSART = AT91C_BASE_US0;					/* Global Pointer to USART0 */

void init_serial(void) {								/* Initialize Serial Interface */

	*AT91C_PMC_PCER = (1 << AT91C_ID_PIOA) |      		/* Enable Clock for PIOA   */
					  (1 << AT91C_ID_US0);        		/* Enable Clock for USART0 */

	*AT91C_PIOA_PDR = AT91C_PA0_RXD0 |            		/* Enable RxD0 Pin */
	                  AT91C_PA1_TXD0;            		/* Enalbe TxD0 Pin */

	pUSART->US_CR = AT91C_US_RSTRX | 					/* Reset Receiver      */
	AT91C_US_RSTTX | 									/* Reset Transmitter   */
	AT91C_US_RXDIS | 									/* Receiver Disable    */
	AT91C_US_TXDIS; 									/* Transmitter Disable */

	pUSART->US_MR = AT91C_US_USMODE_NORMAL |			/* Normal Mode */
	AT91C_US_CLKS_CLOCK |								/* Clock = MCK */
	AT91C_US_CHRL_8_BITS |								/* 8-bit Data  */
	AT91C_US_PAR_NONE |									/* No Parity   */
	AT91C_US_NBSTOP_1_BIT;								/* 1 Stop Bit  */

	pUSART->US_BRGR = BRD;								/* Baud Rate Divisor */

	pUSART->US_CR = AT91C_US_RXEN |						/* Receiver Enable     */
	AT91C_US_TXEN;										/* Transmitter Enable  */
}

int sendchar(int ch) {									/* Write character to Serial Port */

	if (ch == '\n') {									/* Check for CR */
		while (!(pUSART->US_CSR & AT91C_US_TXRDY))
			;											/* Wait for Empty Tx Buffer */
		pUSART->US_THR = '\r';							/* Output CR */
	}
	while (!(pUSART->US_CSR & AT91C_US_TXRDY))
		;												/* Wait for Empty Tx Buffer */
	return (pUSART->US_THR = ch);						/* Transmit Character */
}

int getkey(void) {										/* Read character from Serial Port */

	while (!(pUSART->US_CSR & AT91C_US_RXRDY))
		;												/* Wait for Full Rx Buffer */
	return (pUSART->US_RHR);							/* Read Character */
}
