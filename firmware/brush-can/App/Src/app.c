#include "app.h"
#include "main.h"
#include "motor.h"
#include "encoder.h"
#include "can.h"

uint8_t i = 0;

void App_Init(void) {
	Motor_Init();
	Encoder_Init();
	Can_Init();
}

void App_Loop(void) {
	char msg[16] = "Hello World 000";
	msg[12] += i/100;
	msg[13] += (i%100)/10;
	msg[14] += i%10;
	i++;

	Can_Transmit(23, (uint8_t*)msg, CanSize16);
	HAL_Delay(200);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		Can_Handler();
	}
}
