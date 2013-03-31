#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"

void gui_init(){
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) // Démarrage de la SDL. Si erreur :
	{   
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}   
}

int main(int argc, char * argv[]){
	gui_init();
	memory_init(argv[1]);
	sound_init();
	cpu_init();
	gpu_init();
	interrupts_init();
	run();

	return 0;
}
