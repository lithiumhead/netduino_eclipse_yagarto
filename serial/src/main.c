
#define __MAIN_C__


#include "board-netduino.h"
#include "usart0.h"

void wait(void) {
	unsigned int n;
	for (n = 0; n < 500000; n++)
		;
}


int main(void) {

	//Initialize USART0
	init_serial();

	// Loop forever
	for (;;) {
		wait();
		wait();
		wait();
		sendchar('H');
		sendchar('e');
		sendchar('l');
		sendchar('l');
		sendchar('o');
		sendchar(' ');
		sendchar('W');
		sendchar('o');
		sendchar('r');
		sendchar('l');
		sendchar('d');
	}
}

/*** EOF ***/
