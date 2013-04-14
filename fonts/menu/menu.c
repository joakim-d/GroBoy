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
	else if(where == DOWN && cursor_pos < 11){
		write_text("\1", 5, cursor_pos*10 + 20, sdl_screen);
		cursor_pos++;
		write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
	}
}
void upper(char *src, char *dst){
	int i;
	for(i = 0; i < strlen(src); i++){
		if(src[i] >= 97 && src[i] <= 122){
			dst[i] = src[i] - 32;
		}
		else dst[i] = src[i];
	}
	dst[i] = '\0';
}

int filter(const struct dirent *a)
{
	struct stat file_stat;

	stat(a->d_name, &file_stat);
	if(S_ISDIR(file_stat.st_mode)){return 1;}
	else{
		if(strcmp(a->d_name + strlen(a->d_name) - 3, ".gb") == 0) {return 1;}
		else {return 0;}
	}
}

int compare(const struct dirent **a, const struct dirent **b){
	struct stat a_stat;
	struct stat b_stat;
	stat((*a)->d_name, &a_stat);
	stat((*b)->d_name, &b_stat);
	if(S_ISDIR(a_stat.st_mode) && !S_ISDIR(b_stat.st_mode)){
		return -1;
	}
	else if(S_ISDIR(b_stat.st_mode) && !S_ISDIR(a_stat.st_mode)){
		return 1;
	}
	else return alphasort(a,b);
}

void navigate(char *path_name){
	static Uint8 *keystate;
	SDL_FillRect(sdl_screen, NULL, bg_color);
	struct dirent **files;
	struct stat file_stat;
	DIR *rep;
	int drawed_file = 0;
	int nb_files;
	char buffer[0xFFFF];

	nb_files = scandir(path_name,&files,filter, compare);
	if(nb_files < 0){
		printf("error\n");
	}
	else{
		for(int i = 0; i < nb_files; i++){
			upper(files[i]->d_name, buffer);
			write_text(buffer, 15, i*10 + 20, sdl_screen);	
		}
	}
	SDL_Flip(sdl_screen);
	int key_down = 0;
	SDL_Event event;
	strcpy(buffer, "");
	for(;;){
		SDL_PumpEvents();
		keystate = SDL_GetKeyState(NULL);
		if(keystate[SDLK_UP]){
			move_cursor(sdl_screen, UP);
		}
		if(keystate[SDLK_DOWN]) move_cursor(sdl_screen, DOWN);
		if(keystate[SDLK_RETURN]) {
			stat(files[cursor_pos]->d_name, &file_stat);
			if(S_ISDIR(file_stat.st_mode)){
				strcpy(buffer, path_name);
				strcat(buffer,"/");
				strcat(buffer, files[cursor_pos]->d_name);
				printf("%s\n", buffer);
			}

			break;

		}
		if(keystate[SDLK_ESCAPE]) break;
		SDL_Flip(sdl_screen);
		SDL_Delay(100);
	}
	SDL_Delay(100);
	if(strcmp(buffer, "") != 0) navigate(buffer);

	SDL_Delay(100);
}

void init_menu(SDL_Surface *sdl_screen){
	SDL_FillRect(sdl_screen, NULL, bg_color);
	current_window = menu;
	cursor_pos = 0;
	write_text("GROBOY", sdl_screen->w/2 - 30, 0, sdl_screen);
	write_text("LOAD ROM", 15, 20, sdl_screen);
	write_text("CONFIG JOYPAD", 15, 30, sdl_screen);
	write_text("EXIT", 15, 40, sdl_screen);
	write_text(">", 5, cursor_pos*10 + 20, sdl_screen);
}


void joy_conf(){
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
	init_menu(sdl_screen);

}

void menu_action(){
	if(cursor_pos == 0){
		navigate(".");	
	}
	else if(cursor_pos == 1){
		joy_conf();
	}
	else if(cursor_pos == 2){
		exit(1);
	}
}

int main(int argc, char *argv[]){
	static Uint8 *keystate;
	if(argc != 2){
		printf("Usage: %s <bmp_font>\n", argv[0]);
	}

	if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
	{   
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}   

	sdl_screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(load_bmp(argv[1]) == -1){
		printf("Loading bmp font error\n");
	}
	bg_color = SDL_MapRGB(sdl_screen->format, 255, 255, 255);
	init_menu(sdl_screen);	
	for(;;){
		SDL_PumpEvents();
		keystate = SDL_GetKeyState(NULL);
		if(keystate[SDLK_UP]){
			move_cursor(sdl_screen, UP);
		}
		if(keystate[SDLK_DOWN]) move_cursor(sdl_screen, DOWN);
		if(keystate[SDLK_RETURN]) menu_action();
		if(keystate[SDLK_ESCAPE]) break;
		SDL_Flip(sdl_screen);
		SDL_Delay(100);
	}
	exit(1);
}
