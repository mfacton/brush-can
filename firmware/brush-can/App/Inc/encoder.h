#ifndef INC_DRIVERS_ENCODERS_H_
#define INC_DRIVERS_ENCODERS_H_

#include <motor.h>
#include "inttypes.h"

#define ENCODER_COUNT 3

struct EncoderConfig {
	GPIO_TypeDef* aPort;
	uint16_t aPin;
	GPIO_TypeDef* bPort;
	uint16_t bPin;
};

enum Encoder {
	Encoder1,
	Encoder2,
	Encoder3,
};

void Encoder_Init(void);

void Encoder_Handler(void);

int32_t Encoder_Position(enum Encoder encoder);

void Encoder_Reset(enum Encoder encoder);
void Encoder_ResetAll(void);

#endif
