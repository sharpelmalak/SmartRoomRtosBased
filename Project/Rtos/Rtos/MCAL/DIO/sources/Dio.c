/*
 * Dio.c
 *
 * Created: 10/20/2023 12:21:14 AM
 *  Author: sharpel
 */ 

#include "std_types.h"
#include "bit_math.h"
#include "../includes/Dio_prv.h"
#include "../includes/Dio.h"
#include "../includes/Dio_cfg.h"




/*
NAME: DIO_enu_WriteChannel
DESC: write Logic (HIGH,LOW) on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
      DIO_enu_PinLevel_t   ----> Logic
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR,STATUS_DIO_PIN_LEVEL_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_WriteChannel(DIO_enu_PinId_t arg_enu_pinId,DIO_enu_PinLevel_t arg_enu_pinLevel)
{
	
	// loc return status
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// calculate pin number and port number
	u8 loc_u8_pinNumber  = arg_enu_pinId % REG_SIZE ;
	u8 loc_u8_portNumber = arg_enu_pinId / REG_SIZE ;
	
	// case wrong pin
	if( arg_enu_pinId >= DIO_PINID_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PIN_NUMBER_ERROR;
	}
	// case wrong level
	else if( arg_enu_pinLevel >= DIO_PIN_LEVEL_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PIN_LEVEL_CHOICE_ERROR;
	}
	else
	{
		// switch port number
		switch(loc_u8_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// if low level clear bit in PORT reg else set bit 
				(arg_enu_pinLevel == DIO_PIN_LEVEL_LOW)?CLR_BIT(DIO_PORTA,loc_u8_pinNumber):SET_BIT(DIO_PORTA,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// if low level clear bit in PORT reg else set bit 
				(arg_enu_pinLevel == DIO_PIN_LEVEL_LOW)?CLR_BIT(DIO_PORTB,loc_u8_pinNumber):SET_BIT(DIO_PORTB,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// if low level clear bit in PORT reg else set bit 
				(arg_enu_pinLevel == DIO_PIN_LEVEL_LOW)?CLR_BIT(DIO_PORTC,loc_u8_pinNumber):SET_BIT(DIO_PORTC,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// if low level clear bit in PORT reg else set bit 
				(arg_enu_pinLevel == DIO_PIN_LEVEL_LOW)?CLR_BIT(DIO_PORTD,loc_u8_pinNumber):SET_BIT(DIO_PORTD,loc_u8_pinNumber);
				break;
			}
			
		    
		}
	}
	
	
	return loc_enu_return_status;
}


/*
NAME: DIO_enu_ReadChannel
DESC: read Logic (HIGH,LOW) on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
      u8 *                 ----> pointer to varibale which will hold level (HIGH,LOW)
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR,STATUS_DIO_NULL_PTR_ERROR)

*/
DIO_enu_return_status_t DIO_enu_ReadChannel(DIO_enu_PinId_t arg_enu_pinId,u8 *ptr_value)
{
	//loc for return status
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// calculate pin and port numbers
	u8 loc_u8_pinNumber  = arg_enu_pinId % REG_SIZE ;
	u8 loc_u8_portNumber = arg_enu_pinId / REG_SIZE ;
	
	// check if address is null
	if(ptr_value == PTR_NULL)
	{
		loc_enu_return_status = STATUS_DIO_NULL_PTR_ERROR;
	}
	// check on channel
	else if( arg_enu_pinId >= DIO_PINID_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PIN_NUMBER_ERROR;
	}
	else
	{
		switch(loc_u8_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// asign level 
				*ptr_value = GET_BIT(DIO_PINA,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// asign level 
				*ptr_value = GET_BIT(DIO_PINB,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// asign level 
				*ptr_value = GET_BIT(DIO_PINC,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// asign level 
				*ptr_value = GET_BIT(DIO_PIND,loc_u8_pinNumber);
				break;
			}
			
		}
	}
	
	
	return loc_enu_return_status;
}

/*
NAME: DIO_enu_WritePort
DESC: write value on specific DIO_PORT
ARGS: DIO_enu_PortNumber_t      ----> port
      u8 value                  ----> the value that will assign to the port
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_WritePort(DIO_enu_PortNumber_t arg_enu_portNumber,u8 arg_u8_value)
{
	
	// loc return status code
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// check on port number
	if( arg_enu_portNumber >= DIO_PORT_NUMBER_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PORT_NUMBER_CHOICE_ERROR;
	}
	else
	{
		//switch port number
		switch(arg_enu_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// assign value in the port
				ASSIGN_REG(DIO_PORTA,arg_u8_value);
 				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// assign value in the port
				ASSIGN_REG(DIO_PORTB,arg_u8_value);
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// assign value in the port
				ASSIGN_REG(DIO_PORTC,arg_u8_value);
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// assign value in the port
				ASSIGN_REG(DIO_PORTD,arg_u8_value);
				break;
			}
			
			
		}
	}
	
	
	return loc_enu_return_status;
}

/*
NAME: DIO_enu_ReadPort
DESC: read DIO_PORT and assign it in variable
ARGS: DIO_enu_PortNumber_t      ----> port
      u8 *                      ----> pointer to varibale which will hold value
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR,STATUS_DIO_NULL_PTR_ERROR)

*/
DIO_enu_return_status_t DIO_enu_ReadPort(DIO_enu_PortNumber_t arg_enu_portNumber,u8 *ptr_value)
{
	// loc return status
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// check if ptr is null
	if(ptr_value == PTR_NULL)
	{
		loc_enu_return_status = STATUS_DIO_NULL_PTR_ERROR;
	}
	//check on port number
	else if( arg_enu_portNumber >= DIO_PORT_NUMBER_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PORT_NUMBER_CHOICE_ERROR;
	}
	else
	{
		// switch port
		switch(arg_enu_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// assign port value in the pointer
				*ptr_value = DIO_PINA;
				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// assign port value in the pointer
				*ptr_value = DIO_PINB;
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// assign port value in the pointer
				*ptr_value =DIO_PINC;
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// assign port value in the pointer
				*ptr_value = DIO_PIND;
				break;
			}
			
		
		}
	}
	
	
	return loc_enu_return_status;
}

/*
NAME: DIO_enu_FlipChannel
DESC: flip Logic on specific DIO_CHANNEL
ARGS: DIO_enu_PinId_t      ----> channel
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PIN_NUMBER_ERROR)

*/
DIO_enu_return_status_t DIO_enu_FlipChannel(DIO_enu_PinId_t arg_enu_pinId)
{
	// loc return statues code
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// calculate pin and port number
	u8 loc_u8_pinNumber  = arg_enu_pinId % REG_SIZE ;
	u8 loc_u8_portNumber = arg_enu_pinId / REG_SIZE ;
	
	// case wrong pin number
	if( arg_enu_pinId >= DIO_PINID_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PIN_NUMBER_ERROR;
	}
	else
	{
		// switch port
		switch(loc_u8_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// toggle bit
				TGL_BIT(DIO_PORTA,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// toggle bit
				TGL_BIT(DIO_PORTB,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// toggle bit
				TGL_BIT(DIO_PORTC,loc_u8_pinNumber);
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// toggle bit
				TGL_BIT(DIO_PORTD,loc_u8_pinNumber);
				break;
			}
			
		
		}
	}
	
	
	return loc_enu_return_status;
}

/*
NAME: DIO_enu_FlipPort
DESC: flip DIO_PORT
ARGS: DIO_enu_PortNumber_t      ----> port
RET : DIO_enu_return_status_t (STATUS_DIO_OKAY,STATUS_DIO_PORT_NUMBER_CHOICE_ERROR)

*/
DIO_enu_return_status_t DIO_enu_FlipPort(DIO_enu_PortNumber_t arg_enu_portNumber)
{
	
	// loc return status code
	DIO_enu_return_status_t loc_enu_return_status = STATUS_DIO_OKAY;
	
	// case wrong port number
	if( arg_enu_portNumber >= DIO_PORT_NUMBER_ERROR)
	{
		loc_enu_return_status = STATUS_DIO_PORT_NUMBER_CHOICE_ERROR;
	}
	else
	{
		// switch port
		switch(arg_enu_portNumber)
		{
			case DIO_PORT_NUMBER_A :
			{
				// toggle port
				ASSIGN_REG(DIO_PORTA,(~DIO_PORTA));
				break;
			}
			
			case DIO_PORT_NUMBER_B :
			{
				// toggle port
				ASSIGN_REG(DIO_PORTB,(~DIO_PORTB));
				break;
			}
			
			case DIO_PORT_NUMBER_C :
			{
				// toggle port
				ASSIGN_REG(DIO_PORTC,(~DIO_PORTC));
				break;
			}
			
			case DIO_PORT_NUMBER_D :
			{
				// toggle port
				ASSIGN_REG(DIO_PORTD,(~DIO_PORTD));
				break;
			}
			
			
		}
	}
	
	
	return loc_enu_return_status;
}
