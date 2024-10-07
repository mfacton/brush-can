#include "adc.h"
#include "inttypes.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

static volatile int16_t adc_values[ADC_COUNT];

void Adc_Init(void) {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 2);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc_values+2, 2);
}

int16_t Adc_Value(enum Adc adc) {
	return adc_values[adc];
}
