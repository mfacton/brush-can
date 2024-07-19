#include "pid.h"
#include "main.h"

//PID constants
static float kp[PID_COUNT] = {0};
static float ki[PID_COUNT] = {0};
static float kd[PID_COUNT] = {0};

//PID max components
static float mp[PID_COUNT] = {0};
static float mi[PID_COUNT] = {0};
static float md[PID_COUNT] = {0};

//PID max output
static float mo[PID_COUNT] = {0};

//PID tracking variables
static int32_t err_prev[PID_COUNT] = {0};
static float err_int[PID_COUNT] = {0};
static int32_t target[PID_COUNT] = {0};

static float pid_reg(float a, float b) {
	if (a < -b) {
		return -b;
	}
	if (a > b) {
		return b;
	}
	return a;
}

float Pid_Update(enum Pid pid, int32_t pos, float delta) {
	const int32_t err = target[pid] - pos;

	const float p = pid_reg(kp[pid]*(float)err, mp[pid]);

	err_int[pid] += (float)err*delta;
	const float i = pid_reg(ki[pid]*(float)err_int[pid], mi[pid]);
	if (ki[pid] != 0) {
		err_int[pid] = i/ki[pid];
	}

	const float d = pid_reg(kd[pid]*(float)(err - err_prev[pid])/(float)delta, md[pid]);
	err_prev[pid] = err;

	return pid_reg(p+i+d, mo[pid]);
}

void Pid_Reset(enum Pid pid, int32_t pos) {
	err_prev[pid] = 0;
	err_int[pid] = 0;
	target[pid] = pos;
}

void Pid_Target(enum Pid pid, int32_t pos) {
	target[pid] = pos;
	err_int[pid] = 0;
}

void Pid_SetP(enum Pid pid, float val) {
	kp[pid] = val;
}
void Pid_SetI(enum Pid pid, float val) {
	ki[pid] = val;
}
void Pid_SetD(enum Pid pid, float val) {
	kd[pid] = val;
}

void Pid_SetMaxP(enum Pid pid, float val) {
	mp[pid] = val;
}
void Pid_SetMaxI(enum Pid pid, float val) {
	mi[pid] = val;
}
void Pid_SetMaxD(enum Pid pid, float val) {
	md[pid] = val;
}

void Pid_SetMaxOut(enum Pid pid, float val) {
	mo[pid] = val;
}
