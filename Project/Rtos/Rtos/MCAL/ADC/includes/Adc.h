/*
 * Adc.h
 *
 * Created: 10/23/2023 3:51:41 AM
 *  Author: sharb
 */ 


#ifndef ADC_H_
#define ADC_H_

typedef enum
{
	ADC_PORTA_PIN_0,
	ADC_PORTA_PIN_1,
	ADC_PORTA_PIN_2,
	ADC_PORTA_PIN_3,
	ADC_PORTA_PIN_4,
	ADC_PORTA_PIN_5,
	ADC_PORTA_PIN_6,
	ADC_PORTA_PIN_7,
	ADC_PIN_MAX
	
}ADC_enum_pinIndex_t;


typedef enum
{
	ADC_OKAY,
	ADC_PIN_ERROR,
	ADC_NULLPTR_ERROR
	
}ADC_enum_ErrorStatus_t;

void                   ADC_voidInit(void);
ADC_enum_ErrorStatus_t ADC_enum_readValuePolling(ADC_enum_pinIndex_t arg_enum_pinIndex,u16 *ptr_AdcValue);
ADC_enum_ErrorStatus_t ADC_enum_readValueInterrupt(ADC_enum_pinIndex_t arg_enum_pinIndex);
ADC_enum_ErrorStatus_t ADC_enum_setCallBack(void (*ptr_Func)(u16));
void                   ADC_voidDisable(void);



#endif /* ADC_H_ */