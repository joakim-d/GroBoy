#include "joypad.h"

void joypad_init(int redefine){
	int fd;
	joy_old = 0;
	if(SDL_NumJoysticks() != 0){
		joystick = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);
	}
	else joystick = NULL;
	if( (fd = open("config/joypad-config", O_RDONLY)) == -1){
		printf("Joypad config file not found. Using default config\nRight:\tright_arrow\nLeft:\tleft_arrow\nUp:\tup_arrow\nDown:\tdown_arrow\nB:\ts\nA:\td\nSelect:\tright_shift\nStart:\treturn\n");
		keyboard_config[right] = SDLK_RIGHT;
		keyboard_config[left] = SDLK_LEFT;
		keyboard_config[up] = SDLK_UP;
		keyboard_config[down] = SDLK_DOWN;
		keyboard_config[b] = SDLK_s;
		keyboard_config[a] = SDLK_d;
		keyboard_config[select_] = SDLK_RSHIFT;
		keyboard_config[start] = SDLK_RETURN;
		if( (fd = open("config/joypad-config", O_WRONLY | O_CREAT, 0644)) == -1){
			printf("Unable to create joypad config file.\n");
		}
		else{
			if(write(fd, keyboard_config, sizeof(int) * 8) == -1){
				printf("Unable to write in the joypad config file.\n");
			}
			close(fd);
		}
	}
	else{
		if(read(fd, keyboard_config, sizeof(int) * 8) == -1){
			printf("Joypad config file not found. Using default config\nRight:\tright_arrow\nLeft:\tleft_arrow\nUp:\tup_arrow\nDown:\tdown_arrow\nB:\ts\nA:\td\nSelect:\tright_shift\nStart:\treturn\n");
			keyboard_config[right] = SDLK_RIGHT;
			keyboard_config[left] = SDLK_LEFT;
			keyboard_config[up] = SDLK_UP;
			keyboard_config[down] = SDLK_DOWN;
			keyboard_config[b] = SDLK_s;
			keyboard_config[a] = SDLK_d;
			keyboard_config[select_] = SDLK_RSHIFT;
			keyboard_config[start] = SDLK_RETURN;
		}
	}
}

void joypad_update(int cycles){
	static int joypad_counter = 0;	
	static Uint8 *keystate;
	static BYTE key_buttons = 0xFF; //
	static BYTE joy_new;
	static BYTE joy_cur;

	joypad_counter += cycles;

	joy_cur = memory_read(0xFF00);
	if(joypad_counter > 80000){
		joypad_counter -= 80000;
		keystate = SDL_GetKeyState(NULL);
		if(keystate[keyboard_config[right]]) key_buttons &=0xFE;
		else key_buttons |= 0x01;
		if(keystate[keyboard_config[left]]) key_buttons &=0xFD;
		else key_buttons |= 0x02;
		if(keystate[keyboard_config[up]]) key_buttons &=0xFB;
		else key_buttons |= 0x04;
		if(keystate[keyboard_config[down]]) key_buttons &=0xF7;
		else key_buttons |= 0x08;
		if(keystate[keyboard_config[a]]) key_buttons &=0xEF;
		else key_buttons |= 0x10;
		if(keystate[keyboard_config[b]]) key_buttons &=0xDF;
		else key_buttons |= 0x20;
		if(keystate[keyboard_config[select_]]) key_buttons &=0xBF;
		else key_buttons |= 0x40;
		if(keystate[keyboard_config[start]]) key_buttons &=0x7F;
		else key_buttons |= 0x80;
		if(joystick != NULL){
			if(SDL_JoystickGetButton(joystick, 0)) key_buttons &=0xFE;
		}
		//test sauvegarde
		if(keystate[SDLK_F1]) {save_state(1);SDL_Delay(100);}
		if(keystate[SDLK_F2]) {save_state(2);SDL_Delay(100);}
		if(keystate[SDLK_F3]) {save_state(3);SDL_Delay(100);}
		if(keystate[SDLK_F4]) {save_state(4);SDL_Delay(100);}
		
		if(keystate[SDLK_F5]) {restore(1);SDL_Delay(1000);}
		if(keystate[SDLK_F6]) {restore(2);SDL_Delay(100);}
		if(keystate[SDLK_F7]) {restore(3);SDL_Delay(100);}
		if(keystate[SDLK_F8]) {restore(4);SDL_Delay(100);}
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

void save_joypad(FILE *file){
	if(fwrite(&joy_old,sizeof(BYTE),1,file) != 1) printf("Error when writing joypad variables\n");
}

void restore_joypad(FILE *file){
	if(fread(&joy_old,sizeof(BYTE),1,file) != 1) printf("Error when writing joypad variables\n");
}
