/*
 * PORT.h
 *
 * Created: 10/19/2023 9:58:02 PM
 *  Author: Sharpel
 */ 


#ifndef PORT_H_
#define PORT_H_


// enum for all channels
typedef enum
{
	//PORT A
	PORTA_PIN0 = 0,
	PORTA_PIN1,
	PORTA_PIN2,
	PORTA_PIN3,
	PORTA_PIN4,
	PORTA_PIN5,
	PORTA_PIN6,
	PORTA_PIN7,
	
	//PORT B
	PORTB_PIN0,
	PORTB_PIN1,
	PORTB_PIN2,
	PORTB_PIN3,
	PORTB_PIN4,
	PORTB_PIN5,
	PORTB_PIN6,
	PORTB_PIN7,
	
	//PORT C
	PORTC_PIN0,
	PORTC_PIN1,
	PORTC_PIN2,
	PORTC_PIN3,
	PORTC_PIN4,
	PORTC_PIN5,
	PORTC_PIN6,
	PORTC_PIN7,
	
	//PORT D
	PORTD_PIN0,
	PORTD_PIN1,
	PORTD_PIN2,
	PORTD_PIN3,
	PORTD_PIN4,
	PORTD_PIN5,
	PORTD_PIN6,
	PORTD_PIN7,
	
	// ERROR CHECK
	PORT_PIN_ERROR
	
}PORT_enu_PinId_t;

// enum for pin direction
typedef enum
{
	PORT_PIN_DIRECTION_INPUT,
	PORT_PIN_DIRECTION_OUTPUT,
	PORT_PIN_DIRECTION_ERROR,
	
}PORT_enu_PinDirection_t;

// enum for mode
typedef enum
{
   PIN_MODE_INPUT_LOW,
   PIN_MODE_INPUT_PULLUP,
   PIN_MODE_OUTPUT_HIGH,
   PIN_MODE_OUTPUT_LOW,
   PIN_MODE_MODE_ERROR,
	
}PORT_enu_PinMode_t;

// enum for error status
typedef enum 
{
	STATUS_PORT_OKAY,
	STATUS_PORT_PIN_NUMBER_ERROR,
	STATUS_PORT_DIRECTION_CHOICE_ERROR,
	STATUS_PORT_PINMODE_CHOICE_ERROR,
	
}PORT_enu_return_status_t;

/*
NAME: PORT_voidInit
DESC: initilizes all pins according to cfg file
ARGS: void
RET : void

*/
void                      PORT_voidInit(void);

/*
NAME: PORT_enu_SetPinDirection
DESC: set pin direction (INPUT,OUTPUT)
ARGS: PORT_enu_PinId_t             -----> pin channel id
      PORT_enu_PinDirection_t      -----> pin direction
RET : PORT_enu_return_status_t status code (STATUS_PORT_OKAY,STATUS_PORT_PIN_NUMBER_ERROR,STATUS_PORT_DIRECTION_CHOICE_ERROR)

*/
PORT_enu_return_status_t  PORT_enu_SetPinDirection(PORT_enu_PinId_t arg_enu_PinId,PORT_enu_PinDirection_t arg_enu_PinDirection);

/*
NAME: PORT_enu_SetPinMode
DESC: set pin mode (INPUT_FLOAT,INPUT_PULLUP,OUTPUT_LOW,OUTPUT_HIGH)
ARGS: PORT_enu_PinId_t             -----> pin channel id
      PORT_enu_PinMode_t           -----> pin mode
RET : PORT_enu_return_status_t status code (STATUS_PORT_OKAY,STATUS_PORT_PIN_NUMBER_ERROR,STATUS_PORT_PINMODE_CHOICE_ERROR)

*/
PORT_enu_return_status_t  PORT_enu_SetPinMode(PORT_enu_PinId_t arg_enu_PinId,PORT_enu_PinMode_t arg_enu_PinMode);




#endif /* PORT_H_ */