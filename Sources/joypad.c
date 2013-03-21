#include "joypad.h"

void joypad_update(int cycles){
	static int joypad_counter = 0;	
	static BYTE joy_new;
	static BYTE joy_cur;
	static BYTE joy_old = 0;
	static BYTE key_buttons = 0xFF; //
	static Uint8 *keystate;
	SDL_Event event;

	joypad_counter += cycles;

	joy_cur = memory_read(0xFF00);
	if(joypad_counter > 80000){
		joypad_counter -= 80000;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					exit(0);
					break;
				default:
					break;
		}
		joypad_counter -= 80000;
		keystate = SDL_GetKeyState(NULL);
		if(keystate[SDLK_RIGHT]) key_buttons &=0xFE;
		else key_buttons |= 0x01;
		if(keystate[SDLK_LEFT]) key_buttons &=0xFD;
		else key_buttons |= 0x02;
		if(keystate[SDLK_UP]) key_buttons &=0xFB;
		else key_buttons |= 0x04;
		if(keystate[SDLK_DOWN]) key_buttons &=0xF7;
		else key_buttons |= 0x08;
		if(keystate[SDLK_d]) key_buttons &=0xEF;
		else key_buttons |= 0x10;
		if(keystate[SDLK_s]) key_buttons &=0xDF;
		else key_buttons |= 0x20;
		if(keystate[SDLK_RSHIFT]) key_buttons &=0xBF;
		else key_buttons |= 0x40;
		if(keystate[SDLK_RETURN]) key_buttons &=0x7F;
		else key_buttons |= 0x80;
		}
	}

	joy_new = 0x0F;
	if(0x10 & joy_cur) {
		joy_new &= ((key_buttons & 0xF0)>>4);
	}
	if(0x20 & joy_cur) {
		joy_new &= (key_buttons & 0x0F);
	}

	memory_write(0xFF00, (joy_cur & 0xF0) | joy_new);
	if(joy_new != joy_old){
		joy_old = joy_new;
		make_request(JOYPAD);
	}
}

