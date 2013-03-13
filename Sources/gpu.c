#include "gpu.h"

void gpu_init(){
	line_counter = 0;
	current_mode = 2;
	current_line = 0;
	//fonctions SDL
	if(!DEBUG){

		if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
		{
			fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
			exit(EXIT_FAILURE); // On quitte le programme
		}
		sdl_screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE);
		SDL_WM_SetCaption("Groboy", NULL);
		for(int i=0; i<144; i++)
		{
			for(int j=0; j<160; j++)
			{
				sdl_matrix[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
			}
		}
	}
}
void gpu_update(int cycles){ //fonction appelée en premier
	line_counter += cycles;
	gpu_update_line();
	gpu_update_stat();
}

void gpu_update_line(){
	BYTE lcdc;
	lcdc = memory_read(0xFF40);
	current_line = memory_read(0xFF44);
	if(lcdc & 0x80){
		if(line_counter > ONE_LINE_CYCLES){ //dépassement du temps pour générer une ligne
			if(current_line < LY_VISIBLE_MAX){//Si la ligne est dans le "champ de vision" 
				gpu_drawline();
			}
			current_line++;
			memory_write(0xFF44, current_line);
			line_counter %= ONE_LINE_CYCLES;
		}
		if(current_line >= LY_MAX){
			line_counter = 0;
			memory_write(0xFF44, 0);
		}
	}
}

void gpu_update_stat(){
	BYTE lcdc;//LCD Control 
	BYTE lyc, lcd_stat;

	lcdc = memory_read(0xFF40);
	current_line = memory_read(0xFF44);
	lyc = memory_read(0xFF45);
	lcd_stat = memory_read(0xFF41);

	if(current_line == lyc){//Vérification d'intersection entre LYC et LY
		lcd_stat |= 0x04;
		memory_write(0xFF41, lcd_stat);
		if(lcd_stat & 0x40)
			make_request(LCD_STAT);
	}
	else{
		lcd_stat &= 0xFB;
		memory_write(0xFF41, lcd_stat);
	}
	if(lcdc & 0x80){
		if(current_line < LY_VISIBLE_MAX){ 
			if(line_counter < 80){
				//Mode 2 The LCD controller is reading from OAM memory.
				lcd_stat &= 0xFC;
				lcd_stat |= 0x02;
				memory_write(0xFF41, lcd_stat);
				if(lcd_stat & 0x20){
					make_request(LCD_STAT);
				}
			}
			else if(line_counter < 172){
				//Mode 3 The LCD controller is reading from both OAM and VRAM
				lcd_stat |= 0x03;
				memory_write(0xFF41, lcd_stat);	
			}
			else{
				//Mode 0 The LCD controller is in the H-Blank period and 
				lcd_stat &= 0xFC;
				memory_write(0xFF41, lcd_stat);
				if(lcd_stat & 0x08){
					make_request(LCD_STAT);
				}
			}
		}
		else{
			if(current_line == LY_VISIBLE_MAX && (lcd_stat & 0x10))
				make_request(V_BLANK);
			draw_screen();
		}
	}
}

