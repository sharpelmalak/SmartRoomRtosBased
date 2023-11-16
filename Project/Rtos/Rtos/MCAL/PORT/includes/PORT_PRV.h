/*
 * PORT_PRV.h
 *
 * Created: 10/19/2023 10:00:52 PM
 *  Author: Sharpel
 */ 

#include "std_types.h"


#ifndef PORT_PRV_H_
#define PORT_PRV_H_


#define PORTA	(*(vu8*)(0x3B))
#define DDRA	(*(vu8*)(0x3A))
#define PINA	(*(vu8*)(0x39))

#define PORTB	(*(vu8*)(0x38))
#define DDRB	(*(vu8*)(0x37))
#define PINB	(*(vu8*)(0x36))

#define PORTC	(*(vu8*)(0x35))
#define DDRC	(*(vu8*)(0x34))
#define PINC	(*(vu8*)(0x33))

#define PORTD	(*(vu8*)(0x32))
#define DDRD	(*(vu8*)(0x31))
#define PIND	(*(vu8*)(0x30))


#define PORT_PIN_MODE_INPUT_FLOATING      0
#define PORT_PIN_MODE_INPUT_PULLUP        1
#define PORT_PIN_MODE_OUTPUT_LOW          2
#define PORT_PIN_MODE_OUTPUT_HIGH         3


#define PORT_REG_SIZE                     (8U)
#define PORTA_VALUE                       (0U)
#define PORTB_VALUE                       (1U)
#define PORTC_VALUE                       (2U)
#define PORTD_VALUE                       (3U)


#endif /* PORT_PRV_H_ */