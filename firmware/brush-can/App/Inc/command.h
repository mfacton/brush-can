#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include "inttypes.h"

#define COMMAND_PARSE(type)							\
static type command_##type(uint8_t* d, uint8_t i) {	\
    type v;											\
    memcpy(&v, d + i, sizeof(type));				\
    return v;										\
}

#define COMMAND_SET(id, func, type)         \
case CommandSet##id:						\
		func(chan, command_##type(data, 1));\
		break;								\

#define COMMAND_CAN_SIZE(size) (size==1?CanSize1:size==2?CanSize2:CanSize4)
#define COMMAND_GET(id, func, type)										\
case CommandGet##id: {													\
	const type val = func(chan);										\
	memcpy(msg, &val, sizeof(type));									\
	Can_Transmit(CommandSend##id, msg, COMMAND_CAN_SIZE(sizeof(type)));	\
	break;																\
}

enum Command {
	//set commands
	CommandSetP, //float
	CommandSetI, //float
	CommandSetD, //float
	CommandSetMaxP, //float
	CommandSetMaxI, //float
	CommandSetMaxD, //float
	CommandSetMaxOut, //uint8_t
	CommandSetVoltage, //int8_t
	CommandSetCurrent, //uint8_t
	CommandSetTarget, //int32
	CommandSetMode, //uint8_t
	CommandSetAddress,//uint16_t
	CommandReset, //none

	//request commands
	CommandGetPosition,
	CommandGetCurrent,
	CommandGetMode,

	//responses
	CommandSendPosition, //int32_t
	CommandSendCurrent, //uint16_t
	CommandSendMode, //uint8_t
};

void Command_Instruction(uint8_t id, uint8_t *data);

#endif
