/*
 * Lcd_cfg.h
 *
 * Created: 10/20/2023 5:05:38 AM
 *  Author: sharb
 */ 


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

/*
 * Choosing LCD mode:
 *
 * 		LCD_4_BIT_MODE  for 4 bit mode
 * 		LCD_8_BIT_MODE  for 8 bit mode
 */
#define LCD_MODE		 LCD_4_BIT_MODE


#define RS_PIN           DIO_PORTA_PIN3
//#define RW_PIN           DIO_PORTA_PIN7
#define E_PIN            DIO_PORTA_PIN2


#endif /* LCD_CFG_H_ */