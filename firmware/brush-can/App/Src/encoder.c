#include <encoder.h>
#include "main.h"

extern TIM_HandleTypeDef htim17;

static struct EncoderConfig encoders[ENCODER_COUNT] = {{.aPort = ECA1_GPIO_Port, .aPin = ECA1_Pin, .bPort = ECB1_GPIO_Port, .bPin=ECB1_Pin},
		{.aPort = ECA2_GPIO_Port, .aPin = ECA2_Pin, .bPort = ECB2_GPIO_Port, .bPin=ECB2_Pin},
		{.aPort = ECA3_GPIO_Port, .aPin = ECA3_Pin, .bPort = ECB3_GPIO_Port, .bPin=ECB3_Pin}};


static uint8_t aLast[ENCODER_COUNT] = {0};
static uint8_t bLast[ENCODER_COUNT] = {0};
static int32_t position[ENCODER_COUNT] = {0};

void Encoder_Init(void) {
	HAL_TIM_Base_Start_IT(&htim17);
}

void Encoder_Handler(void) {
	for (uint8_t e = 0; e < ENCODER_COUNT; e++) {
		const uint8_t aState = HAL_GPIO_ReadPin(encoders[e].aPort, encoders[e].aPin);
		const uint8_t bState = HAL_GPIO_ReadPin(encoders[e].bPort, encoders[e].bPin);

		const uint8_t aDiff = aState^aLast[e];
		const uint8_t bDiff = bState^bLast[e];
		if (aDiff^bDiff) {
			const uint8_t dir = aDiff^aState;
			position[e] += (dir<<2) - 1;
		}
		aLast[e] = aState;
		bLast[e] = bState;
	}
}

int32_t Encoder_Position(enum Encoder encoder) {
	return position[encoder];
}

void Encoder_Reset(enum Encoder encoder) {
	position[encoder] = 0;
}

void Encoder_ResetAll() {
	for (uint8_t e = 0; e < ENCODER_COUNT; e++) {
		position[e] = 0;
	}
}
