#include "gpu.h"

static inline void swap_sprites(sprite_t *spr1, sprite_t *spr2);
static inline void gpu_drawblackline();
static inline void gpu_drawline();
static inline void get_tile(BYTE num, tile_t *tile, int type);
static inline void tile_flip(tile_t *tile, int flipx_y, int size);
static inline void draw_screen();
static inline void ChangeMode();
static inline void event_process();
static inline void set_speed(uint16_t fps);
static inline void sleep_SDL();

void gpu_init(){
	current_line = 0;
	//fonctions SDL

	/** //test pour connaitre les resolutions possibles
	  resolutions =SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

	// On verifie si un mode est possible 
	if(resolutions == (SDL_Rect **)0){
	printf("Aucune résolution n'est possible!\n");
	exit(-1);
	}

	// On verifie si toutes les resolutions sont possibles 
	if(resolutions == (SDL_Rect **)-1){
	printf("Toutes les resolutions sont possibles.\n");
	}
	else{
	// Print valid modes 
	printf("Résolutions possibles :\n");
	for(int i=0;resolutions[i];++i)
	printf("  %d x %d\n", resolutions[i]->w, resolutions[i]->h);
	}	*/
	
/*/	sdl_screen = SDL_SetVideoMode(160*2, 144*2, 32, SDL_VIDEO_FLAGS); // remplacer 160 et 144 par resolustions[0]->w et resolutions[0]->h pour avoir la résolution la plus grande possible
        sdl_screenTemp = SDL_SetVideoMode(160*2, 144*2, 32, SDL_VIDEO_FLAGS); //SDL_CreateRGBSurface(SDL_SWSURFACE,160,144,32, 0, 0, 0, 0);
*/

	sdl_screenTemp =SDL_SetVideoMode(160*2, 144*2, 32, SDL_VIDEO_FLAGS); // remplacer 160 et 144 par resolustions[0]->w et resolutions[0]->h pour avoir la résolution la plus grande possible 
	SDL_WM_SetCaption("Groboy", NULL);
	screen_mode = 0;
	set_speed(60);
}

void gpu_update(int cycles){ //fonction appelée en premier
	BYTE lcdc;		//FF40 lcdcontrol
	BYTE lcdstat;		//FF41 lcdstat
	BYTE lyc;		//FF45 ly compare

	static unsigned int vblank_clock_counter = 0;
	static unsigned int line_clock_counter = 0;

	vblank_clock_counter += cycles;
	if(vblank_clock_counter >= 70224){//si on dépasse la période de vblank
		current_line = 0;
		vblank_clock_counter -= 70224;
		line_clock_counter = vblank_clock_counter;
	}
	current_line = vblank_clock_counter / 456;
	set_force_write();
	memory_write(0xFF44, current_line);
	reset_force_write();

	lcdc = memory_read(0xFF40);
	lcdstat = memory_read(0xFF41);
	if(lcdc & 0x80){					//si LCD est sur on
		lyc = memory_read(0xFF45);
		if(current_line == lyc){			//on compare ligne actuelle avec lyc
			if(!(lcdstat & 0x04)){			//si le flag n'est toujours pas mis
				lcdstat |= 0x04;
				memory_write(0xFF41, lcdstat);
				if(lcdstat & 0x40){
					make_request(LCD_STAT);
				}
			}
		}
		else{
			if(lcdstat & 0x04){	
				lcdstat &= 0xFB;
				memory_write(0xFF41, lcdstat);
			}
		}
	} 

	if(vblank_clock_counter >= 65664){ //Si on entre dans la période de vblank
		if(!(lcdstat & 0x01)){
			lcdstat &= 0xFC;
			memory_write(0xFF41, lcdstat | 1);
			draw_screen();
			make_request(V_BLANK);
			if(lcdstat & 0x10) make_request(LCD_STAT);
		}
	}
	else{
		line_clock_counter += cycles;
		if(line_clock_counter >= 456) line_clock_counter -= 456;
		if(line_clock_counter <= 80){ //mode 2
			if(!(lcdstat & 0x02) != 2){
				lcdstat &= 0xFC;
				memory_write(0xFF41, lcdstat | 2);
				if(lcdstat & 0x20) make_request(LCD_STAT);
			}
		}
		else if(line_clock_counter <= 252){ //mode 3
			if((lcdstat & 0x03) != 3){
				memory_write(0xFF41, lcdstat | 3);
			}
		}
		else { 				//mode 0 
			if(lcdstat & 0x03){ 	
				memory_write(0xFF41, lcdstat & 0xFC);
				if(lcdstat & 0x08){
					make_request(LCD_STAT);
				}
				gpu_drawline();
			}
		}
	}
}

