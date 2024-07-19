#ifndef INC_PID_H_
#define INC_PID_H_

#include "inttypes.h"

#define PID_COUNT 3

enum Pid {
	Pid1,
	Pid2,
	Pid3,
};

float Pid_Update(enum Pid pid, int32_t pos, float dt);

void Pid_Reset(enum Pid pid, int32_t pos);

void Pid_Target(enum Pid pid, int32_t target);

void Pid_SetP(enum Pid pid, float val);
void Pid_SetI(enum Pid pid, float val);
void Pid_SetD(enum Pid pid, float val);

void Pid_SetMaxP(enum Pid pid, float val);
void Pid_SetMaxI(enum Pid pid, float val);
void Pid_SetMaxD(enum Pid pid, float val);

void Pid_SetMaxOut(enum Pid pid, float val);

#endif
