#ifndef TIMER_H
#define TIMER_H
#include "def.h"
#include "cpu.h"
#include "memory.h"
#define CLOCK_SPEED 4194304
#define DIVIDER_TIMER_SPEED CLOCK_SPEED/16384
static const int tac_speed[] = {CLOCK_SPEED / 4096, CLOCK_SPEED/262144, CLOCK_SPEED/65536, CLOCK_SPEED/16384};
static inline void handle_timer(BYTE cycles);
#endif
