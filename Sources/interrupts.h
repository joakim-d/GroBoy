#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "def.h"
#include "cpu.h"
#include "memory.h"
#define V_BLANK 0
#define LCD_STAT 1
#define TIMER 2
#define SERIAL 3
#define JOYPAD 4
#define CLOCK_SPEED 4194304
#define DIVIDER_TIMER_SPEED CLOCK_SPEED/16384
BYTE IME; // Interrupt Master Enable Flag (Si flag = 0 -> aucune interruption tolérée, si Flag = 1 interruptions tolérées)
static const int tac_speed[] = {CLOCK_SPEED / 4096, CLOCK_SPEED/262144, CLOCK_SPEED/65536, CLOCK_SPEED/16384};
void set_IME();
void reset_IME();
void handle_interrupts(BYTE cycles);
void execute_interrupt(z80_t *z80, BYTE type);
void make_request(BYTE type);
static inline void handle_timer(BYTE cycles);
#endif
