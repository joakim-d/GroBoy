#include "joypad.h"

void joypad_update(int cycles){
	static int joypad_counter = 0;	
	static BYTE joy_new;
	static BYTE joy_cur;
	static BYTE joy_old = 0;
	static BYTE key_buttons = 0xFF ; //
	SDL_Event event;

	joypad_counter += cycles;

	joy_cur = memory_read(0xFF00);

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_RIGHT: 
							key_buttons &= 0xFE;
							break;
						case SDLK_LEFT:
							key_buttons &= 0xFD;
							break;
						case SDLK_UP:
							key_buttons &= 0xFB;
							break;
						case SDLK_DOWN:
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
						case SDLK_RETURN:
							key_buttons &= 0x7F;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_RIGHT: 
							key_buttons |= 0x01;
							break;
						case SDLK_LEFT:
							key_buttons |= 0x02;
							break;
						case SDLK_UP:
							key_buttons |= 0x04;
							break;
						case SDLK_DOWN:
							key_buttons |= 0x08;
							break;
						case SDLK_d:
							key_buttons |= 0x10;
							break;
						case SDLK_s:
							key_buttons |= 0x20;
							break;
						case SDLK_RSHIFT:
							key_buttons |= 0x40;
							break;
						case SDLK_RETURN:
							key_buttons |= 0x80;
							break;
						default:
							break;
					}
					break;
				default:
					break;
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

