
/*
 * Rtos.c
 *
 * Created: 11/12/2023 1:37:38 AM
 * Author : Sharpel
 */ 

#include "std_types.h"
#include "../MCAL/PORT/includes/PORT.h"
#include "../MCAL/ADC/includes/Adc.h"
#include "../HAL/LCD/includes/Lcd.h"
#include "../HAL/KEYPAD/includes/Keypad.h"
#include "../MCAL/DIO/includes/Dio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"

/************************************************************************/
/*                           MACROS                                     */
/************************************************************************/


#define INITIAL_DESIRED_TEMP                          27
#define KP_POSSIBLE_KEYS                              16
#define KP_MAX_KEYS                                   3
#define KP_MAX_INDEX                                  2
#define KP_TENTH_POS                                  0
#define KP_TENTH_INIT                                 2
#define KP_UNIT_POS                                   1
#define KP_UNIT_INIT                                  7
#define CELSIUIS_INDEX                                5
#define SEMAPHOR_WAITING_TICKS                        50
#define LCD_STATE_STRINGS                             0
#define LCD_STATE_TEMP_READING                        1
#define LCD_STATE_DESIRED_CHECK                       2

/************************************************************************/
/*                           PROTOTYPES                                 */
/************************************************************************/

void vAppInitCode      (void * pvParameters); // TASK FOR MODULES INITILIZATION 
void vLcdControllerCode(void * pvParameters); // TASK TO CONTROL LCD
void vAppControlCode   (void * pvParameters); // TASK TO CONTROL ADC READING AND FAN STATUS
void vKPCode           (void * pvParameters); // TASK TO CONTROL KEYPAD TO SET DESIRED TEMP
void vADCCallBack      (u16 read);            // FUNCTION TO EXECUTE AFTER ADC CONVERSION DONE



/************************************************************************/
/*                           GLOBALS                                    */
/************************************************************************/
u16  gl_Adc_Reading            = FALSE;                    // STORE TEMP READING
u8   gl_Adc_DONE               = FALSE;                    // FLAG TO INDICATE CONV IS DONE
u8   gl_KP_DONE                = FALSE;                    // FLAG TO INDICATE DESIRED TEMP IS SET
u8   gl_kpbuff[KP_MAX_KEYS]    = "";                       // BUFF TO STORE DESIRED TEMP FROM 0 TO 99 ONLY
u8   gl_Desired_InitValue      = INITIAL_DESIRED_TEMP;     // INITIAL VALUE FOR DESIRED TEMP
u8   gl_customCelsius[]        = {0b00011,0b00011,0b00000,0b01111,0b01000,0b01000,0b01111,0b00000};// CUSTOM CHARACTER FOR CELSIUS (TEMP UNIT)
u8   glkeys_[KP_POSSIBLE_KEYS] = {7,8,9,'c',4,5,6,'c',1,2,3,'c','c',0,'e','c'}; // KP KEYS ONLY ACCEPT NUMBERS AND ANY OTHER KEY IS CLEAR



////////// TASKS HANDLERS
xTaskHandle        xAppInitHandle       = PTR_NULL;
xTaskHandle        xLcdControllerHandle = PTR_NULL;
xTaskHandle        xAppControlHandle    = PTR_NULL;
xTaskHandle        xKPHandle            = PTR_NULL;


//////// 
xQueueHandle       xQueue1              = PTR_NULL;     
xSemaphoreHandle   xSemaphore           = PTR_NULL;



/* CONTAIN ALL INITILZATION FOR MODULES */
void vAppInitCode( void * pvParameters )
{
	
    PORT_voidInit(); // INITILIZE PORT (ALL PINS DIRECTION AND MODE)
    ADC_voidInit();  // INITILIZE ADC
	ADC_enum_setCallBack(vADCCallBack); // SET CALL BACK
	gl_kpbuff[KP_TENTH_POS] = KP_TENTH_INIT; // INIT VALUE FOR DESIRED TEMP
	gl_kpbuff[KP_UNIT_POS]  = KP_UNIT_INIT;
	
	
	LCD_API_STATE_t loc_enu_LCD_state  = LCD_API_WAITING;  // LOC FOR LCD INIT STATE
	LCD_API_STATE_t loc_enu_LCD_create = LCD_API_WAITING;  // LOC FOR LCD CREATE CUSTOM CHAR STATE
	LCD_API_STATE_t loc_enu_LCD_send   = LCD_API_WAITING;  // LOC FOR LCD SENDING STATE
    for( ;; )
    {
		// CHECK IF LCD INIT IS DONE
		if (loc_enu_LCD_state  != LCD_API_JOB_DONE)
		{
			// CALL IT UNTIL IT RETURNS JOB DONE
			loc_enu_LCD_state = LCD_enu_InitWithOs();
		}
		else // CASE LCD INITILIZED 
		{
			
			// CHECK IF LCD CREATE CUSTOM IS DONE
			if (loc_enu_LCD_create != LCD_API_JOB_DONE)
			{
				// CALL IT UNTIL IT RETURNS JOB DONE
				loc_enu_LCD_create = LCD_CreateCustomCharacter(gl_customCelsius,CELSIUIS_INDEX);
			}
			else // CASE CREATED 
			{
				// DELETE THIS TASK
			    vTaskDelete(xAppInitHandle);
				
			}
			
		}
		
		vTaskDelay(5); // 5 MILLI FOR LCD DELAYS
    }
}

