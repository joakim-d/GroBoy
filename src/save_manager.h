#ifndef SAVE_H
#define SAVE_H
#include "cpu.h"
//#include "memory.h"
//#include "interrupts.h"
//#include "memory.h"
//#include "gpu.h"
//#include "sound.h"
//#include "interrupts.h"
//#include "joypad.h"
//#include "timer.h"
#include <stdio.h>
#include <string.h>
FILE *fichier;
static const char *save_path = "saves/save1"; 
void save_state();
void restore();
#endif

