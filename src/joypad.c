#include "joypad.h"

void joypad_init(int redefine){
	int fd;
	if( (fd = open("config/joypad-config", O_RDONLY)) == -1){
		printf("Joypad config file not found. Using default config\nRight:\tright_arrow\nLeft:\tleft_arrow\nUp:\tup_arrow\nDown:\tdown_arrow\nB:\ts\nA:\td\nSelect:\tright_shift\nStart:\treturn\n");
		joypad_config[right] = SDLK_RIGHT;
		joypad_config[left] = SDLK_LEFT;
		joypad_config[up] = SDLK_UP;
		joypad_config[down] = SDLK_DOWN;
		joypad_config[b] = SDLK_s;
		joypad_config[a] = SDLK_d;
		joypad_config[select_] = SDLK_RSHIFT;
		joypad_config[start] = SDLK_RETURN;
		if( (fd = open("config/joypad-config", O_WRONLY | O_CREAT, 0644)) == -1){
			printf("Unable to create joypad config file.\n");
		}
		else{
			if(write(fd, joypad_config, sizeof(int) * 8) == -1){
				printf("Unable to write in the joypad config file.\n");
			}
		}
	}
	else{
		if(read(fd, joypad_config, sizeof(int) * 8) == -1){
			printf("Joypad config file not found. Using default config\nRight:\tright_arrow\nLeft:\tleft_arrow\nUp:\tup_arrow\nDown:\tdown_arrow\nB:\ts\nA:\td\nSelect:\tright_shift\nStart:\treturn\n");
			joypad_config[right] = SDLK_RIGHT;
			joypad_config[left] = SDLK_LEFT;
			joypad_config[up] = SDLK_UP;
			joypad_config[down] = SDLK_DOWN;
			joypad_config[b] = SDLK_s;
			joypad_config[a] = SDLK_d;
			joypad_config[select_] = SDLK_RSHIFT;
			joypad_config[start] = SDLK_RETURN;
		}
	}
}

void joypad_update(int cycles){
	static int joypad_counter = 0;	
	static BYTE joy_new;
	static BYTE joy_cur;
	static BYTE joy_old = 0;
	static BYTE key_buttons = 0xFF; //
	static Uint8 *keystate;

	joypad_counter += cycles;

	joy_cur = memory_read(0xFF00);
	if(joypad_counter > 80000){
		joypad_counter -= 80000;
		keystate = SDL_GetKeyState(NULL);
		if(keystate[joypad_config[right]]) key_buttons &=0xFE;
		else key_buttons |= 0x01;
		if(keystate[joypad_config[left]]) key_buttons &=0xFD;
		else key_buttons |= 0x02;
		if(keystate[joypad_config[up]]) key_buttons &=0xFB;
		else key_buttons |= 0x04;
		if(keystate[joypad_config[down]]) key_buttons &=0xF7;
		else key_buttons |= 0x08;
		if(keystate[joypad_config[a]]) key_buttons &=0xEF;
		else key_buttons |= 0x10;
		if(keystate[joypad_config[b]]) key_buttons &=0xDF;
		else key_buttons |= 0x20;
		if(keystate[joypad_config[select_]]) key_buttons &=0xBF;
		else key_buttons |= 0x40;
		if(keystate[joypad_config[start]]) key_buttons &=0x7F;
		else key_buttons |= 0x80;
		//test sauvegarde
		if(keystate[SDLK_F1]) {save_state();}
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

