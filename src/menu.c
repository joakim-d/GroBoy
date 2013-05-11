#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bmp_font.h"
#include "menu.h"
#include <dirent.h> 

void move_cursor(SDL_Surface *sdl_screen, int where){
	if(where == UP && cursor_pos > 0){
		write_text("\1", 5, cursor_pos*10 + 20, sdl_screen);
		cursor_pos--;
		write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
	}
	else if(where == DOWN && cursor_pos < 22){
		write_text("\1", 5, cursor_pos*10 + 20, sdl_screen);
		cursor_pos++;
		write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
	}
}
void upper(char *src, char *dst){
	int i;
	for(i = 0; i < strlen(src) && i < 24; i++){
		if(src[i] >= 97 && src[i] <= 122){
			dst[i] = src[i] - 32;
		}
		else dst[i] = src[i];
	}
	if(i == 24 && strlen(src) != 24) memcpy(dst+24, "...\0", 4);
	else dst[i] = '\0';
}

int filter(const struct dirent *a)
{
	char path[PATH_MAX];
	struct stat file_stat;
	strcpy(path, cwd);
	strcat(path, "/");
	strcat(path, a->d_name);
	stat(path, &file_stat);
	if(S_ISDIR(file_stat.st_mode)){return 1;}
	else{
		if(strcmp(a->d_name + strlen(a->d_name) - 3, ".gb") == 0) {return 1;}
		else {return 0;}
	}
}

int compare(const struct dirent **a, const struct dirent **b){
	struct stat a_stat;
	struct stat b_stat;
	char path_a[PATH_MAX];
	char path_b[PATH_MAX];

	strcpy(path_a, cwd);
	strcat(path_a, "/");
	strcat(path_a, (*a)->d_name);

	strcpy(path_b, cwd);
	strcat(path_b, "/");
	strcat(path_b, (*b)->d_name);

	stat(path_a, &a_stat);
	stat(path_b, &b_stat);
	if(S_ISDIR(a_stat.st_mode) && !S_ISDIR(b_stat.st_mode)){
		return -1;
	}
	else if(S_ISDIR(b_stat.st_mode) && !S_ISDIR(a_stat.st_mode)){
		return 1;
	}
	else return alphasort(a,b);
}
void display_menu(SDL_Surface *sdl_screen){
	if(strcmp(selected_game, "") == 0){
		SDL_FillRect(sdl_screen, NULL, bg_color);
		current_window = menu;
		write_text("GROBOY", sdl_screen->w/2 - 30, 0, sdl_screen);
		write_text("LOAD ROM", 15, 20, sdl_screen);
		write_text("CONFIG KEYBOARD", 15, 30, sdl_screen);
		write_text("EXIT", 15, 40, sdl_screen);
		write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
	}
	else{
		SDL_FillRect(sdl_screen, NULL, bg_color);
		current_window = menu;
		write_text("GROBOY", sdl_screen->w/2 - 30, 0, sdl_screen);
		write_text("PLAY", 15,20,sdl_screen);
		write_text("LOAD ROM", 15, 30, sdl_screen);
		write_text("CONFIG KEYBOARD", 15, 40, sdl_screen);
		write_text("EXIT", 15, 50, sdl_screen);
		write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
	}
}

