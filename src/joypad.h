#ifndef JOYPAD_H
#define JOYPAD_H
	#include "memory.h"
	#include "interrupts.h"
	//#include "save_manager.h"
	void joypad_update(int cycles);
	void joypad_init();
	enum {right, left, up, down, b, a, select, start};
	int joypad_config[8];

#endif
