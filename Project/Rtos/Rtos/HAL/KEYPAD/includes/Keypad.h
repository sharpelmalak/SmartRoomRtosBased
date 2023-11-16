/*
 * Keypad.h
 *
 * Created: 10/21/2023 3:23:35 PM
 *  Author: sharb
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

typedef enum 
{
	KEY_0  = 0  ,
	KEY_1       ,
	KEY_2       ,
	KEY_3       ,
	KEY_4       ,
	KEY_5       ,
	KEY_6       ,
	KEY_7       ,
	KEY_8       ,
	KEY_9       ,
	KEY_10      ,
	KEY_11      ,
	KEY_12      ,
	KEY_13      ,
	KEY_14      ,
	KEY_15      ,
	KEY_NOTHING 
	
}KEYPAD_enu_PressedKey_t;

KEYPAD_enu_PressedKey_t KEYPAD_GetKey(void);

#endif /* KEYPAD_H_ */