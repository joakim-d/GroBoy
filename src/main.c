#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"

void gui_init(){
	if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
	{   
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}   
}

int main(int argc, char * argv[]){
	char gamepath[4096];
	SDL_Surface *sdl_screen;
	gui_init();
	sdl_screen = SDL_SetVideoMode(160*2, 144*2, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	load_gui(sdl_screen);
	get_gamepath(gamepath);
	memory_init(gamepath);
	sound_init();
	cpu_init();
	gpu_init(sdl_screen);
	interrupts_init();
	joypad_init();
	run();
	return 0;
}
