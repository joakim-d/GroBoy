#include "joypad.h"

void joypad_update(int cycles){
	BYTE joy_stat;
	BYTE key_buttons; //
	SDL_Event event;

	static int joypad_counter = 0;	
	joypad_counter += cycles;

	if(joypad_counter > 80000){
		joy_stat = memory_read(0xFF00);
		joy_stat |= 0xCF;
		key_buttons = 0xFF;
		while(SDL_PollEvent(&event)){;
			switch(event.type)
			{
				case SDL_QUIT:
					exit(0);break;
			}
			switch(event.key.keysym.sym){
				case SDLK_RIGHT: 
					key_buttons &= 0xFE;
					break;
				case SDLK_LEFT:
					key_buttons &= 0xFD;
					break;
				case SDLK_DOWN:
					key_buttons &= 0xFB;
					break;
				case SDLK_UP:
					key_buttons &= 0xF7;
					break;
				case SDLK_d:
					key_buttons &= 0xEF;
					break;
				case SDLK_s:
					key_buttons &= 0xDF;
					break;
				case SDLK_RSHIFT:
					key_buttons &= 0xBF;
					break;
				case SDLK_BACKSPACE:
					key_buttons &= 0x7F;
					break;
				default: break;
			}
			joypad_counter %= 80000;
		}
		if(key_buttons != 0xFF) make_request(JOYPAD);
		if(!(0x20 & joy_stat)) joy_stat = (joy_stat & 0xF0) | (key_buttons & 0x0F);
		if(!(0x10 & joy_stat)) joy_stat = (joy_stat & 0xF0) | ((key_buttons & 0xF0)>>4);
		memory_write(0xFF00, joy_stat);
	}
}
