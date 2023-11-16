/*
 * PORT.c
 *
 * Created: 10/19/2023 10:02:00 PM
 *  Author: Sharpel
 */ 


#include "std_types.h"
#include "bit_math.h"
#include "../includes/PORT_PRV.h"
#include "../includes/Port.h"
#include "../includes/PORT_CFG.h"

/*
NAME: PORT_voidInit
DESC: initilizes all pins according to cfg file
ARGS: void
RET : void

*/
void PORT_voidInit(void)
{
	#if     (PRE_BUILD_STATUS == TRUE)  //case of pre-buils configuration
	
	#undef  PORT_PIN_MODE_INPUT_PULLUP
	#define PORT_PIN_MODE_INPUT_PULLUP        0
	
	#undef  PORT_PIN_MODE_OUTPUT_LOW
	#define PORT_PIN_MODE_OUTPUT_LOW          1
	
	#undef  PORT_PIN_MODE_OUTPUT_HIGH
	#define PORT_PIN_MODE_OUTPUT_HIGH         1
	
	// set direction 
	
	DDRA = CONC(PORTA_PIN7_MODE,PORTA_PIN6_MODE,PORTA_PIN5_MODE,PORTA_PIN4_MODE,PORTA_PIN3_MODE,PORTA_PIN2_MODE,PORTA_PIN1_MODE,PORTA_PIN0_MODE);
	DDRB = CONC(PORTB_PIN7_MODE,PORTB_PIN6_MODE,PORTB_PIN5_MODE,PORTB_PIN4_MODE,PORTB_PIN3_MODE,PORTB_PIN2_MODE,PORTB_PIN1_MODE,PORTB_PIN0_MODE);
	DDRC = CONC(PORTC_PIN7_MODE,PORTC_PIN6_MODE,PORTC_PIN5_MODE,PORTC_PIN4_MODE,PORTC_PIN3_MODE,PORTC_PIN2_MODE,PORTC_PIN1_MODE,PORTC_PIN0_MODE);
	DDRD = CONC(PORTD_PIN7_MODE,PORTD_PIN6_MODE,PORTD_PIN5_MODE,PORTD_PIN4_MODE,PORTD_PIN3_MODE,PORTD_PIN2_MODE,PORTD_PIN1_MODE,PORTD_PIN0_MODE);
	
	
	
	#undef  PORT_PIN_MODE_INPUT_PULLUP
	#define PORT_PIN_MODE_INPUT_PULLUP        1
	
	#undef  PORT_PIN_MODE_OUTPUT_LOW
	#define PORT_PIN_MODE_OUTPUT_LOW          0
	
	#undef  PORT_PIN_MODE_OUTPUT_HIGH
	#define PORT_PIN_MODE_OUTPUT_HIGH         1
	
	// set mode
	
	PORTA = CONC(PORTA_PIN7_MODE,PORTA_PIN6_MODE,PORTA_PIN5_MODE,PORTA_PIN4_MODE,PORTA_PIN3_MODE,PORTA_PIN2_MODE,PORTA_PIN1_MODE,PORTA_PIN0_MODE);
	PORTB = CONC(PORTB_PIN7_MODE,PORTB_PIN6_MODE,PORTB_PIN5_MODE,PORTB_PIN4_MODE,PORTB_PIN3_MODE,PORTB_PIN2_MODE,PORTB_PIN1_MODE,PORTB_PIN0_MODE);
	PORTC = CONC(PORTC_PIN7_MODE,PORTC_PIN6_MODE,PORTC_PIN5_MODE,PORTC_PIN4_MODE,PORTC_PIN3_MODE,PORTC_PIN2_MODE,PORTC_PIN1_MODE,PORTC_PIN0_MODE);
	PORTD = CONC(PORTD_PIN7_MODE,PORTD_PIN6_MODE,PORTD_PIN5_MODE,PORTD_PIN4_MODE,PORTD_PIN3_MODE,PORTD_PIN2_MODE,PORTD_PIN1_MODE,PORTD_PIN0_MODE);
	
	
	// rollback configuration macros
	
	#undef  PORT_PIN_MODE_INPUT_PULLUP
	#undef  PORT_PIN_MODE_OUTPUT_LOW
	#undef  PORT_PIN_MODE_OUTPUT_HIGH
	#define PORT_PIN_MODE_INPUT_PULLUP        1
	#define PORT_PIN_MODE_OUTPUT_LOW          2
	#define PORT_PIN_MODE_OUTPUT_HIGH         3
	
	
	#else   //case of linking time configuration
	#endif
}