/* CONTAIN KEYPAD WORKING MECHANISM */
void vKPCode( void * pvParameters )
{
	
	KEYPAD_enu_PressedKey_t key             = KEY_NOTHING;  // INITILALLY KEY IS NOTHING
	u8                      loc_buffcounter = FALSE;        // BUFF TRACKER OR COUNTER
	
	vTaskDelay(200); // FIRST DELAY TO MAKE SURE MODULES IS INITIALIZED
	for( ;; )
	{
		
		key = KEYPAD_GetKey(); // CHECK KEY
		if(key != KEY_NOTHING) // IF SOMTHING PRESSED
		{
			// CASE NON NUMBERS KEY
			if (glkeys_[key] == 'c')
			{
				// ASK FOR SEMAPHOR
				xSemaphoreTake(xSemaphore,SEMAPHOR_WAITING_TICKS);
				gl_kpbuff[KP_TENTH_POS] = FALSE;//CLEAR
				gl_kpbuff[KP_UNIT_POS]  = FALSE;//CLEAR
				xSemaphoreGive(xSemaphore);
				loc_buffcounter         = FALSE ; // CLEAR LOCAL BUFF TRACKER
			}
			// CASE EQUALL KEY
			else if (glkeys_[key]=='e')
			{
				// DO NOTHING
			}
			else // CASE NUMBER
			{
				// ASK FOR SEMAPHOR
				xSemaphoreTake(xSemaphore,SEMAPHOR_WAITING_TICKS);
				gl_kpbuff[loc_buffcounter++] = glkeys_[key]; // STORE PRESSED KEY IN KP BUFF IN INDEX BUFF TRACKER
				gl_Desired_InitValue         = gl_kpbuff[KP_TENTH_POS]*10+gl_kpbuff[KP_UNIT_POS]; // STORE DESIRED TEMP FROM KEYS
				xSemaphoreGive(xSemaphore);
				if (loc_buffcounter == KP_MAX_INDEX) // IF TWO NUMBERS HAD ENTERD CLEAR INDEX 
				{
					loc_buffcounter = FALSE ;
				}
			}
		}
		
		vTaskDelay(150);
	}
}


