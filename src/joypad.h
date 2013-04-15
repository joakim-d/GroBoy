#ifndef JOYPAD_H
#define JOYPAD_H
	#include "save_manager.h"
	#include "memory.h"
	#include "interrupts.h"
	void joypad_update(int cycles);
	void joypad_init();
	SDL_Joystick *joystick;
	enum {right, left, up, down, b, a, select_, start};
	int keyboard_config[8];
	int joystick_config[8];
#endif