static inline void gpu_drawblackline(){
	int i;
	for(i = 0; i < 160;i++){
		gpu_screen[current_line][i] = 3;
	}
}

static inline void gpu_drawline(){
	int bg_y, bg_x;
	int window_y, window_x;
	int i,j;
	BYTE lcd_cont;
	BYTE current_pixel = 0; 		//compteur allant de 0 à 160 pour savoir lorsque l'on finit la ligne
	BYTE cur_tile_px_x; 			//compteur pour savoir quel pixel récupérer sur la tuile et pour passer à la suivante lorsque > 7
	BYTE cur_tile_px_y; 			//Permet de savoir quelle ligne du pixel choisir
	static BYTE ordered_sprites_num[40];
	BYTE displyd_sprites_nb;
	BYTE sprite_size;
	BYTE scy;
	BYTE scx;
	static tile_t tile; 				//tuile courante
	static sprite_t sprites[40];

	lcd_cont = memory_read(0xFF40);

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

		cur_tile_px_x = (scx) % 8;			//On récupère la position en x du pixel sur la tuile à dessiner
		cur_tile_px_y = (scy+current_line) % 8;		//On récupère la position en y du pixel sur la tuile à dessiner

		while(current_pixel < 160){			//On parcourt toute la ligne
			gpu_screen[current_line][current_pixel++] = tile.px[cur_tile_px_y][cur_tile_px_x++];
			if(cur_tile_px_x > 7){ 
				cur_tile_px_x = 0;
				bg_x = (bg_x + 1)%32;
				if(lcd_cont & 0x08)				// On regarde quelle est la background map			
					get_tile(memory_read(0x9C00 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
				else
					get_tile(memory_read(0x9800 + bg_y*32 + bg_x), &tile, BACKGROUND);	// On récupère la tuile correspondante	
			}
		}
	}

	if(lcd_cont & 0x20){ //Si Window établie

		scy = memory_read(0xFF4A);
		scx = memory_read(0xFF4B);
		if(current_line >= scy && scx < 167){//si la ligne actuelle est bien au dessus de la window et que la window apparait en x
			window_y = (current_line - scy)/8;
			window_x = 0;
			tile.palette = memory_read(0xFF47);
			tile.x_flip = 0;
			tile.y_flip = 0;
			if(lcd_cont & 0x40)				// On regarde quelle est la background map			
				get_tile(memory_read(0x9C00 + window_y*32), &tile, WINDOW);	// On récupère la tuile correspondante	
			else
				get_tile(memory_read(0x9800 + window_y*32), &tile, WINDOW);	// On récupère la tuile correspondante	

			cur_tile_px_x = 0;
			cur_tile_px_y = (current_line - scy) % 8;
			while(scx < 167){	//On parcourt toute la ligne
				if(scx>=7)
					gpu_screen[current_line][scx - 7] = tile.px[cur_tile_px_y][cur_tile_px_x];
				scx++;
				cur_tile_px_x++;
				if(cur_tile_px_x > 7){ 
					cur_tile_px_x = 0;
					window_x++;
					if(lcd_cont & 0x40)				// On regarde quelle est la background map			
						get_tile(memory_read(0x9C00 + window_y*32 + window_x), &tile, WINDOW);	// On récupère la tuile correspondante	
					else
						get_tile(memory_read(0x9800 + window_y*32 + window_x), &tile, WINDOW);	// On récupère la tuile correspondante	
				}
			}

		}
	}
	if(lcd_cont & 0x02){ //Si Sprites établis

		displyd_sprites_nb = 0;
		j = 0;
		for(i = 0xFE00; i < 0xFE9F; i+= 4){//préparation des sprites pour le tri et récupération des sprites à afficher
			sprites[j].y = memory_read(i);
			sprites[j].x = memory_read(i + 1);
			sprites[j].pattern_nb = memory_read(i + 2);
			sprites[j].attributes = memory_read(i + 3);
			if(lcd_cont & 0x04) sprite_size = 16;
			else sprite_size = 8;
			if(current_line >= (sprites[j].y - 16) && current_line < (sprites[j].y - 16 + sprite_size)){
				ordered_sprites_num[displyd_sprites_nb++] = j;
			}
			j++;
		}
		i = 0;
		while(i < displyd_sprites_nb - 1){//tri des sprites
			if(sprites[ordered_sprites_num[i + 1]].x < sprites[ordered_sprites_num[i]].x){ //on compare la pos en x des deux sprites
				while(sprites[ordered_sprites_num[i + 1]].x < sprites[ordered_sprites_num[i]].x && i>=0){
					swap_sprites(sprites + ordered_sprites_num[i+1], sprites + ordered_sprites_num[i]);
					i--;
				}
				i = 0;
			}
			else i++;
		}
		for(i = displyd_sprites_nb - 1; i >= 0; i--){//affichage des sprites
			tile.x_flip = sprites[ordered_sprites_num[i]].attributes & 0x20;			//si il y'a un flip horizontal	
			tile.y_flip = sprites[ordered_sprites_num[i]].attributes & 0x40;			//si il y'a un flip vertical
			if(sprites[ordered_sprites_num[i]].attributes & 0x10)	tile.palette = memory_read(0xFF49);
			else tile.palette = memory_read(0xFF48);

			get_tile(sprites[ordered_sprites_num[i]].pattern_nb, &tile, SPRITES);
			cur_tile_px_y = current_line + 16 - sprites[ordered_sprites_num[i]].y; 
			for(j = 0; j < 8; j++){
				if(tile.px[cur_tile_px_y][j] != 4 && sprites[ordered_sprites_num[i]].x + j > 7 && sprites[ordered_sprites_num[i]].x - 8 + j < 160){ // Si le sprite n'est pas transparent
					if((!(sprites[ordered_sprites_num[i]].attributes & 0x80) || gpu_screen[current_line][sprites[ordered_sprites_num[i]].x -8 + j] == 0)){	//si le sprite est dessus ou que le background est à 0
						gpu_screen[current_line][sprites[ordered_sprites_num[i]].x - 8 +j] = tile.px[cur_tile_px_y][j]; // on dessine le pixel
					}
				}
			}
		}
	}
}

//recupere la tuile 
//type 1 -> sprites
//type 2 -> background
//type 3 -> window
static inline void get_tile(BYTE num, tile_t *tile, int type){
	int size;// 8x8 ou 8x16
	int lig;
	int i;
	BYTE byte1,byte2; 
	BYTE lcd_cont;
	int pt;
	unsigned short pos;
	lcd_cont = memory_read(0xFF40);

	if (type == SPRITES){
		if(lcd_cont & 0x04){
			size = 16;
			pos = 0x8000 + ((num & 0xFE) * 16);
		}
		else{
			pos = 0x8000 + (num * 16);	
			size = 8;
		}
		//tile_table est unique pour les sprites elle est définie de 0x8000 à 0x8FFF
		//Il y'a 16 (0x10) octets par sprite et 256 sprites(0x100), donc 0x100 * 0x10 = 0x1000 octets.
		//Ce qui correspond bien à la plage 0x8000-0x8FFF
		//num permet de sélectionner un sprite entre 0 et FF.
	}
	else{
		size = 8;
		if(lcd_cont & 0x10) pos = 0x8000 + (num * 16);
		else {
			BYTE_S num_s = (BYTE_S) num;
			pos = 0x8800 + (num_s + 128) * 16;
		}
	}
	lig = 0;
	for(pt=pos; pt < pos+(size*2); pt+=2)
	{
		byte1 = memory_read(pt);
		byte2 = memory_read(pt+1);
		for(i=0; i<8; i++)
		{
			if(byte1 & (1<<(7-i))) tile->px[lig][i] = 1;
			else tile->px[lig][i]=0;
			if(byte2 & (1<<(7-i))) tile->px[lig][i] |= 2;

			if(tile->px[lig][i] == 0){
				if(type != SPRITES) tile->px[lig][i] = tile->palette & 0x03;
				else tile->px[lig][i] = 4;
			}
			else if(tile->px[lig][i] == 1) tile->px[lig][i] = (tile->palette & 0x0C) >> 2;
			else if(tile->px[lig][i] == 2) tile->px[lig][i] = (tile->palette & 0x30) >> 4;
			else tile->px[lig][i] = (tile->palette & 0xC0) >> 6;
		}
		lig++;
	}

	//if(tile->x_flip && tile->y_flip) tile_flip(tile,3,size);
	//else if(tile->x_flip)tile_flip(tile,1,size);
	//else if(tile->y_flip)tile_flip(tile,2,size);
	if(tile->x_flip)tile_flip(tile,0,size);
	if(tile->y_flip)tile_flip(tile,1,size);

	tile->size = size;
}


//prend en parametre une tile , 0 pour le flip horizontal ou 1 pour le flip vertical, la taille de la tile
static inline void tile_flip(tile_t *tile, int flipx_y, int size)
{
	BYTE tempflip[16][8];
	int cpt;

	if(flipx_y == 0)
	{
		for(int i=0; i<size; i++)
		{
			cpt = 0;
			for(int j=7; j>=0; j--)
			{
				tempflip[i][cpt] = tile->px[i][j];
				cpt++;
			}
		}
	}
	if(flipx_y == 1)
	{
		cpt = 0;
		for(int i=size-1; i>=0; i--)
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

static inline void draw_screen()
{
	static BYTE frame_skip = 0;
	static BYTE frame_counter = 0;
	event_process();
	if(frame_counter < frame_skip) frame_counter++;
	else{
		frame_counter = 0;
		for(int i=0; i<144; i++)
		{
			for(int j=0; j<160; j++)
			{
				if(gpu_screen[i][j] == 0)
					*((Uint32*)(sdl_screenTemp->pixels) + j + i * sdl_screenTemp->w) = SDL_MapRGB(sdl_screenTemp->format, 255,255,255);
				else if(gpu_screen[i][j] == 1)
					*((Uint32*)(sdl_screenTemp->pixels) + j + i * sdl_screenTemp->w) = SDL_MapRGB(sdl_screenTemp->format, 170,170,170);
				else if(gpu_screen[i][j] == 2)
					*((Uint32*)(sdl_screenTemp->pixels) + j + i * sdl_screenTemp->w) = SDL_MapRGB(sdl_screenTemp->format, 85,85,85);
				else 
					*((Uint32*)(sdl_screenTemp->pixels) + j + i * sdl_screenTemp->w) = SDL_MapRGB(sdl_screenTemp->format, 0,0,0);
			}
		}
		//SDL_SoftStretch(sdl_screenTemp, NULL, sdl_screen, NULL);
		SDL_Flip(sdl_screenTemp); /* Mise à jour de l'écran */
		sleep_SDL();
	}
}

static inline void set_speed(uint16_t fps){
	cycle_length = 1000/fps;
	timer1 = SDL_GetTicks();
}

static inline void sleep_SDL(){
	while(1){
		timer2 = SDL_GetTicks() - timer1;
		if(timer2 >= cycle_length) break;
		else SDL_Delay(2);
	}
	timer1 = SDL_GetTicks() - (timer2 % cycle_length);
}

static inline void swap_sprites(sprite_t *spr1, sprite_t *spr2){
	sprite_t spr_temp;

	spr_temp.x = spr1->x;
	spr_temp.y = spr1->y;
	spr_temp.pattern_nb = spr1->pattern_nb;
	spr_temp.attributes = spr1->attributes;

	spr1->x = spr2->x;
	spr1->y = spr2->y;
	spr1->pattern_nb = spr2->pattern_nb;
	spr1->attributes = spr2->attributes;

	spr2->x = spr_temp.x;
	spr2->y = spr_temp.y;
	spr2->pattern_nb = spr_temp.pattern_nb;
	spr2->attributes = spr_temp.attributes;
}

static inline void event_process()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
				/**case SDL_KEYDOWN:
				  if(event.key.keysym.sym==SDLK_ESCAPE)
				  {
				  ChangeMode();
				  }
				  break;*/
			case SDL_VIDEORESIZE:
				//SDL_Surface *sdl_screen_copie;
				sdl_screen = SDL_SetVideoMode(event.resize.w, event.resize.h,32,SDL_HWSURFACE | SDL_RESIZABLE);
				SDL_SoftStretch(sdl_screenTemp, NULL, sdl_screen, NULL);
				SDL_Flip(sdl_screen);
				break;
			default:break;

		}
	}
}