/* CONTAIN LCD WORKING MECHANISM */
void vLcdControllerCode( void * pvParameters )
{
	
	
	u8 LCD_state = LCD_STATE_STRINGS; // INIT STATE IS WRITING STATIC STRINGS ON LCD 4
	u8 TEMP_BUFF = FALSE;             // STORE TEMP VALUE
	u8 TEMP_LOCK = FALSE;             // LOCK TO NOT OVERWRITE READ
	LCD_API_STATE_t loc_enu_LCD_state  = LCD_API_WAITING;// LOCALS TO STORE STATE OF CALLED FUNCTION
	LCD_API_STATE_t loc_enu_LCD_num    = LCD_API_WAITING;// LOCALS TO STORE STATE OF WRITE NUM FUNCTION
	LCD_API_STATE_t loc_enu_LCD_XY     = LCD_API_WAITING;// LOCALS TO STORE STATE OF GOTOXY FUNCTION
	LCD_API_STATE_t loc_enu_LCD_KP     = LCD_API_WAITING;// LOCALS TO STORE STATE OF PRESSED KP
	
	vTaskDelay(200); // FIRST DELAY TO MAKE SURE MODULES IS INITIALIZED
	for(;;)
	{
	    
		if (LCD_state == LCD_STATE_STRINGS) // FIRST TIME ONLY
		{
			// CHECK ON WRITING STRING ON LCD
			if (loc_enu_LCD_state != LCD_API_JOB_DONE)
			{
				// RETURN JOB DONE IF STRING IS WROTE SUCCESSFULLY
				loc_enu_LCD_state = LCD_writeString("TEMP AT ROOM : ");
			}
			else
			{
				// CHECK ON MOVING CURSOR ON LCD
				if (loc_enu_LCD_XY != LCD_API_JOB_DONE)
				{
					// RETURN JOB DONE IF CURSOR MOVED SUCCESSFULLY
					loc_enu_LCD_XY = LCD_GoToXY(2,0);
				}
				else
				{
					// CHECK ON WRITING STRING ON LCD FOR KP ENTERING
				    if (loc_enu_LCD_KP != LCD_API_JOB_DONE)
				    {
						// RETURN JOB DONE IF STRING IS WROTE SUCCESSFULLY
					    loc_enu_LCD_KP = LCD_writeString("DESIRED TEMP : ");
				    }
				    else // CASE EVERY THING IS DONE 
				    {
						loc_enu_LCD_num   = LCD_API_WAITING;// CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_XY    = LCD_API_WAITING;// CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_state = LCD_API_WAITING;// CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_KP    = LCD_API_WAITING;// CLEAR VAR TO USE IT IN NEXT STATE
					    LCD_state         = LCD_STATE_TEMP_READING; // MOVE TO NEXT STATE
				    }
				}
				
			}
		}
		else if (LCD_state == LCD_STATE_TEMP_READING) // LET'S CHECK ON TEMP READING TO DISPLAY
		{
			
			// CHECK ON MOVING CURSOR ON LCD
			if (loc_enu_LCD_XY != LCD_API_JOB_DONE)
			{
				// RETURN JOB DONE IF CURSOR MOVED SUCCESSFULLY
				loc_enu_LCD_XY = LCD_GoToXY(0,14);
			}
			else
			{
				// CHECK IF TEMP HAS ALREADY READED
				if(TEMP_LOCK == FALSE)
				{
					// RECIEVE READING
					if( xQueueReceive( xQueue1,&( TEMP_BUFF),10 ) == pdPASS )
					{
						// LOCK RECIVING UNTIL VALUE IS DISPLAYED
						TEMP_LOCK = TRUE;
					}
				}
				
                // CHECK ON WRITING TEMP VALUE ON LCD
				if (loc_enu_LCD_num != LCD_API_JOB_DONE)
				{
					// RETURN JOB DONE IF NUMBER WROTE SUCCESSFULLY
					loc_enu_LCD_num = LCD_writeNumber(TEMP_BUFF);
				}
				else
				{
					// CHECK ON WRITING CUSTOM CHAR CELSIUS ON LCD
					if (loc_enu_LCD_state != LCD_API_JOB_DONE)
					{
						// RETURN JOB DONE IF CUSTOM CHAR CELSIUS WROTE SUCCESSFULLY
					   loc_enu_LCD_state = LCD_send(LCD_DATA,CELSIUIS_INDEX>>4); // SEND HIGHER NIBBLE

					}
					else
					{
						LCD_send(LCD_DATA,CELSIUIS_INDEX);   // SEND LOWER NIBBLE
						loc_enu_LCD_num   = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_XY    = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_state = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
						loc_enu_LCD_KP    = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
						TEMP_LOCK         = FALSE;           // CLEAR LOCK TO RECIEVE NEXT CYCLE
						LCD_state         = LCD_STATE_DESIRED_CHECK; // MOVE TO NEXT STATE
					}

				}
			}
			
		} 
		else if (LCD_state == LCD_STATE_DESIRED_CHECK) // LET'S CHECK ON DESIRED TEMP ENTERD BY KP TO DISPLAY
		{
			
			// CHECK ON MOVING CURSOR ON LCD
			if (loc_enu_LCD_XY != LCD_API_JOB_DONE)
			{
				// RETURN JOB DONE IF CURSOR MOVED SUCCESSFULLY
				loc_enu_LCD_XY = LCD_GoToXY(2,14);
			}
			else
			{
				
				if (loc_enu_LCD_num != LCD_API_JOB_DONE)
				{
					// ASK FOR SEMAPHOR
					xSemaphoreTake(xSemaphore,SEMAPHOR_WAITING_TICKS);
					loc_enu_LCD_num = LCD_writeNumber(gl_kpbuff[KP_TENTH_POS]); // DISPLAY KP ENTERD AT TENTH POS
					xSemaphoreGive(xSemaphore);
				}
				else
				{
					if (loc_enu_LCD_state != LCD_API_JOB_DONE)
					{
						// ASK FOR SEMAPHOR
						xSemaphoreTake(xSemaphore,SEMAPHOR_WAITING_TICKS);
						loc_enu_LCD_state = LCD_writeNumber(gl_kpbuff[KP_UNIT_POS]); // DISPLAY KP ENTERD AT UNIT POS
						xSemaphoreGive(xSemaphore);
					}
					else
					{
						// CHECK ON WRITING CUSTOM CHAR CELSIUS ON LCD
						if (loc_enu_LCD_KP != LCD_API_JOB_DONE)
						{
							loc_enu_LCD_KP = LCD_send(LCD_DATA,CELSIUIS_INDEX>>4); // SEND HIGHER NIBBLE
						}
						else
						{
							LCD_send(LCD_DATA,CELSIUIS_INDEX); // SEND LOW NIBBLE
							loc_enu_LCD_num   = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
							loc_enu_LCD_XY    = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
							loc_enu_LCD_state = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
							loc_enu_LCD_KP    = LCD_API_WAITING; // CLEAR VAR TO USE IT IN NEXT STATE
							LCD_state = LCD_STATE_TEMP_READING; // MOVE TO NEXT STATE
						}
						
					}

				}
			}
			
		}
		
		vTaskDelay(5);
	}
}

