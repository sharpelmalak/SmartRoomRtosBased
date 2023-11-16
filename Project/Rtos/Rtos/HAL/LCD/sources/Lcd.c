/*
 * Lcd.c
 *
 * Created: 10/20/2023 5:06:14 AM
 *  Author: sharb
 */ 

#include "std_types.h"
#include "bit_math.h"
#include "../../../MCAL/DIO/includes/Dio.h"
#include "../includes/Lcd_prv.h"
#include "../includes/Lcd.h"
#include "../includes/Lcd_cfg.h"

#include "util/delay.h"

extern DIO_enu_PinId_t gl_enu_LCD_dataPins[5];
extern u8              gl_u8_LCD_initSequence[LCD_4_BIT_MODE];


LCD_API_STATE_t  LCD_send(LCD_REQUEST_t request,u8 data)
{
	  u8              loc_u8_itrator   = FALSE;
	  static u8       loc_u8_Control   = FALSE;
	  LCD_API_STATE_t loc_enu_APiState = LCD_API_WAITING;
	  if (loc_u8_Control == FALSE)
	  {
		  DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
		  if ( request == LCD_CMD)
		  {
			  DIO_enu_WriteChannel(RS_PIN,DIO_PIN_LEVEL_LOW); // CHOOSE COMMAND
		  }
		  else if (request == LCD_DATA)
		  {
			  DIO_enu_WriteChannel(RS_PIN,DIO_PIN_LEVEL_HIGH); // CHOOSE DATA
		  }
		  for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_4_BIT_MODE ; loc_u8_itrator++ )
		  {
			  DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(data,loc_u8_itrator));
		  }
		  DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable High
		  //loc_u8_Control = TRUE;
		  loc_enu_APiState = LCD_API_JOB_DONE;
	  }
	  
	  else
	  {
		  DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
		   loc_u8_Control   = FALSE;
		   loc_enu_APiState = LCD_API_JOB_DONE;
	  }
	 return loc_enu_APiState;
}
LCD_API_STATE_t  LCD_enu_InitWithOs(void)
{
   LCD_API_STATE_t loc_enu_initState  = LCD_API_WAITING;
   LCD_API_STATE_t loc_enu_sendState  = LCD_API_WAITING;
   static u8       loc_u8_ApiControl  = FALSE;
   static u8       loc_u8_sendControl = FALSE;
   if (loc_u8_sendControl == FALSE) // FOR HIGH NIBBLE
   {
       loc_enu_sendState = LCD_send(LCD_CMD,gl_u8_LCD_initSequence[loc_u8_ApiControl]>>4);
       if (loc_enu_sendState == LCD_API_JOB_DONE)
       {
           loc_u8_sendControl = TRUE;
       }
   }
   else // FOR LOW NIBBLE
   {
       loc_enu_sendState = LCD_send(LCD_CMD,gl_u8_LCD_initSequence[loc_u8_ApiControl]);
       if (loc_enu_sendState == LCD_API_JOB_DONE)
       {
		   loc_u8_sendControl = FALSE;
		   if (loc_u8_ApiControl == 4)
		   {
			   loc_enu_initState  = LCD_API_JOB_DONE;
		   }
	   }
       
   }

   if (loc_enu_sendState == LCD_API_JOB_DONE && loc_u8_sendControl == FALSE)
   loc_u8_ApiControl++;

   
   return loc_enu_initState;
}
LCD_API_STATE_t  LCD_writeString(u8 *arg_u8_string)
{
	LCD_API_STATE_t loc_return = LCD_API_WAITING;
	static u8 sloc_u8_itartor = FALSE;
	static u8 state           = FALSE;
	if (state == FALSE && arg_u8_string[sloc_u8_itartor] != STR_NULL)
	{
		LCD_send(LCD_DATA,arg_u8_string[sloc_u8_itartor]>>4);
		state = TRUE;
	}
	else if (state == TRUE)
	{
		LCD_send(LCD_DATA,arg_u8_string[sloc_u8_itartor++]);
		state = FALSE;
		if (arg_u8_string[sloc_u8_itartor] == STR_NULL)
		{
			loc_return = LCD_API_JOB_DONE;
			sloc_u8_itartor = FALSE;
		}
	}
	
	return loc_return;
}
LCD_API_STATE_t LCD_writeNumber(s32 arg_s32_number)
{
	LCD_API_STATE_t loc_return = LCD_API_WAITING;
	static u8 sloc_u8_itartor = FALSE;
	static u8 state           = FALSE;
	static u32 loc_u8_inverse = TRUE;
	if (arg_s32_number == 0)
	{
		if (state == FALSE)
		{
			LCD_send(LCD_DATA,'0'>>4);
			state = TRUE;
		}
		else
		{
			LCD_send(LCD_DATA,'0');
			state = FALSE;
			loc_return = LCD_API_JOB_DONE;
		}
		
	}
	else
	{
		if (state == FALSE)
		{
			while(arg_s32_number != FALSE)
			{
				loc_u8_inverse*=10;
				loc_u8_inverse+=arg_s32_number%10;
				arg_s32_number/=10;
			}
			state = TRUE;
		}
		else
		{
			if (loc_u8_inverse != TRUE && sloc_u8_itartor == FALSE)
			{
				LCD_send(LCD_DATA,((loc_u8_inverse%10)+'0')>>4);
				sloc_u8_itartor = TRUE;
			}
			else if (loc_u8_inverse != TRUE && sloc_u8_itartor == TRUE)
			{
				LCD_send(LCD_DATA,((loc_u8_inverse%10)+'0'));
				loc_u8_inverse/=10;
				sloc_u8_itartor = FALSE;
			}
			else if (loc_u8_inverse == TRUE)
			{
				state = FALSE;
				sloc_u8_itartor = FALSE;
				loc_return = LCD_API_JOB_DONE;
			}
		}
		
		
		
	}
	return loc_return;
}
LCD_API_STATE_t LCD_CreateCustomCharacter(u8 *arg_u8_data_array,u8 arg_u8_index)
{
	LCD_API_STATE_t loc_enu_createReturn = LCD_API_WAITING;
	static u8       sloc_u8_createState  = FALSE;
	static u8       sloc_u8_itrator      = FALSE;
	if (arg_u8_index < CGRAM_MAX_INDEX)  // CGRAM only store 8 characters
	{
		if (sloc_u8_createState == FALSE)
		{
			LCD_send(LCD_CMD,(LCD_SET_CGRAM_ADDR_CMD + (arg_u8_index*CGRAM_INDEX_BYTES))>>4);
			sloc_u8_createState = TRUE;
		}
		else if (sloc_u8_createState == TRUE)
		{
			LCD_send(LCD_CMD,(LCD_SET_CGRAM_ADDR_CMD + (arg_u8_index*CGRAM_INDEX_BYTES)));
			sloc_u8_createState = 2;
		}
		else if (sloc_u8_createState == 2)
		{
			LCD_send(LCD_DATA,(arg_u8_data_array[sloc_u8_itrator]>>4));
			sloc_u8_createState =3;
		}
		else if (sloc_u8_createState == 3)
		{
			LCD_send(LCD_DATA,(arg_u8_data_array[sloc_u8_itrator++]));
			sloc_u8_createState =2;
		}
		if (sloc_u8_itrator == CGRAM_INDEX_BYTES )
		{
			LCD_send(LCD_CMD,LCD_SET_DDRAM_ADDR_CMD>>4);
			sloc_u8_itrator = 0;
			sloc_u8_createState = 4;
		}
		else if (sloc_u8_createState == 4)
		{
			LCD_send(LCD_CMD,LCD_SET_DDRAM_ADDR_CMD);
			sloc_u8_createState = 0;
			loc_enu_createReturn= LCD_API_JOB_DONE;
		}
		
	}
		return loc_enu_createReturn;
		
}
/*
 * function		: LCD_vidInit
 * description 	: func to set LCD initialization 
                  before you call this function make sure that chosen pins in cfg.h and cfg.c are configured as output_low in PORT_cfg.h
 * input param 	: void
 * return		: void
 * */

