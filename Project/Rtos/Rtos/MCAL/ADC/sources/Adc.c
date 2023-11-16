/*
 * Adc.c
 *
 * Created: 10/23/2023 3:52:27 AM
 *  Author: sharpel
 */ 


#include "std_types.h"
#include "bit_math.h"
#include "vect_table.h"
#include "../includes/Adc_prv.h"
#include "../includes/Adc_cfg.h"
#include "../includes/Adc.h"


static volatile u16 gl_u16_ConversionResult = FALSE;
void     (*gl_ptr_ADC)(u16) = PTR_NULL;

void ADC_voidInit(void)
{
	
	#if   REFRENCE_SELECTION == AREF_PIN_REF
	
	CLR_BIT(ADMUX,REFS0);
	CLR_BIT(ADMUX,REFS1);
	
	#elif REFRENCE_SELECTION == INTERNAL_VCC_5V_REF
	
	SET_BIT(ADMUX,REFS0);
	CLR_BIT(ADMUX,REFS1);
	
	#elif REFRENCE_SELECTION == INTERNAL_2_56V_REF
	
	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);
	
	#else 
	#error "REFRENCE_SELECTION CONFIG ERROR"
	#endif
	
	#if   ADC_Adjust_Result == LEFT_ADJUST
	
	SET_BIT(ADMUX,ADLAR);
	
	#elif ADC_Adjust_Result == RIGHT_ADJUST
	
	CLR_BIT(ADMUX,ADLAR);
	
	#else
	#error "ADC_Adjust_Result CONFIG ERROR"
	#endif
	
	#if   ADC_PRESCALER_SELECTION == DIVISION_FACTOR_2
	CLR_BIT(ADCSRA,ADPS0);
	CLR_BIT(ADCSRA,ADPS1);
	CLR_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_4
	CLR_BIT(ADCSRA,ADPS0);
	SET_BIT(ADCSRA,ADPS1);
	CLR_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_8
	SET_BIT(ADCSRA,ADPS0);
	SET_BIT(ADCSRA,ADPS1);
	CLR_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_16
	CLR_BIT(ADCSRA,ADPS0);
	CLR_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_32
	SET_BIT(ADCSRA,ADPS0);
	CLR_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_64
	CLR_BIT(ADCSRA,ADPS0);
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
	#elif ADC_PRESCALER_SELECTION == DIVISION_FACTOR_128
	SET_BIT(ADCSRA,ADPS0);
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
	#else
	#error "ADC_PRESCALER_SELECTION CONFIG ERROR"
	#endif
	
	#if    ADC_INTERRUPT_STATUS == ADC_INTERRUPT_ENABLE
	SET_BIT(ADCSRA,ADIE);
	#elif  ADC_INTERRUPT_STATUS == ADC_INTERRUPT_DISABLE
	CLR_BIT(ADCSRA,ADIE);
	#else
	#error "ADC_INTERRUPT_STATUS CONFIG ERROR"
	#endif
	
	SET_BIT(ADCSRA,ADEN);
	
}
ADC_enum_ErrorStatus_t ADC_enum_readValuePolling(ADC_enum_pinIndex_t arg_enum_pinIndex,u16 *ptr_AdcValue)
{
	ADC_enum_ErrorStatus_t loc_enum_returnStatus = ADC_OKAY;
	
	if (arg_enum_pinIndex >= ADC_PIN_MAX)
	{
		loc_enum_returnStatus = ADC_PIN_ERROR;
	}
	else if ( ptr_AdcValue == PTR_NULL)
	{
		loc_enum_returnStatus = ADC_NULLPTR_ERROR;
	}
	else
	{
		
		if ( GET_BIT(ADCSRA,ADIE) == TRUE)
		{
			CLR_BIT(ADCSRA,ADIE);
		}
		else
		{
			// DO NOTHING
		}
		
		// select input pin
		
	   	ADMUX |= (arg_enum_pinIndex & MASK_PIN) ;
		
		// start Conversion
		
		SET_BIT(ADCSRA,ADSC);
		
		// POOL UNTIL FLAG IS RAISED
		while(GET_BIT(ADCSRA,ADSC));
		
		//clear FLAG
		SET_BIT(ADCSRA,ADIF);
		
		*ptr_AdcValue = ADCLH;
	}
	
	return loc_enum_returnStatus;
}

ADC_enum_ErrorStatus_t ADC_enum_setCallBack(void (*ptr_Func)(u16))
{
	ADC_enum_ErrorStatus_t loc_enum_returnStatus = ADC_OKAY;
	if ( ptr_Func == PTR_NULL)
	{
		loc_enum_returnStatus = ADC_NULLPTR_ERROR;
	}
	else
	{
		gl_ptr_ADC  = ptr_Func ;
	}
	
	return loc_enum_returnStatus;
}
ADC_enum_ErrorStatus_t ADC_enum_readValueInterrupt(ADC_enum_pinIndex_t arg_enum_pinIndex)
{
	ADC_enum_ErrorStatus_t loc_enum_returnStatus = ADC_OKAY;
	
	if (arg_enum_pinIndex >= ADC_PIN_MAX)
	{
		loc_enum_returnStatus = ADC_PIN_ERROR;
	}
	
	else
	{
		
		if ( GET_BIT(ADCSRA,ADIE) == FALSE)
		{
			SET_BIT(ADCSRA,ADIE);
		}
		else
		{
			// DO NOTHING
		}
		
		
		// select input pin
		
	   	ADMUX |= (arg_enum_pinIndex & MASK_PIN) ;
		
		// start Conversion
		
		SET_BIT(ADCSRA,ADSC);
	}
	
	return loc_enum_returnStatus;
}
void ADC_voidDisable(void)
{
	ADMUX  = FALSE;
	ADCSRA = FALSE;
}




ISR(ADC_INT)
{
	if(gl_ptr_ADC != PTR_NULL)
	{
		gl_u16_ConversionResult = (ADCLH & MASK_DATA); // MASK
		gl_ptr_ADC(gl_u16_ConversionResult);
	}
}


// TASK 