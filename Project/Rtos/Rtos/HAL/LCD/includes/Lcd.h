/*
 * Lcd.h
 *
 * Created: 10/20/2023 5:05:19 AM
 *  Author: Sharpel
 */ 


#ifndef LCD_H_
#define LCD_H_


/*
 * function		: LCD_vidInit
 * description 	: func to set LCD initialization
 * input param 	: void
 * return		: void
 * */


typedef enum
{
	LCD_CMD,
	LCD_DATA
	
}LCD_REQUEST_t;
typedef enum
{
	LCD_API_JOB_DONE,
	LCD_API_WAITING
   
}LCD_API_STATE_t;
LCD_API_STATE_t  LCD_enu_InitWithOs(void);
LCD_API_STATE_t  LCD_send(LCD_REQUEST_t request,u8 data);
LCD_API_STATE_t  LCD_CreateCustomCharacter(u8 *arg_u8_data_array,u8 arg_u8_index);
LCD_API_STATE_t  LCD_writeString(u8  *arg_u8_string);
LCD_API_STATE_t  LCD_writeNumber(s32 arg_s32_number);
LCD_API_STATE_t  LCD_GoToXY(u8 arg_u8_row,u8 arg_u8_col);





void             LCD_vidInit(void);
void             LCD_vidSendCommand(u8 arg_u8_command);
void             LCD_vidDisplayCharacter(u8 arg_u8_data);
void             LCD_vidCreateCustomCharacter(u8 *arg_u8_data_array,u8 arg_u8_index);
void             LCD_vidGoToXY(u8 arg_u8_row,u8 arg_u8_col);

void             LCD_vidDisplayString(u8 *arg_u8_string);
void             LCD_vidDisplayNumber(s32 arg_s32_number);
void             LCD_vidClrDisplay(void);

#endif /* LCD_H_ */