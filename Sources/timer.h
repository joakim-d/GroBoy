#ifndef TIMER_H
#define TIMER_H
#include "def.h"
#include "cpu.h"
#include "memory.h"
#define CLOCK_SPEED 4194304
#define DIVIDER_TIMER_SPEED CLOCK_SPEED/16384
static const int tac_speed[] = {4096, 262144, 65536, 16384};
void timer_update(BYTE cycles);
#endif
