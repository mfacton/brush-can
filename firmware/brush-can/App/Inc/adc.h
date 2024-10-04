#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "inttypes.h"

#define ADC_COUNT 4

enum Adc {
	Adc1,
	Adc2,
	Adc3,
	AdcTemp,
};

void Adc_Init(void);

int16_t Adc_Value(enum Adc adc);

#endif