void LCD_vidInit(void)
{
    
	LCD_vidSendCommand(LCD_CURSOR_HOME);
	#if   LCD_MODE == LCD_4_BIT_MODE
	LCD_vidSendCommand(LCD_4_BIT_MODE_SELECTION);
	#elif LCD_MODE == LCD_8_BIT_MODE
	LCD_vidSendCommand(LCD_8_BIT_MODE_SELECTION);
	#else
	#error "LCD MODE ERROR CHECK MACRO IN Lcd_cfg.h file"
	#endif
	
	LCD_vidSendCommand(LCD_CLEAR_DISPLAY);
	_delay_ms(4);

	LCD_vidSendCommand(LCD_ENTRY_MODE_CURSOR_INCREMENT);
	_delay_ms(1);

	LCD_vidSendCommand(LCD_DISPLAY_ON_CURSOR_ON_NO_BLINK);
	_delay_ms(1);
}
void LCD_vidSendCommand(u8 arg_u8_command)
{
	u8 loc_u8_itrator;
	DIO_enu_WriteChannel(RS_PIN,DIO_PIN_LEVEL_LOW); // CHOOSE COMMAND
	//DIO_enu_WriteChannel(RW_PIN,DIO_PIN_LEVEL_LOW); // CHOOSE WRITE
	#if   LCD_MODE == LCD_4_BIT_MODE
	u8 loc_u8_temp = FALSE;
	loc_u8_temp    = (arg_u8_command & 0xF0)>>4;
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_4_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(loc_u8_temp,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_us(200);
	
	loc_u8_temp    = (arg_u8_command & 0x0F);
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_4_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(loc_u8_temp,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_ms(2);

	
	#elif LCD_MODE == LCD_8_BIT_MODE
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_8_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(arg_u8_command,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_ms(2);
	#else
	#error "LCD MODE ERROR CHECK MACRO IN Lcd_cfg.h file"
	#endif
}
void LCD_vidDisplayCharacter(u8 arg_u8_data)
{
	u8 loc_u8_itrator;
	DIO_enu_WriteChannel(RS_PIN,DIO_PIN_LEVEL_HIGH); // CHOOSE DATA
	//DIO_enu_WriteChannel(RW_PIN,DIO_PIN_LEVEL_LOW); // CHOOSE WRITE
	#if   LCD_MODE == LCD_4_BIT_MODE
	u8 loc_u8_temp = FALSE;
	loc_u8_temp    = (arg_u8_data & 0xF0)>>4;
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_4_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(loc_u8_temp,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_us(200);
	
	loc_u8_temp    = (arg_u8_data & 0x0F);
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_4_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(loc_u8_temp,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_ms(2);

	
	#elif LCD_MODE == LCD_8_BIT_MODE
	
	for(loc_u8_itrator = FALSE ; loc_u8_itrator < LCD_8_BIT_MODE ; loc_u8_itrator++ )
	{
		DIO_enu_WriteChannel(gl_enu_LCD_dataPins[loc_u8_itrator],GET_BIT(arg_u8_data,loc_u8_itrator));
	}
	
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_HIGH); // Enable High
	_delay_ms(1);
	DIO_enu_WriteChannel(E_PIN,DIO_PIN_LEVEL_LOW); // Enable Low
	_delay_ms(2);
	#else
	#error "LCD MODE ERROR CHECK MACRO IN Lcd_cfg.h file"
	#endif
}
void LCD_vidCreateCustomCharacter(u8 *arg_u8_data_array,u8 arg_u8_index)
{
	u8 loc_u8_itrator = FALSE;
	if (arg_u8_index < CGRAM_MAX_INDEX)  // CGRAM only store 8 characters
	{
		LCD_vidSendCommand(LCD_SET_CGRAM_ADDR_CMD + (arg_u8_index*CGRAM_INDEX_BYTES));
		_delay_ms(1);
		for ( ; loc_u8_itrator < CGRAM_INDEX_BYTES ; loc_u8_itrator++)
		{
			LCD_vidDisplayCharacter(arg_u8_data_array[loc_u8_itrator]);
		}
		
		LCD_vidSendCommand(LCD_SET_DDRAM_ADDR_CMD);
	}
}
void LCD_vidGoToXY(u8 arg_u8_row,u8 arg_u8_col)
{
	if(arg_u8_row == 0)
	{
	   LCD_vidSendCommand(LCD_SET_DDRAM_ADDR_CMD + 0 + arg_u8_col);
	}
	else if (arg_u8_row == 1)
	LCD_vidSendCommand(LCD_SET_DDRAM_ADDR_CMD + 0x40 + arg_u8_col);
	else if (arg_u8_row == 2)
	LCD_vidSendCommand(LCD_SET_DDRAM_ADDR_CMD + 0x14 + arg_u8_col);
	else if (arg_u8_row == 3)
	LCD_vidSendCommand(LCD_SET_DDRAM_ADDR_CMD + 0x54 + arg_u8_col);
}

LCD_API_STATE_t LCD_GoToXY(u8 arg_u8_row,u8 arg_u8_col)
{
	LCD_API_STATE_t loc_return = LCD_API_WAITING;
	static u8 loc_u8_nibble = FALSE;
	if (loc_u8_nibble == FALSE)
	{
		if(arg_u8_row == 0)
		LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0 + arg_u8_col)>>4);
		else if (arg_u8_row == 1)
		LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x40 + arg_u8_col)>>4);
		else if (arg_u8_row == 2)
		LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x14 + arg_u8_col)>>4);
		else if (arg_u8_row == 3)
		LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x54 + arg_u8_col)>>4);
		
		loc_u8_nibble = TRUE;
	}
	else
	{
		
			if(arg_u8_row == 0)
			LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0 + arg_u8_col));
			else if (arg_u8_row == 1)
			LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x40 + arg_u8_col));
			else if (arg_u8_row == 2)
			LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x14 + arg_u8_col));
			else if (arg_u8_row == 3)
			LCD_send(LCD_CMD,(LCD_SET_DDRAM_ADDR_CMD + 0x54 + arg_u8_col));
			
			loc_u8_nibble = FALSE;
			loc_return = LCD_API_JOB_DONE;
		
	}
	return loc_return;
}
void LCD_vidDisplayString(u8 *arg_u8_string)
{
	u8 loc_u8_itartor = FALSE;
	while(arg_u8_string[loc_u8_itartor] != STR_NULL)
	{
		LCD_vidDisplayCharacter(arg_u8_string[loc_u8_itartor++]);
	}
}
void LCD_vidDisplayNumber(s32 arg_s32_number)
{
	u32 loc_u8_inverse = TRUE;
	if (arg_s32_number == 0)
	{
		LCD_vidDisplayCharacter('0');
	}
	else
	{
		if (arg_s32_number < 0)
		{
			LCD_vidDisplayCharacter('-');
			arg_s32_number = (arg_s32_number)-(arg_s32_number*2);
		}
		while(arg_s32_number != FALSE)
		{
			loc_u8_inverse*=10;
			loc_u8_inverse+=arg_s32_number%10;
			arg_s32_number/=10;
		}
		
		while(loc_u8_inverse != TRUE)
		{
			LCD_vidDisplayCharacter((loc_u8_inverse%10)+'0');
			loc_u8_inverse/=10;
		}
	}
	
}
void LCD_vidClrDisplay(void)
{
	LCD_vidSendCommand(LCD_CLEAR_DISPLAY);
	_delay_ms(4);
	LCD_vidSendCommand(LCD_CURSOR_HOME);
	_delay_ms(4);

}