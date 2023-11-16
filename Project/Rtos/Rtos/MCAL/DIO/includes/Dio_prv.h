/*
 * Dio_prv.h
 *
 * Created: 10/20/2023 12:36:08 AM
 *  Author: sharb
 */ 


#ifndef DIO_PRV_H_
#define DIO_PRV_H_


/*
 * PORT_PRV.h
 *
 * Created: 10/19/2023 10:00:52 PM
 *  Author: Sharpel
 */ 

#include "std_types.h"





#define DIO_PORTA	(*(vu8*)(0x3B))
#define DIO_PINA	(*(vu8*)(0x39))
#define DIO_PORTB	(*(vu8*)(0x38))
#define DIO_PINB	(*(vu8*)(0x36))
#define DIO_PORTC	(*(vu8*)(0x35))
#define DIO_PINC	(*(vu8*)(0x33))
#define DIO_PORTD	(*(vu8*)(0x32))
#define DIO_PIND	(*(vu8*)(0x30))



#define REG_SIZE    (8U)


#endif /* DIO_PRV_H_ */