#ifndef TIMER_H
#define TIMER_H
#include "def.h"
#include "cpu.h"
#include "memory.h"
#define CLOCK_SPEED 4194304
#define DIVIDER_TIMER_SPEED 256
static const int tac_speed[] = {1024, 16, 64, 256};
void timer_update(BYTE cycles);
unsigned int div_timer;
unsigned int tac_timer;
void restore_timer(FILE *file);
void save_timer(FILE *file);
void timer_init();

#endif
