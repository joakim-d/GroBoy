#include "joypad.h"

void joypad_update(){
	BYTE joy_stat;
	SDL_Event event;

	joy_stat = memory_read(0xFF00);
	joy_stat |= 0xCF;
	while(SDL_PollEvent(&event)){;
		switch(event.type)
		{
			case SDL_QUIT:
				exit(0);
		}
	}
	memory_write(0xFF00, joy_stat);
}
