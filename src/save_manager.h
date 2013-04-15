#ifndef SAVE_H
#define SAVE_H
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"
#include "menu.h"

FILE *fichier;
void save_state();
void restore();
#endif