void navigate(SDL_Surface *sdl_screen, char *path_name, char *destination){
	static Uint8 *keystate;
	struct dirent **files;
	struct stat file_stat;
	int nb_files;
	char buffer[PATH_MAX];
	int choice;
	int buf_len;
	int current_index;
	int i,j;

	SDL_FillRect(sdl_screen, NULL, bg_color);//Empty the sdl_screen
	nb_files = scandir(path_name,&files,filter, compare);//scanning the dir path_name

	write_text("\1", 5, cursor_pos*10 + 20, sdl_screen);
	cursor_pos = 0;
	write_text(">", 5, cursor_pos*10 + 20, sdl_screen);

	if(nb_files < 0){
		printf("Error when reading path_name %s\n", path_name);
		exit(1);
	}
	else{
		for(i = 0; i < nb_files && i < 23; i++){ //display files on sdl_screen
			upper(files[i]->d_name, buffer);
			write_text(buffer, 15, i*10 + 20, sdl_screen);	
		}
	}
	choice = 0;
	current_index = 0;
	SDL_Flip(sdl_screen);//update graphics
	strcpy(buffer, "");		//init buffer to verify if the user choose something or quit
	while(!choice){
		SDL_PumpEvents();		//To grab the last events
		keystate = SDL_GetKeyState(NULL);
		if(keystate[SDLK_UP]){	
			if(cursor_pos != 0){
				current_index--;
				move_cursor(sdl_screen, UP);
				SDL_Flip(sdl_screen);
			}
			else if(current_index != 0){
				current_index--;
				SDL_FillRect(sdl_screen, NULL, bg_color);//Empty the sdl_screen
				j = 0;
				write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
				for(i = current_index; i < nb_files && i < current_index + 12; i++){ //display files on sdl_screen
					upper(files[i]->d_name, buffer);
					write_text(buffer, 15, j*10 + 20, sdl_screen);
					j++;
				}
				SDL_Flip(sdl_screen);
			}
		}
		if(keystate[SDLK_DOWN]){
			if(cursor_pos != 22 && cursor_pos != nb_files - 1){
				current_index++;
				move_cursor(sdl_screen, DOWN);
				SDL_Flip(sdl_screen);
			}
			else if(current_index != nb_files - 1){
				current_index++;
				SDL_FillRect(sdl_screen, NULL, bg_color);//Empty the sdl_screen
				j = 0;
				write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
				for(i = current_index - 22; i < nb_files && i <= current_index; i++){
					upper(files[i]->d_name, buffer);
					write_text(buffer, 15, j*10 + 20, sdl_screen);
					j++;
				}
				SDL_Flip(sdl_screen);
			}
		}
		if(keystate[SDLK_RETURN]) {	//if user select a file
			stat(files[current_index]->d_name, &file_stat);
			if(S_ISDIR(file_stat.st_mode)){	
				if(strcmp(files[current_index]->d_name, ".") == 0){
					cursor_pos = 0;
					SDL_Flip(sdl_screen);
				}
				else if(strcmp(files[current_index]->d_name, "..") ==0){
					if(strrchr(path_name,'/') != path_name){ //if the previous dir is not the root
						buf_len = strlen(path_name) - strlen(strrchr(path_name, '/'));
						strncpy(buffer, path_name, buf_len);
						buffer[buf_len] = '\0';
						choice = 1;
					}
					else{
						strcpy(buffer, "/");
						choice = 1;
					}
				}
				else{
					strcpy(buffer, path_name);
					strcat(buffer,"/");
					strcat(buffer, files[current_index]->d_name);
					choice = 1;
				}
			}
			else{
				strcpy(buffer, path_name);
				strcat(buffer,"/");
				strcat(buffer, files[current_index]->d_name);
				choice = 1;
			}
		}
		if(keystate[SDLK_ESCAPE]) choice = 1;
		SDL_Delay(50);
	}
	if(strcmp(buffer, "") != 0){
		strcpy(destination, buffer);
	}
	for(i = 0; i < nb_files; i++){
		free(files[i]);
	}
	free(files);
	SDL_Delay(100);
}



void keyboard_conf(SDL_Surface *sdl_screen){
	int fd;
	int key_down = 0;
	char touch_string[8][20];
	SDL_Event event;
	strcpy(touch_string[0], "RIGHT:");
	strcpy(touch_string[1], "LEFT:");
	strcpy(touch_string[2], "UP:");
	strcpy(touch_string[3], "DOWN:");
	strcpy(touch_string[4], "B:");
	strcpy(touch_string[5], "A:");
	strcpy(touch_string[6], "SELECT:");
	strcpy(touch_string[7], "START:");

	current_window = config;
	SDL_FillRect(sdl_screen, NULL, bg_color);
	SDL_Flip(sdl_screen);

	while(!key_down){
		while (SDL_PollEvent(&event)){
			switch (event.type)
			{
				case SDL_KEYDOWN:
					key_down = 1;
					break;
			}
		}
	}
	key_down = 0;
	for(int i = 0; i < 8; i++){
		write_text(touch_string[i], 5, i*10 + 10, sdl_screen);
		SDL_Flip(sdl_screen);
		while(!key_down){
			while (SDL_PollEvent(&event)){
				switch (event.type)
				{
					case SDL_KEYDOWN:
						key_down = 1;
						joypad_config[i] = event.key.keysym.sym;
						break;
				}
			}
			SDL_Delay(60);
		}
		key_down = 0;
		strcat(touch_string[i], "...OK!");
		write_text(touch_string[i], 5, i*10 + 10, sdl_screen);
		SDL_Flip(sdl_screen);
		SDL_Delay(500);	
	}
	if((fd = open("config/joypad-config", O_WRONLY | O_CREAT, 0644)) == -1){
		printf("Error when creating joypad-config file\n");
	}
	else{
		if(write(fd, joypad_config, sizeof(int) * 8) == -1){
			printf("Unable to write in the joypad config file.\n");
		}   
		close(fd);
	}
	cursor_pos = 0;
	display_menu(sdl_screen);
}

