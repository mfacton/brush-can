#include "control.h"
#include "inttypes.h"
#include "encoder.h"
#include "pid.h"
#include "stm32g4xx_hal.h"

extern TIM_HandleTypeDef htim16;
static uint8_t mode[CONTROL_COUNT] = {0};

void Control_Init(void) {
//	Motor_Current(0, 40);
//
//	Pid_Reset(0, Encoder_Position(0));
//	Pid_SetP(0, 0.1);
//	Pid_SetMaxP(0, 80);
//	Pid_SetMaxOut(0, 80);
//
//	Pid_Target(0, 10000);

	HAL_TIM_Base_Start(&htim16);
}

void Control_Update(void) {
	const uint16_t tim = __HAL_TIM_GET_COUNTER(&htim16);
	__HAL_TIM_SET_COUNTER(&htim16, 0);
	for (uint8_t ch = 0; ch < CONTROL_COUNT; ch++) {
		if (mode[ch] == ControlPid) {
			Motor_Voltage(ch, (int8_t)Pid_Update(ch, Encoder_Position(ch), (float)tim/1000000.0));
		}
	}
//	Motor_Voltage(0, val);

//	float err = (Encoder_Position(0)-1000);
//	float voltage = 1*err;
//	if (voltage < -80) {
//		voltage = -80;
//	}
//	Motor_Voltage(0, -(int8_t)voltage);

//	for (uint8_t i = 0; i < CONTROL_COUNT; i++) {
//		const int8_t val = (int8_t)Pid_Update(i, Encoder_Position(i), 0.01);
//		Motor_Voltage(i, val);
////		if (mode[i] == ControlPid) {
////
////
////
////		}
//	}
	//	for (uint8_t i = 0; i < 3; i++) {
	//		const int8_t val = (uint8_t)Pid_Update(i, Encoder_Position(i));
	//		Motor_Voltage(i, val);
	//	}
}

void Control_Reset(enum Control control) {
	Encoder_Reset(control);
	Pid_Reset(control, 0);
}

void Control_SetMode(enum Control control, uint8_t val) {
	Pid_Reset(control, Encoder_Position(control));
	mode[control] = val;
}
uint8_t Control_GetMode(enum Control control) {
	return mode[control];
}
