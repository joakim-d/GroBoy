#ifndef BMP_FONT_H
#define BMP_FONT_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
	SDL_Surface *bitmap_font;
	SDL_Rect chars[256];
	int load_bmp(char *bmp_path);
	void write_text(char *text, int x, int y, SDL_Surface *surface);
#endif
