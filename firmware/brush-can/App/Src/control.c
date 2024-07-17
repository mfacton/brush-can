#include "control.h"
#include "inttypes.h"
#include "encoder.h"
#include "pid.h"

static uint8_t mode[Control_COUNT] = {0};

void Control_Init(void) {

}
void Control_Update(void) {
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