void gpu_drawline(){
	BYTE lcd_cont;
	int bg_y, bg_x;
	int window_y, window_x;
	int i,j;
	BYTE current_pixel = 0; 		//compteur allant de 0 à 160 pour savoir lorsque l'on finit la ligne
	BYTE cur_tile_px_x; 			//compteur pour savoir quel pixel récupérer sur la tuile et pour passer à la suivante lorsque > 7
	BYTE cur_tile_px_y; 			//Permet de savoir quelle ligne du pixel choisir
	unsigned short cur_tile_nb;		//num de la tuile courante
	tile_t tile; 				//tuile courante
	unsigned short tried_sprites[40];
	unsigned short temp_sprite;
	BYTE displyd_sprites_nb;
	BYTE sprite_size;
	BYTE sprite_sets;
	BYTE sprite_x;
	BYTE scy;
	BYTE scx;
	lcd_cont = memory_read(0xFF40);
	current_line;
	if(lcd_cont & 0x01){ //Si background établi
		
		scy = memory_read(0xFF42);
		scx = memory_read(0xFF43);
		bg_y = ((current_line + scy)/8)%32; 	//On récupère la ligne du background à dessiner
		bg_x = scx/8;			//On récupère la colonne du bg à dessiner

		tile.palette = memory_read(0xFF47);
		tile.x_flip = 0;
		tile.y_flip = 0;
		
		//printf("%d\n", bg_y*32 + bg_x);

		if(lcd_cont & 0x08)				// On regarde quelle est la background map			
			get_tile(memory_read(0x9C00 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
		else
			get_tile(memory_read(0x9800 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
		int i,j;
		for(i = 0x8000; i < 0x9FFF;i++){
		//	printf("%d\n", memory_read(i));
		}
		cur_tile_px_x = (scx) % 8;			//On récupère la position en x du pixel sur la tuile à dessiner
		cur_tile_px_y = (scy+current_line) % 8;		//On récupère la position en y du pixel sur la tuile à dessiner

		while(current_pixel < 160){			//On parcourt toute la ligne
			gpu_screen[current_line][current_pixel++] = tile.px[cur_tile_px_y][cur_tile_px_x++];
			if(cur_tile_px_x > 7){ 
				cur_tile_px_x = 0;
				bg_x = (bg_x + 1)%32;
				//printf("%d\n", bg_y*32 + bg_x);
				if(lcd_cont & 0x08)				// On regarde quelle est la background map			
					get_tile(memory_read(0x9C00 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
				else
					get_tile(memory_read(0x9800 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
			}
		}
	}
	if(lcd_cont & 0x20){ //Si Window établie
	/*	window_y = memory_read(0xFF4A);
		window_x = memory_read(0xFF4B) - 7;
		if(current_line >= window_y && window_x < 160){

			tile.palette = memory_read(0xFF47);
			tile.x_flip = 0;
			tile.y_flip = 0;
			cur_tile_nb = ((window_y - current_line)*4) + (window_x/8);
			if(lcd_cont & 0x40)				// On regarde quelle est la background map			
				get_tile(memory_read(cur_tile_nb + 0x9C00), &tile, WINDOW);	// On récupère la tuile correspondante	
			else
				get_tile(memory_read(cur_tile_nb + 0x9800), &tile, WINDOW);	// On récupère la tuile correspondante	

			cur_tile_px_x = window_x % 8;
			cur_tile_px_y = window_y % 8;

			while(window_x < 160){	//On parcourt toute la ligne
				if(window_x>=0)
					gpu_screen[current_line][window_x++] = tile.px[cur_tile_px_y][cur_tile_px_x++];
				window_x++;
				if(cur_tile_px_x > 7){ 
					cur_tile_px_x = 0;
					cur_tile_nb++;
					if(lcd_cont & 0x40)				// On regarde quelle est la background map			
						get_tile(memory_read(cur_tile_nb + 0x9C00), &tile, WINDOW);	// On récupère la tuile correspondante	
					else
						get_tile(memory_read(cur_tile_nb + 0x9800), &tile, WINDOW);	// On récupère la tuile correspondante	
				}
			}

		}*/
	}
	if(lcd_cont & 0x02){ //Si Sprites établis
		/*
		displyd_sprites_nb = 0;
		for(i = 0xFE00; i < 0xFE9F; i+= 4){//préparation des sprites pour le tri et récupération des sprites à afficher
			if(current_line >= memory_read(i) && current_line <= memory_read(i) + 7){ //memory_read(i) correspond à la pos en y du sprite
				tried_sprites[displyd_sprites_nb++] = i;
			}
		}
		i = 0;
		while(i < displyd_sprites_nb - 1){//tri des sprites
			if(memory_read(tried_sprites[i + 1] + 1) < memory_read(tried_sprites[i] + 1)){ //on compare la pos en x des deux sprites
				while(memory_read(tried_sprites[i + 1] + 1) < memory_read(tried_sprites[i] + 1)){
					temp_sprite = tried_sprites[i];
					tried_sprites[i] = tried_sprites[i+1];
					tried_sprites[i+1] = temp_sprite;
					i--;
				}
				i = 0;
			}
			else i++;
		}
		for(i = displyd_sprites_nb - 1; i >= 0; i--){//affichage des sprites
			sprite_sets = memory_read(tried_sprites[i] + 3); 	//caractéristiques du sprite
			sprite_x = memory_read(tried_sprites[i] + 1);		//position en x du sprite sur l'écran
			tile.x_flip = sprite_sets & 0x20;			//si il y'a un flip horizontal	
			tile.y_flip = sprite_sets & 0x40;			//si il y'a un flip vertical
			if(sprite_sets & 0x10)	tile.palette = memory_read(0xFF48);
			else tile.palette = memory_read(0xFF49);
			get_tile(memory_read(tried_sprites[i] + 2), &tile, SPRITES);
			cur_tile_px_y = (current_line - memory_read(tried_sprites[i])) % 8;
			for(j = 0; j < 8; j++){
				if(tile.px[cur_tile_px_y][j] != 0){ // Si le sprite n'est pas transparent
					if(sprite_sets & 0x80 || gpu_screen[current_line][sprite_x + i] == 0)	//si le sprite est dessus ou que le background est à 0
						gpu_screen[current_line][sprite_x + i] = tile.px[cur_tile_px_y][j]; // on dessine le pixel
				}
			}
		}*/
	}
}

//recupere la tuile 
//type 1 -> sprites
//type 2 -> background
//type 3 -> window
void get_tile(BYTE num, tile_t *tile, int type){
	int size = 8;// 8x8 ou 8x16
	int lig = 0;
	int i,j;
	unsigned short bg_wd_table_addr; //table du background & window
	BYTE palette;
	BYTE byte1,byte2,pos; 
	BYTE lcd_cont;
	BYTE pt;

	lcd_cont = memory_read(0xFF40);
	if (lcd_cont & 0x10) bg_wd_table_addr = 0x8000;
	else bg_wd_table_addr = 0x8800;

	if (type == SPRITES){
		if(lcd_cont & 0x04){
			size = 16;
			pos = 0x8000 + ((num & 0xFE) * 16);
		}
		else
			pos = 0x8000 + (num * 16);	
		//tile_table est unique pour les sprites elle est définie de 0x8000 à 0x8FFF
		//Il y'a 16 (0x10) octets par sprite et 256 sprites(0x100), donc 0x100 * 0x10 = 0x1000 octets.
		//Ce qui correspond bien à la plage 0x8000-0x8FFF
		//num permet de sélectionner un sprite entre 0 et FF.
	}
	else{
		if(lcd_cont & 0x10) pos = 0x8000 + (num * 16);
		else {
			BYTE_S num_s = (BYTE_S) num;
			pos = 0x8800 + (num_s + 128) * 16;
		}

	}
	for(pt=pos; pt < pos+(size*2); pt+=2)
	{
		byte1 = memory_read(pt);
		byte2 = memory_read(pt+1);
		for(i=0; i<8; i++)
		{
			if(byte1 & (1<<(7-i))) tile->px[lig][i] = 1;
			else tile->px[lig][i]=0;
			if(byte2 & (1<<(7-i))) tile->px[lig][i] |= 2;
		}
		lig++;
	}

	if(tile->x_flip !=0 )tile_flip(tile,0,size);
	if(tile->y_flip !=0 )tile_flip(tile,1,size);

	for(i = 0; i < size; i++){
		for(j = 0; j < 8; j++){
			if(tile->px[i][j] == 0 && type != SPRITES) tile->px[i][j] = tile->palette & 0x03;
			else if(tile->px[i][j] == 1) tile->px[i][j] = (tile->palette & 0x0C) >> 2;
			else if(tile->px[i][j] == 2) tile->px[i][j] = (tile->palette & 0x30) >> 4;
			else tile->px[i][j] = (tile->palette & 0xC0) >> 6;
		}
	}
	tile->size = size;
}


//prend en parametre une tile , 0 pour le flip horizontal ou 1 pour le flip vertical, la taille de la tile
void tile_flip(tile_t *tile, int flipx_y, int size)
{
	BYTE tempflip[16][8];
	int cpt;

	for(int i=0; i<16; i++)
	{
		for(int j=0; j<8; j++)
		{
			tempflip[i][j] = 0;
		}
	}

	if(flipx_y == 0)
	{

		for(int i=0; i<size; i++)
		{
			for(int j=7; j=0; j++)
			{
				tempflip[i][cpt] = tile->px[i][j];
				cpt++;
			}
		}
	}


	if(flipx_y == 1)
	{
		cpt = 0;
		for(int i=size-1; i=0; i--)
		{
			for(int j=0; j<8; j++)
			{
				tempflip[cpt][j] = tile->px[i][j];
			}
			cpt++;
		}
	}

	for(int i=0; i<16; i++)
	{
		for(int j=0; j<8; j++)
		{
			tile->px[i][j] = tempflip[i][j];
		}
	}
}

void draw_screen()
{
	if(!DEBUG){
		for(int i=0; i<144; i++)
		{
			for(int j=0; j<160; j++)
			{
				if(gpu_screen[i][j] == 0)
					*((Uint32*)(sdl_screen->pixels) + j + i * sdl_screen->w) = SDL_MapRGB(sdl_screen->format, 0,0,0);
				else if(gpu_screen[i][j] == 1)
					*((Uint32*)(sdl_screen->pixels) + j + i * sdl_screen->w) = SDL_MapRGB(sdl_screen->format, 85,85,85);
				else if(gpu_screen[i][j] == 2)
					*((Uint32*)(sdl_screen->pixels) + j + i * sdl_screen->w) = SDL_MapRGB(sdl_screen->format, 170,170,170);
				else 
					*((Uint32*)(sdl_screen->pixels) + j + i * sdl_screen->w) = SDL_MapRGB(sdl_screen->format, 255,255,255);
			}
		}
		SDL_Flip(sdl_screen); /* Mise à jour de l'écran */
		SDL_Delay(16);
	}
}
