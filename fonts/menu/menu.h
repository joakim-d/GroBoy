#include "bmp_font.h"

int main(int argc, char *argv[]){
	
	
	if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
	{   
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}   

	SDL_Surface *sdl_screen = SDL_SetVideoMode(160, 144, 32, SDL_VIDEO_FLAGS);

}