/*
NAME: PORT_enu_SetPinDirection
DESC: set pin direction (INPUT,OUTPUT)
ARGS: PORT_enu_PinId_t             -----> pin channel id
      PORT_enu_PinDirection_t      -----> pin direction
RET : PORT_enu_return_status_t status code (STATUS_PORT_OKAY,STATUS_PORT_PIN_NUMBER_ERROR,STATUS_PORT_DIRECTION_CHOICE_ERROR)

*/
PORT_enu_return_status_t  PORT_enu_SetPinDirection(PORT_enu_PinId_t arg_enu_PinId,PORT_enu_PinDirection_t arg_enu_PinDirection)
{
	// loc return status
	PORT_enu_return_status_t loc_enu_return_status = STATUS_PORT_OKAY;
	
	// calculate pin and port number
	u8 loc_u8_portNumber = arg_enu_PinId/PORT_REG_SIZE;
	u8 loc_u8_pinNumber  = arg_enu_PinId%PORT_REG_SIZE;
	
	// case pin number error
	if( arg_enu_PinId >= PORT_PIN_ERROR)
	{
		loc_enu_return_status = STATUS_PORT_PIN_NUMBER_ERROR;
	}
	
	//case direction error
	else if ( arg_enu_PinDirection >= PORT_PIN_DIRECTION_ERROR)
	{
		loc_enu_return_status = STATUS_PORT_DIRECTION_CHOICE_ERROR;
	}
	else
	{
		// switch port
		switch(loc_u8_portNumber)
		{
			case PORTA_VALUE :
			{
				// if input clear bit in DDR reg else set bit 
				(arg_enu_PinDirection == PORT_PIN_DIRECTION_INPUT)?CLR_BIT(DDRA,loc_u8_pinNumber):SET_BIT(DDRA,loc_u8_pinNumber);
				break;
			}
			
			case PORTB_VALUE :
			{
				// if input clear bit in DDR reg else set bit
				(arg_enu_PinDirection == PORT_PIN_DIRECTION_INPUT)?CLR_BIT(DDRB,loc_u8_pinNumber):SET_BIT(DDRB,loc_u8_pinNumber);
				break;
			}
			
			case PORTC_VALUE :
			{
				// if input clear bit in DDR reg else set bit
				(arg_enu_PinDirection == PORT_PIN_DIRECTION_INPUT)?CLR_BIT(DDRC,loc_u8_pinNumber):SET_BIT(DDRC,loc_u8_pinNumber);
				break;
			}
			
			case PORTD_VALUE :
			{
				// if input clear bit in DDR reg else set bit
				(arg_enu_PinDirection == PORT_PIN_DIRECTION_INPUT)?CLR_BIT(DDRD,loc_u8_pinNumber):SET_BIT(DDRD,loc_u8_pinNumber);
				break;
			}
			
			default:
			{
			    break;	
			}
		}
	}
	
	
	return loc_enu_return_status;
}


