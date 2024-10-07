#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "inttypes.h"

#define ADC_COUNT 4

enum Adc {
	AdcTemp,
	Adc3,
	Adc2,
	Adc1,
};

void Adc_Init(void);

int16_t Adc_Value(enum Adc adc);

#endif
