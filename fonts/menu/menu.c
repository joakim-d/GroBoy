#include "bmp_font.h"

int main(int argc, char *argv[]){
	
	if(argc != 2){
		printf("Usage: %s <bmp_font>\n", argv[0]);
	}
	
	if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
	{   
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}   

	SDL_Surface *sdl_screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(load_bmp(argv[1]) == -1){
		printf("Loading bmp font error\n");
	}
	write_text("Hello world!\n", 5, 5, sdl_screen);
	SDL_Flip(sdl_screen);
	for(;;){
	SDL_Delay(1000);
	}
}
