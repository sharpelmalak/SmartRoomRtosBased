/*
 * Dio.h
 *
 * Created: 10/20/2023 12:20:42 AM
 *  Author: sharb
 */ 


#ifndef DIO_H_
#define DIO_H_




// enum for all DIO channels
typedef enum
{
	//PORT A
	DIO_PORTA_PIN0 = 0,
	DIO_PORTA_PIN1,
	DIO_PORTA_PIN2,
	DIO_PORTA_PIN3,
	DIO_PORTA_PIN4,
	DIO_PORTA_PIN5,
	DIO_PORTA_PIN6,
	DIO_PORTA_PIN7,
	
	//PORT B
	DIO_PORTB_PIN0,
	DIO_PORTB_PIN1,
	DIO_PORTB_PIN2,
	DIO_PORTB_PIN3,
	DIO_PORTB_PIN4,
	DIO_PORTB_PIN5,
	DIO_PORTB_PIN6,
	DIO_PORTB_PIN7,
	
	//PORT C
	DIO_PORTC_PIN0,
	DIO_PORTC_PIN1,
	DIO_PORTC_PIN2,
	DIO_PORTC_PIN3,
	DIO_PORTC_PIN4,
	DIO_PORTC_PIN5,
	DIO_PORTC_PIN6,
	DIO_PORTC_PIN7,
	
	//PORT D
	DIO_PORTD_PIN0,
	DIO_PORTD_PIN1,
	DIO_PORTD_PIN2,
	DIO_PORTD_PIN3,
	DIO_PORTD_PIN4,
	DIO_PORTD_PIN5,
	DIO_PORTD_PIN6,
	DIO_PORTD_PIN7,
	
	// ERROR CHECK
	DIO_PINID_ERROR
	
}DIO_enu_PinId_t;



// enum for DIO PORTS i.e groups
typedef enum
{
	DIO_PORT_NUMBER_A,
	DIO_PORT_NUMBER_B,
	DIO_PORT_NUMBER_C,
	DIO_PORT_NUMBER_D,
	DIO_PORT_NUMBER_ERROR,
}DIO_enu_PortNumber_t;


//enum for DIO pin Logic
typedef enum
{
  DIO_PIN_LEVEL_LOW = 0,
  DIO_PIN_LEVEL_HIGH,
  DIO_PIN_LEVEL_ERROR,
  
	
}DIO_enu_PinLevel_t;


//enum for return status code
typedef enum
{
	STATUS_DIO_OKAY,
	STATUS_DIO_PIN_NUMBER_ERROR,
	STATUS_DIO_PIN_LEVEL_CHOICE_ERROR,
	STATUS_DIO_PORT_NUMBER_CHOICE_ERROR,
	STATUS_DIO_NULL_PTR_ERROR,
	
}DIO_enu_return_status_t;


/*
NAME: DIO_enu_WriteChannel
DESC: write Logic (HIGH,LOW) on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
      DIO_enu_PinLevel_t   ----> Logic
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR,STATUS_DIO_PIN_LEVEL_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_WriteChannel(DIO_enu_PinId_t arg_enu_pinId,DIO_enu_PinLevel_t arg_enu_pinLevel);

/*
NAME: DIO_enu_ReadChannel
DESC: read Logic (HIGH,LOW) on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
      u8 *                 ----> pointer to varibale which will hold level (HIGH,LOW)
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR,STATUS_DIO_NULL_PTR_ERROR)

*/
DIO_enu_return_status_t DIO_enu_ReadChannel(DIO_enu_PinId_t arg_enu_pinId,u8 *ptr_value);

/*
NAME: DIO_enu_WritePort
DESC: write value on specific DIO_PORT
ARGS: DIO_enu_PortNumber_t      ----> port
      u8 value                  ----> the value that will assign to the port
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_WritePort(DIO_enu_PortNumber_t arg_enu_portNumber,u8 arg_u8_value);

/*
NAME: DIO_enu_ReadPort
DESC: read DIO_PORT and assign it in variable
ARGS: DIO_enu_PortNumber_t      ----> port
      u8 *                      ----> pointer to varibale which will hold value
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR,STATUS_DIO_NULL_PTR_ERROR)

*/
DIO_enu_return_status_t DIO_enu_ReadPort(DIO_enu_PortNumber_t arg_enu_portNumber,u8 *ptr_value);

/*
NAME: DIO_enu_FlipChannel
DESC: flip Logic on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR)

*/
DIO_enu_return_status_t DIO_enu_FlipChannel(DIO_enu_PinId_t arg_enu_pinId);

/*
NAME: DIO_enu_FlipPort
DESC: flip DIO_PORT
ARGS: DIO_enu_PortNumber_t      ----> port
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_FlipPort(DIO_enu_PortNumber_t arg_enu_portNumber);

#endif /* DIO_H_ */