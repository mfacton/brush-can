#include "command.h"
#include "string.h"
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "can.h"
#include "adc.h"
#include "control.h"

//Auto-generate parse functions
COMMAND_PARSE(uint8_t)
COMMAND_PARSE(int8_t)
COMMAND_PARSE(uint16_t)
//COMMAND_PARSE(int16_t)
//COMMAND_PARSE(uint32_t)
COMMAND_PARSE(int32_t)
COMMAND_PARSE(float)

static uint16_t address_offset = 0;

static void command_address(uint16_t offset) {
	address_offset = offset;
}

void Command_Instruction(uint8_t id, uint8_t *data) {
	if (id < address_offset) {
		return;
	}
	id -= address_offset;
	const uint8_t chan = data[0];
	uint8_t msg[4];
	switch (id) {
		//Auto-generate case statements
		COMMAND_SET(P, Pid_SetP, float)
		COMMAND_SET(I, Pid_SetI, float)
		COMMAND_SET(D, Pid_SetD, float)
		COMMAND_SET(MaxP, Pid_SetMaxP, float)
		COMMAND_SET(MaxI, Pid_SetMaxI, float)
		COMMAND_SET(MaxD, Pid_SetMaxD, float)
		COMMAND_SET(MaxOut, Pid_SetMaxOut, float)
		COMMAND_SET(Voltage, Motor_Voltage, int8_t)
		COMMAND_SET(Current, Motor_Current, uint8_t)
		COMMAND_SET(Target, Pid_Target, int32_t)
		COMMAND_SET(Mode, Control_SetMode, uint8_t)

		COMMAND_GET(Position, Encoder_Position, int32_t)
		COMMAND_GET(Current, Adc_Value, int16_t)

	case CommandSetAddress:
		command_address(command_uint16_t(data, 0));
		break;
	case CommandReset:
		Control_Reset(chan);
		break;
	}
}