/* CONTAIN ADC AND MOTOR WORKING MECHANISM */
void vAppControlCode( void * pvParameters )
{
	
	
	vTaskDelay(200); // FIRST DELAY TO MAKE SURE ALL MODULE IS INITIALIZED
	for(;;)
	{
		
		// CHECK IF READING IS DONE
		if (gl_Adc_DONE == FALSE)
		{
			gl_Adc_DONE = TRUE ; 
			ADC_enum_readValueInterrupt(ADC_PORTA_PIN_0); // ASK FOR NEW READING
		}
		
		xSemaphoreTake(xSemaphore,50);
		if(gl_Desired_InitValue < gl_Adc_Reading && gl_KP_DONE == FALSE) // CASE DESIRED LESS THAN READING 
		{
		   DIO_enu_WriteChannel(DIO_PORTB_PIN6,DIO_PIN_LEVEL_HIGH); // ENABLE FAN
		   gl_KP_DONE = TRUE;
		}
		else if (gl_Desired_InitValue >= gl_Adc_Reading) // CASE DESIRED IS MORE THAN OR EQUALL READING
		{
		 
		    if (gl_KP_DONE == TRUE)
		    {
		  	  DIO_enu_WriteChannel(DIO_PORTB_PIN6,DIO_PIN_LEVEL_LOW); //  DISABLE FAN
		  	  gl_KP_DONE = FALSE;
		    }
		 
		}
		xSemaphoreGive(xSemaphore);
		vTaskDelay(100);
		
	}
}
int main(void)
{

	    xQueue1 = xQueueCreate( 10, sizeof( unsigned char ) );
		vSemaphoreCreateBinary(xSemaphore);
	    /* Create the task, storing the handle. */
	    xTaskCreate(
	    vAppInitCode,       /* Function that implements the task. */
	    "APP_INIT",          /* Text name for the task. */
	    85,      /* Stack size in words, not bytes. */
	    PTR_NULL,    /* Parameter passed into the task. */
	    4,/* Priority at which the task is created. */
	    &xAppInitHandle );      /* Used to pass out the created task's handle. */
		
		 /* Create the task, storing the handle. */
		 xTaskCreate(
		 vLcdControllerCode,       /* Function that implements the task. */
		 "LCD_CONTROL",          /* Text name for the task. */
		 128,      /* Stack size in words, not bytes. */
		 PTR_NULL,    /* Parameter passed into the task. */
		 1,/* Priority at which the task is created. */
		 &xLcdControllerHandle );      /* Used to pass out the created task's handle. */
		 
		  /* Create the task, storing the handle. */
		  xTaskCreate(
		  vAppControlCode,       /* Function that implements the task. */
		  "ADC_CONTROL",          /* Text name for the task. */
		  128,      /* Stack size in words, not bytes. */
		  PTR_NULL,    /* Parameter passed into the task. */
		  2,/* Priority at which the task is created. */
		  &xAppControlHandle );      /* Used to pass out the created task's handle. */
		  
		    /* Create the task, storing the handle. */
		    xTaskCreate(
		    vKPCode,       /* Function that implements the task. */
		    "KP_CONTROL",          /* Text name for the task. */
		    128,      /* Stack size in words, not bytes. */
		    PTR_NULL,    /* Parameter passed into the task. */
		    2,/* Priority at which the task is created. */
		    &xKPHandle );      /* Used to pass out the created task's handle. */
		vTaskStartScheduler();
        while (1) ;
		   
}




void vADCCallBack(u16 read)
{
	gl_Adc_Reading = (read*5.0)/1024*100;// CONVERT READING TO TEMP
	xQueueSend( xQueue1,( void * ) &gl_Adc_Reading,0); // SEND CONVERTED VALUE IN QUEUE
	gl_Adc_DONE    = FALSE; // FLAG INDICATE ADC IS DONE
}