#include "adc.h"
#include "inttypes.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern TIM_HandleTypeDef htim7;

static volatile uint16_t adc_buf[ADC_PERIPHERAL_COUNT][ADC_MAX_CHANNELS] = {0};

static volatile uint32_t adc_raw[ADC_COUNT] = {0};
static volatile uint16_t adc_values[ADC_COUNT] = {0};
static volatile uint32_t adc_count = 0;

static inline void adc_start(void) {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf[0], 3);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc_buf[1], 1);
}

void Adc_Init(void) {
	adc_start();

	HAL_TIM_Base_Start_IT(&htim7);
}

void Adc_Handler(void) {
	for (uint8_t i = 0; i < ADC_COUNT; i++) {
		const uint32_t val = adc_raw[i]/adc_count;
		adc_values[i] = val;
		adc_raw[i] = 0;
	}
	adc_count = 0;
}

int16_t Adc_Value(enum Adc adc) {
	return adc_values[adc];
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	adc_start();
	adc_raw[0] += adc_buf[0][0];
	adc_raw[1] += adc_buf[0][1];
	adc_raw[2] += adc_buf[1][0];
	adc_raw[3] += adc_buf[0][2];
	adc_count++;
}
