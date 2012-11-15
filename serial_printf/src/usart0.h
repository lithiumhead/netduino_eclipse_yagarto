/*
 * usart0.h
 *
 *  Created on: 07-Mar-2012
 *      Author: Anurag
 */

#ifndef USART0_H_
#define USART0_H_

void init_serial(void);
int sendchar(int ch);
int getkey(void);

#endif /* USART0_H_ */
