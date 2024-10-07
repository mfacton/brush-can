#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "inttypes.h"

enum CanDataSize {
	CanSize0,
	CanSize1,
	CanSize2,
	CanSize3,
	CanSize4,
	CanSize5,
	CanSize6,
	CanSize7,
	CanSize8,
	CanSize12,
	CanSize16,
	CanSize20,
	CanSize24,
	CanSize32,
	CanSize48,
	CanSize64,
};

void Can_Init(void);

void Can_Transmit(uint16_t id, uint8_t *data, enum CanDataSize size);
void Can_Handler(void);

#endif
