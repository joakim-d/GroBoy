#ifndef GPU_H
#define GPU_H
#include "def.h"
#include "memory.h"
#include "interrupts.h"
#define LY_VISIBLE_MAX 144
#define LY_MAX 154
#define ONE_LINE_CYCLES 456
int line_counter;//line counter permet de savoir où en est le GPU sur la ligne en cours
BYTE current_mode;
BYTE current_line;
BYTE gpu_counter;
void gpu_init();
void gpu_update(int cycles);
void gpu_update_line();
void gpu_update_stat();
void gpu_drawline();

#endif
