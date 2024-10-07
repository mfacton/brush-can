#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

struct Encoder_Handle {
	// configuration
	TIM_HandleTypeDef* htim; // 20khz

	GPIO_TypeDef** aPorts;
	uint16_t* aPins;
	GPIO_TypeDef** bPorts;
	uint16_t* bPins;

	uint8_t count;

	// internal
	uint8_t* aLasts;
	uint8_t* bLasts;

	int32_t* positions;
};

void Encoder_Init(struct Encoder_Handle* handle);

void Encoder_TimHandler(struct Encoder_Handle* handle);
uint8_t Encoder_TimFlag(struct Encoder_Handle* handle, TIM_HandleTypeDef *htim);

#endif
