/*
 * Adc_prv.h
 *
 * Created: 10/23/2023 3:51:57 AM
 *  Author: sharb
 */ 


#ifndef ADC_PRV_H_
#define ADC_PRV_H_

#include "std_types.h"

#define ADMUX               (*(vu8*)(0x27)) 
#define ADCSRA              (*(vu8*)(0x26))
#define ADCH                (*(vu8*)(0x25))
#define ADCL                (*(vu8*)(0x24))
#define ADCLH               (*(vu16*)(0x24))
#define SFIOR               (*(vu8*)(0x50))



#define REFS1                 7
#define REFS0                 6
#define ADLAR                 5
#define MUX4                  4
#define MUX3                  3
#define MUX2                  2
#define MUX1                  1
#define MUX0                  0
							  
#define ADEN                  7
#define ADSC                  6
#define ADATA                 5
#define ADIF                  4
#define ADIE                  3
#define ADPS2                 2
#define ADPS1                 1
#define ADPS0                 0
							  
#define AREF_PIN_REF	      0
#define INTERNAL_VCC_5V_REF   1
#define INTERNAL_2_56V_REF    2
							  
							  
#define LEFT_ADJUST           0
#define RIGHT_ADJUST          1
							  
#define DIVISION_FACTOR_2     0
#define DIVISION_FACTOR_4     1
#define DIVISION_FACTOR_8     2
#define DIVISION_FACTOR_16    3
#define DIVISION_FACTOR_32    4
#define DIVISION_FACTOR_64    5
#define DIVISION_FACTOR_128   6
							  
#define AUTO_TRIGGER_ENABLE   0
#define AUTO_TRIGGER_DISABLE  1

#define ADC_INTERRUPT_ENABLE  0
#define ADC_INTERRUPT_DISABLE 1



#define MASK_DATA              0x0FFF
#define MASK_PIN               0x1F




#endif /* ADC_PRV_H_ */