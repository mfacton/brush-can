#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "inttypes.h"

#define Control_COUNT 3

enum Control {
	Control1,
	Control2,
	Control3,
};

enum ControlMode {
	ControlManual,
	ControlPid,
};

void Control_Init(void);
void Control_Update(void);

void Control_SetMode(enum Control control, uint8_t val);
uint8_t Control_GetMode(enum Control control);

#endif
