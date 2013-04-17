#ifndef SAVE_H
#define SAVE_H
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"
#include "menu.h"
#include "gpu.h"
#include "joypad.h"
#include "interrupts.h"

FILE *file;
void save_state(int num);
void restore(int num);
void save();
#endif

