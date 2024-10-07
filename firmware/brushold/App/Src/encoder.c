#include <encoder.h>
#include "main.h"

void Encoder_Init(struct Encoder_Handle* handle) {
	handle->aLasts = malloc(handle->count);
	handle->bLasts = malloc(handle->count);
	memset(handle->aLasts, 0, handle->count);
	memset(handle->bLasts, 0, handle->count);

	handle->positions = malloc(4*handle->count);
	memset(handle->positions, 0, 4*handle->count);

	HAL_TIM_Base_Start_IT(handle->htim);
}

void Encoder_TimHandler(struct Encoder_Handle* handle) {
	for (uint8_t e = 0; e < handle->count; e++) {
		const uint8_t aState = HAL_GPIO_ReadPin(handle->aPorts[e], handle->aPins[e]);
		const uint8_t bState = HAL_GPIO_ReadPin(handle->bPorts[e], handle->bPins[e]);

		const uint8_t aDiff = aState^handle->aLasts[e];
		const uint8_t bDiff = bState^handle->bLasts[e];
		if (aDiff^bDiff) {
			const uint8_t dir = aDiff ^ aState ^ bState;
			handle->positions[e] += (dir<<1);
			handle->positions[e] -= 1;
		}
		handle->aLasts[e] = aState;
		handle->bLasts[e] = bState;
	}
}
uint8_t Encoder_TimFlag(struct Encoder_Handle* handle, TIM_HandleTypeDef *htim) {
	return handle->htim == htim;
}