/*
NAME: PORT_enu_SetPinMode
DESC: set pin mode (INPUT_FLOAT,INPUT_PULLUP,OUTPUT_LOW,OUTPUT_HIGH)
ARGS: PORT_enu_PinId_t             -----> pin channel id
      PORT_enu_PinMode_t           -----> pin mode
RET : PORT_enu_return_status_t status code (STATUS_PORT_OKAY,STATUS_PORT_PIN_NUMBER_ERROR,STATUS_PORT_PINMODE_CHOICE_ERROR)

*/
PORT_enu_return_status_t  PORT_enu_SetPinMode(PORT_enu_PinId_t arg_enu_PinId,PORT_enu_PinMode_t arg_enu_PinMode)
{
	// loc return status code
	PORT_enu_return_status_t loc_enu_return_status = STATUS_PORT_OKAY;
	
	// calculate pin and port number
	u8 loc_u8_portNumber   = arg_enu_PinId / PORT_REG_SIZE;
	u8 loc_u8_pinNumber    = arg_enu_PinId % PORT_REG_SIZE;
	
	// check on pin number
	if( arg_enu_PinId >= PORT_PIN_ERROR)
	{
		loc_enu_return_status = STATUS_PORT_PIN_NUMBER_ERROR;
	}
	// check on pin mode
	else if ( arg_enu_PinMode >= PIN_MODE_MODE_ERROR)
	{
		loc_enu_return_status = STATUS_PORT_PINMODE_CHOICE_ERROR;
	}
	else
	{
		// switch port
		switch(loc_u8_portNumber)
		{
			case PORTA_VALUE :
			{
				if(arg_enu_PinMode == PIN_MODE_INPUT_LOW)
				{
					CLR_BIT(DDRA,loc_u8_pinNumber);
					CLR_BIT(PORTA,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_INPUT_PULLUP)
				{
					CLR_BIT(DDRA,loc_u8_pinNumber);
					SET_BIT(PORTA,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_OUTPUT_HIGH)
				{
					SET_BIT(DDRA,loc_u8_pinNumber);
					SET_BIT(PORTA,loc_u8_pinNumber);
				}
				else
				{
					SET_BIT(DDRA,loc_u8_pinNumber);
					CLR_BIT(PORTA,loc_u8_pinNumber);
				}
				
				break;
			}
			
			case PORTB_VALUE :
			{
				if(arg_enu_PinMode == PIN_MODE_INPUT_LOW)
				{
					CLR_BIT(DDRB,loc_u8_pinNumber);
					CLR_BIT(PORTB,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_INPUT_PULLUP)
				{
					CLR_BIT(DDRB,loc_u8_pinNumber);
					SET_BIT(PORTB,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_OUTPUT_HIGH)
				{
					SET_BIT(DDRB,loc_u8_pinNumber);
					SET_BIT(PORTB,loc_u8_pinNumber);
				}
				else
				{
					SET_BIT(DDRB,loc_u8_pinNumber);
					CLR_BIT(PORTB,loc_u8_pinNumber);
				}
				
				break;
			}
			
			case PORTC_VALUE :
			{
				if(arg_enu_PinMode == PIN_MODE_INPUT_LOW)
				{
					CLR_BIT(DDRC,loc_u8_pinNumber);
					CLR_BIT(PORTC,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_INPUT_PULLUP)
				{
					CLR_BIT(DDRC,loc_u8_pinNumber);
					SET_BIT(PORTC,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_OUTPUT_HIGH)
				{
					SET_BIT(DDRC,loc_u8_pinNumber);
					SET_BIT(PORTC,loc_u8_pinNumber);
				}
				else
				{
					SET_BIT(DDRC,loc_u8_pinNumber);
					CLR_BIT(PORTC,loc_u8_pinNumber);
				}
				break;
			}
			
			case PORTD_VALUE :
			{
				if(arg_enu_PinMode == PIN_MODE_INPUT_LOW)
				{
					CLR_BIT(DDRD,loc_u8_pinNumber);
					CLR_BIT(PORTD,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_INPUT_PULLUP)
				{
					CLR_BIT(DDRD,loc_u8_pinNumber);
					SET_BIT(PORTD,loc_u8_pinNumber);
				}
				else if(arg_enu_PinMode == PIN_MODE_OUTPUT_HIGH)
				{
					SET_BIT(DDRD,loc_u8_pinNumber);
					SET_BIT(PORTD,loc_u8_pinNumber);
				}
				else
				{
					SET_BIT(DDRD,loc_u8_pinNumber);
					CLR_BIT(PORTD,loc_u8_pinNumber);
				}
				break;
			}
			
			default:
			{
			    break;	
			}
		}
	}
	
	
	return loc_enu_return_status;
}

