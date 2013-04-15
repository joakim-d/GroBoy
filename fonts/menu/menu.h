#ifndef MENU_H
#define MENU_H
	#include <linux/limits.h>
	#define UP 1
	#define DOWN 0
	SDL_Surface *sdl_screen;
	int cursor_pos;
	int cursor_pos_max;
	int current_window;
	Uint32 bg_color;
	int joypad_config[8];
	char selected_game[PATH_MAX];
	char cwd[PATH_MAX];
	typedef struct{
		char text[20];
		int pos_x;
		int pos_y;
	} menu_elem_t;
	typedef enum {menu, config, load_rom} page_e;


#endif
