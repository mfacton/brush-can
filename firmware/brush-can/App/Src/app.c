#include "app.h"
#include "main.h"
#include "motor.h"
#include "encoder.h"
#include "can.h"
#include "pid.h"
#include "adc.h"
#include "control.h"

extern TIM_HandleTypeDef htim17;

void App_Init(void) {
	Motor_Init();
	Adc_Init();
	Encoder_Init();
	Can_Init();
	Control_Init();

	HAL_Delay(1);
}

void App_Loop(void) {
	Control_Update();

	HAL_Delay(2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim17) {
		Encoder_Handler();
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		Can_Handler();
	}
}
