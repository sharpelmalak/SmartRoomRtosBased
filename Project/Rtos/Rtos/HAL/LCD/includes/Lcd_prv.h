/*
 * Lcd_prv.h
 *
 * Created: 10/20/2023 5:05:55 AM
 *  Author: sharb
 */ 


#ifndef LCD_PRV_H_
#define LCD_PRV_H_


#define LCD_CLEAR_DISPLAY					0x01
#define LCD_CURSOR_HOME					    0x02
#define LCD_ENTRY_MODE_CURSOR_INCREMENT	    0x06
#define LCD_DISPLAY_ON_CURSOR_ON_NO_BLINK	0x0C
#define LCD_4_BIT_MODE_SELECTION			0x28
#define LCD_8_BIT_MODE_SELECTION			0x38
#define LCD_DISPLAY_SHIFT_LEFT				0x18
#define LCD_DISPLAY_SHIFT_RIGHT			    0x1C
#define LCD_SET_CGRAM_ADDR_CMD				0x40
#define LCD_SET_DDRAM_ADDR_CMD				0x80



#define LCD_8_BIT_MODE				        8
#define LCD_4_BIT_MODE				        4
#define CGRAM_MAX_INDEX                     8
#define CGRAM_INDEX_BYTES                   8



#endif /* LCD_PRV_H_ */