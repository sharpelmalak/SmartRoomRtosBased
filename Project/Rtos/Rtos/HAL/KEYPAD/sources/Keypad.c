/*
 * Keypad.c
 *
 * Created: 10/21/2023 3:24:06 PM
 *  Author: sharpel
 */


#include "std_types.h"
#include "bit_math.h"
#include "../../../MCAL/DIO/includes/Dio.h"
#include "../includes/Keypad.h"
#include "../includes/Keypad_cfg.h"


#define DEFAULT_PATTERN  0x0F

#define COL_0_PATTERN    0x0E
#define COL_1_PATTERN    0x0D
#define COL_2_PATTERN    0x0B
#define COL_3_PATTERN    0x07


static void KEYPAD_writeRows(u8 arg_u8_pattern)
{
	DIO_enu_WriteChannel(KEYPAD_ROW_0_PIN,GET_BIT(arg_u8_pattern,0));
	DIO_enu_WriteChannel(KEYPAD_ROW_1_PIN,GET_BIT(arg_u8_pattern,1));
	DIO_enu_WriteChannel(KEYPAD_ROW_2_PIN,GET_BIT(arg_u8_pattern,2));
	DIO_enu_WriteChannel(KEYPAD_ROW_3_PIN,GET_BIT(arg_u8_pattern,3));
}
static void KEYPAD_readCols(u8 *arg_u8_col)
{
	u8 loc_u8_temp = FALSE;
	*arg_u8_col     = FALSE;
	
	DIO_enu_ReadChannel(KEYPAD_COL_0_PIN,&loc_u8_temp);
	*arg_u8_col |= (loc_u8_temp << 0);
	
	DIO_enu_ReadChannel(KEYPAD_COL_1_PIN,&loc_u8_temp);
	*arg_u8_col |= loc_u8_temp << 1;
	
	DIO_enu_ReadChannel(KEYPAD_COL_2_PIN,&loc_u8_temp);
	*arg_u8_col |= loc_u8_temp << 2;
	
	DIO_enu_ReadChannel(KEYPAD_COL_3_PIN,&loc_u8_temp);
	*arg_u8_col |= loc_u8_temp << 3;
}
KEYPAD_enu_PressedKey_t KEYPAD_GetKey(void)
{
	KEYPAD_enu_PressedKey_t loc_enu_returnKey = KEY_NOTHING;
	u8                      loc_u8_itrator    = FALSE;
	u8                      loc_u8_colSelect  = FALSE;
	
	for(;loc_u8_itrator<ROWS_NUMBER;loc_u8_itrator++)
	{
		KEYPAD_writeRows(TGL_NO_ASSIGN(DEFAULT_PATTERN,loc_u8_itrator));
		KEYPAD_readCols(&loc_u8_colSelect);
		
		switch(loc_u8_colSelect)
		{
			case COL_0_PATTERN : loc_enu_returnKey = (loc_u8_itrator*4);break;
			case COL_1_PATTERN : loc_enu_returnKey = (loc_u8_itrator*4)+1;break;
			case COL_2_PATTERN : loc_enu_returnKey = (loc_u8_itrator*4)+2;break;
			case COL_3_PATTERN : loc_enu_returnKey = (loc_u8_itrator*4)+3;break;
			default:break;
		}
		if(loc_enu_returnKey == KEY_NOTHING)
		{
			//  do Nothing
		}
		else
		{
			// break loop since key is pressed
			break; 
		}
		
		
		
	}
	
	while(loc_u8_colSelect != 0x0F)
	{
		KEYPAD_readCols(&loc_u8_colSelect);
	};
	return loc_enu_returnKey;
}
