/*
 * Lcd_cfg.c
 *
 * Created: 10/21/2023 11:50:15 PM
 *  Author: Sharpel
 */ 

#include "std_types.h"
#include "../../../MCAL/DIO/includes/Dio.h"
#include "../includes/Lcd_prv.h"
#include "../includes/Lcd_cfg.h"



/*
 IMPORTANT : USER MUST MAKE SURE THAT ALL PINS ARE CONFIGURED AS OUTPUT IN PORT_CFG.H FILE
*/

u8 gl_u8_LCD_initSequence[5] = {LCD_CURSOR_HOME,LCD_4_BIT_MODE_SELECTION,LCD_CLEAR_DISPLAY,LCD_ENTRY_MODE_CURSOR_INCREMENT,LCD_DISPLAY_ON_CURSOR_ON_NO_BLINK};
#if LCD_MODE == LCD_4_BIT_MODE

DIO_enu_PinId_t gl_enu_LCD_dataPins[LCD_4_BIT_MODE] = {DIO_PORTB_PIN0,DIO_PORTB_PIN1,DIO_PORTB_PIN2,DIO_PORTB_PIN4};
	
#elif LCD_MODE == LCD_8_BIT_MODE

DIO_enu_PinId_t gl_enu_LCD_dataPins[LCD_8_BIT_MODE] =
{
	DIO_PORTA_PIN0,DIO_PORTA_PIN1,DIO_PORTA_PIN2,DIO_PORTA_PIN3,
	DIO_PORTA_PIN4,DIO_PORTA_PIN5,DIO_PORTA_PIN6,DIO_PORTA_PIN7,
};
#else

#error "Configuration Error see LCD_MODE MACRO"
#endif

