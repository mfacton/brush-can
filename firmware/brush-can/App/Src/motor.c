#include <motor.h>
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim15;

static struct MotorConfig motors[MOTOR_COUNT] = {{.aTimer = &htim3, .aChannel = TIM_CHANNEL_2, .bTimer = &htim15, .bChannel = TIM_CHANNEL_1, .currentTimer = &htim3, .currentChannel = TIM_CHANNEL_4},
		{.aTimer = &htim15, .aChannel = TIM_CHANNEL_2, .bTimer = &htim8, .bChannel = TIM_CHANNEL_1, .currentTimer = &htim1, .currentChannel = TIM_CHANNEL_1},
		{.aTimer = &htim1, .aChannel = TIM_CHANNEL_3, .bTimer = &htim1, .bChannel = TIM_CHANNEL_2, .currentTimer = &htim2, .currentChannel = TIM_CHANNEL_1}};

void Motor_Init(void) {
	for (int m = 0; m < MOTOR_COUNT; m++) {
		HAL_TIM_PWM_Start(motors[m].aTimer, motors[m].aChannel);
		HAL_TIM_PWM_Start(motors[m].bTimer, motors[m].bChannel);
		HAL_TIM_PWM_Start(motors[m].currentTimer, motors[m].currentChannel);
	}
}

void Motor_Current(enum Motor motor, uint8_t current) {
	__HAL_TIM_SET_COMPARE(motors[motor].currentTimer, motors[motor].currentChannel, current);
}

void Motor_Duty(enum Motor motor, uint8_t aDuty, uint8_t bDuty) {
	__HAL_TIM_SET_COMPARE(motors[motor].aTimer, motors[motor].aChannel, aDuty);
	__HAL_TIM_SET_COMPARE(motors[motor].bTimer, motors[motor].bChannel, bDuty);
}