void joystick_conf(SDL_Surface *sdl_screen){
	int fd;
	int key_down = 0;
	char touch_string[8][20];
	SDL_Event event;
	uint8_t touch_type[8];
	strcpy(touch_string[0], "RIGHT:");
	strcpy(touch_string[1], "LEFT:");
	strcpy(touch_string[2], "UP:");
	strcpy(touch_string[3], "DOWN:");
	strcpy(touch_string[4], "B:");
	strcpy(touch_string[5], "A:");
	strcpy(touch_string[6], "SELECT:");
	strcpy(touch_string[7], "START:");

	current_window = config;
	SDL_FillRect(sdl_screen, NULL, bg_color);
	SDL_Flip(sdl_screen);

	while(!key_down){
		while (SDL_PollEvent(&event)){
			switch (event.type)
			{
				case SDL_KEYDOWN:
					key_down = 1;
					break;
			}
		}
	}
	key_down = 0;
	for(int i = 0; i < 8; i++){
		write_text(touch_string[i], 5, i*10 + 10, sdl_screen);
		SDL_Flip(sdl_screen);
		while(!key_down){
			while (SDL_PollEvent(&event)){
				switch (event.type)
				{
					case SDL_JOYBUTTONDOWN:
						key_down = 1;
						joypad_config[i] = event.jbutton.button;
						touch_type[i] = 0;
						break;
					case SDL_JOYHATMOTION:
						if(event.jhat.value != SDL_HAT_CENTERED){
							joypad_config[i] = event.jhat.hat;
							key_down = 1;
							touch_type[i] = 1;
						}
						break;
				}
			}
			SDL_Delay(60);
		}
		key_down = 0;
		strcat(touch_string[i], "...OK!");
		write_text(touch_string[i], 5, i*10 + 10, sdl_screen);
		SDL_Flip(sdl_screen);
		SDL_Delay(500);	
	}
	if((fd = open("config/joystick-config", O_WRONLY | O_CREAT, 0644)) == -1){
		printf("Error when creating joystick-config file\n");
	}
	else{
		if(write(fd, joypad_config, sizeof(int) * 8) == -1){
			printf("Unable to write in the joystick config file.\n");
		}   
		close(fd);
	}
	cursor_pos = 0;
	display_menu(sdl_screen);
}
int menu_action(SDL_Surface *sdl_screen){
	char destination[0xFFFF];
	int action;
	action = (strcmp(selected_game, "") != 0)? cursor_pos: cursor_pos + 1;
	if(action == 0){
		return 1;
	}
	if(action == 1){
		if(getcwd(cwd, sizeof(cwd)) != NULL){
			navigate(sdl_screen, cwd, destination);	
			while(strcmp(destination, "") != 0 && strcmp(destination + strlen(destination) - 3, ".gb") != 0){
				strcpy(cwd, destination);
				navigate(sdl_screen, cwd, destination);
			}
			if(strcmp(destination + strlen(destination) - 3, ".gb") == 0){
				strcpy(selected_game,destination);
			}
			cursor_pos = 0;
			display_menu(sdl_screen);
		}
		else{
			printf("Error when getting current directory\n");
		}
	}
	else if(action == 2){
		keyboard_conf(sdl_screen);
	}
	/*else if(action == 3){
		joystick_conf(sdl_screen);
	}*/
	else if(action == 3){
		exit(1);
	}
	return 0;
}

void get_gamepath(char *gamepath){
	strcpy(gamepath, selected_game);
}	

void set_gamepath(char *gamepath){
	strcpy(selected_game, gamepath);
}

void get_gamename(char *gamename){
	char *temp;
	temp = strrchr(selected_game, '/');
	strcpy(gamename,temp + 1); 
}

void load_gui(SDL_Surface *sdl_screen){
	static Uint8 *keystate;
	int play = 0;

	if(load_bmp("fonts/gui_font.bmp") == -1){
		printf("Loading bmp font error\n");
	}

	bg_color = SDL_MapRGB(sdl_screen->format, 255, 255, 255);
	display_menu(sdl_screen);	
	cursor_pos = 0;
	while (!play){
		SDL_PumpEvents();
		keystate = SDL_GetKeyState(NULL);
		if(keystate[SDLK_UP]){
			move_cursor(sdl_screen, UP);
		}
		if(keystate[SDLK_DOWN]){
			if(strcmp(selected_game, "") == 0 && cursor_pos < 2) move_cursor(sdl_screen, DOWN);
			else if (strcmp(selected_game, "") != 0 && cursor_pos < 3) move_cursor(sdl_screen, DOWN);
		}
		if(keystate[SDLK_RETURN]) play = menu_action(sdl_screen);
		if(keystate[SDLK_ESCAPE]) break;
		SDL_Flip(sdl_screen);
		SDL_Delay(100);
	}
}
