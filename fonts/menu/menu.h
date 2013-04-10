#ifndef MENU_H
#define MENU_H
	#define UP 1
	#define DOWN 0
	SDL_Surface *sdl_screen;
	int cursor_pos;
	int current_window;
	Uint32 bg_color;
	int joypad_config[8];
	typedef struct{
		char text[20];
		int pos_x;
		int pos_y;
	} menu_elem_t;
	typedef enum {menu, config, load_rom};


#endif
