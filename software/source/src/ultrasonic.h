/*
 * ultrasonic.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Saloni
 */

#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

//#include "em_leuart.h"


//void initGpio(void);
#if 0
void init_usart2(void);
char get_received_char_usart2(void);
#endif

void ultrasonicInit(void);
void init_leuart(void);
char get_received_char_leuart0(void);

int get_processed_data_from_ultrasonic_sensor(void);



#endif /* SRC_ULTRASONIC_H_ */
