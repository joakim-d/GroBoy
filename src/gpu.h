#ifndef GPU_H
#define GPU_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "def.h"
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"
//#include "save_manager.h"
#define LY_VISIBLE_MAX 144
#define LY_MAX 154
#define ONE_LINE_CYCLES 456
#define SPRITES 1
#define BACKGROUND 2
#define WINDOW 3
#define SDL_VIDEO_FLAGS (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)
typedef struct{
	BYTE px[16][8];
	BYTE size;
	BYTE x_flip;
	BYTE y_flip;
	BYTE palette;
}tile_t;

typedef struct{
	BYTE x;
	BYTE y;
	BYTE pattern_nb;
	BYTE attributes;
}sprite_t;

int screen_mode; //0 pour fenetré , 1 pour plein ecran
BYTE current_line;
BYTE gpu_screen[144][160];
SDL_Surface* sdl_matrix[144][160];
SDL_Surface *sdl_screen;
SDL_Surface *sdl_screenTemp; 
int timer1;
int timer2;
int cycle_length;
unsigned int vblank_clock_counter;
unsigned int line_clock_counter;
BYTE frame_skip;
BYTE frame_counter;

//SDL_Rect **resolutions; //test pour savoir les resolutions possibles
void gpu_init(SDL_Surface *sdl_scr);
void gpu_update(int cycles);
int save_gpu(FILE* file);
void restore_gpu(FILE *file);
#endif