//Passer du mode fenetré au mode plein ecran et inversement
static inline void ChangeMode()
{
	if(screen_mode==0)
	{	
		sdl_screen = SDL_SetVideoMode(0, 0, 32, SDL_VIDEO_FLAGS ^ SDL_FULLSCREEN); /* Passe en mode plein écran */
		if(sdl_screen == NULL) sdl_screen = SDL_SetVideoMode(0, 0, 0, SDL_VIDEO_FLAGS); /* Si le changement échoue, réinitialise la fenêtre avec la configuration précédente */
		if(sdl_screen == NULL) exit(1); /* Si la réinitialisation échoue, alors c'est un échec */
		SDL_SoftStretch(sdl_screenTemp, NULL, sdl_screen, NULL);
		screen_mode = 1;
	}
	else
	{
		sdl_screen = SDL_SetVideoMode(160,144,32,SDL_VIDEO_FLAGS);
		SDL_SoftStretch(sdl_screenTemp, NULL, sdl_screen, NULL);
		screen_mode = 0;
	}
	SDL_Flip(sdl_screen);
}

int save_gpu(FILE* fichier)
{
	int nb=0;
	int nb_element=7;
	nb += fwrite(&line_clock_counter,sizeof(int),1,fichier);
        nb += fwrite(&vblank_clock_counter,sizeof(int),1,fichier);
        nb += fwrite(&screen_mode,sizeof(int),1,fichier);
        nb += fwrite(&current_line,sizeof(BYTE),1,fichier);
        nb += fwrite(&timer1,sizeof(int),1,fichier);
        nb += fwrite(&timer2,sizeof(int),1,fichier);
        nb += fwrite(&cycle_length,sizeof(int),1,fichier);
	if(nb!=nb_element) printf("erreur d'écriture des variables du gpu");
	return nb;
}

void restore_gpu(FILE * fichier)
{
	fread(&line_clock_counter,sizeof(int),1,fichier);
	fread(&vblank_clock_counter,sizeof(int),1,fichier);
	fread(&screen_mode,sizeof(int),1,fichier);
	fread(&current_line,sizeof(BYTE),1,fichier);
	fread(&timer1,sizeof(int),1,fichier);
	fread(&timer2,sizeof(int),1,fichier);
	fread(&cycle_length,sizeof(int),1,fichier);
}

