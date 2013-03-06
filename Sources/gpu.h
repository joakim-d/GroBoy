#ifndef GPU_H
#define GPU_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "def.h"
#include "memory.h"
#include "interrupts.h"
#define LY_VISIBLE_MAX 144
#define LY_MAX 154
#define ONE_LINE_CYCLES 456
#define SPRITES 1
#define BACKGROUND 2
#define WINDOW 3
typedef struct{
	BYTE px[16][8];
	BYTE size;
	BYTE x_flip;
	BYTE y_flip;
	BYTE palette;
}tile_t;

int line_counter;//line counter permet de savoir où en est le GPU sur la ligne en cours
BYTE current_mode;
BYTE current_line;
BYTE gpu_screen[144][160];
SDL_Surface sdl_matrix[144][160];
SDL_Surface *sdl_screen;
void gpu_init();
void gpu_update(int cycles);
void gpu_update_line();
void gpu_update_stat();
void gpu_drawline();
void get_tile(BYTE num, tile_t *tile, int type);
void tile_flip(tile_t *tile, int flipx_y, int size);

#endif
