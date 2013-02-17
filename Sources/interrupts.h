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
BYTE IME; // Interrupt Master Enable Flag (Si flag = 0 -> aucune interruption tolérée, si Flag = 1 interruptions tolérées)
void set_IME();
void reset_IME();
void handle_interrupts(BYTE cycles);
void execute_interrupt(BYTE type);
void make_request(BYTE type);
#endif
