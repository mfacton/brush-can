#ifndef INC_DRIVERS_MOTORS_H_
#define INC_DRIVERS_MOTORS_H_

#include "stm32g4xx_hal.h"
#include "inttypes.h"

#define MOTOR_COUNT 3

struct MotorConfig {
	TIM_HandleTypeDef* aTimer;
	uint16_t aChannel;
	TIM_HandleTypeDef* bTimer;
	uint16_t bChannel;
	TIM_HandleTypeDef* currentTimer;
	uint16_t currentChannel;
};

enum Motor {
	Motor1,
	Motor2,
	Motor3,
};

void Motor_Init(void);

//0-100
void Motor_Current(enum Motor motor, uint8_t current);

void Motor_Voltage(enum Motor motor, int8_t voltage);

#endif
