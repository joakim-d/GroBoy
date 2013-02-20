#include "gpu.h"

void gpu_init(){
	gpu_counter = 0;
	current_mode = 2;
	current_line = 0;
	//fonctions SDL
}
void gpu_update(int cycles){ //fonction appelée en premier
	gpu_counter += cycles;
	gpu_update_line();
	gpu_update_stat();
}

void gpu_update_line(){
	BYTE lcdc;
	BYTE current_line;
	lcdc = memory_read(0xFF40);
	current_line = memory_read(0xFF44);
	if(lcdc & 0x80){
		if(line_counter > ONE_LINE_CYCLES){ //dépassement du temps pour générer une ligne
			if(current_line < LY_VISIBLE_MAX){//Si la ligne est dans le "champ de vision" 
				gpu_draw_line();
			}
			current_line++;
			memory_write(0xFF44, current_line);
			gpu_counter %= ONE_LINE_CYCLES;
		}
		if(current_line == LY_VISIBLE_MAX){
			make_request(V_BLANK);
		}
		else if(current_line >= LY_MAX){
			line_counter = 0;
			memory_write(0xFF44, 0);
		}
	}
}

void gpu_update_stat(){
	BYTE lcdc;//LCD Control 
	BYTE current_line, lyc, lcd_stat;

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
		if(current_line < LY_MAX){ 
			if(gpu_counter < 80){
				//Mode 2 The LCD controller is reading from OAM memory.
				lcd_stat &= 0xFC;
				lcd_stat |= 0x02;
				memory_write(0xFF41, lcd_stat);
				if(lcd_stat & 0x20){
					make_request(LCD_STAT);
				}
			}
			else if(gpu_counter < 172){
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
			if(current_line == LY_MAX && (lcd_stat & 0x10))
				make_request(V_BLANK);
		}
	}
}

void gpu_drawline(){
	BYTE lcd_cont;
	int bg_y, bg_x;
	int window_y, window_x;
	int i,j;
	BYTE current_pixel = 0; //compteur allant de 0 à 160 pour savoir lorsque l'on finit la ligne
	BYTE cur_tile_px_x; 	//compteur pour savoir quel pixel récupérer sur la tuile et pour passer à la suivante lorsque > 7
	BYTE cur_tile_px_y; 	//Permet de savoir quelle ligne du pixel choisir
	BYTE cur_tile_nb;	//num de la tuile courante
	BYTE tyle[16][8]; 	//tuile courante
	unsigned short tried_sprites[40];
	unsigned short temp_sprite;
	BYTE displyd_sprites_nb;
	BYTE sprite_size;
	lcd_cont = memory_read(0xFF40);
	if(lcd_cont & 0x01){ //Si background établi
		bg_y = currentline + memory_read(0xFF42); 	//On récupère la ligne du background à dessiner
		bg_x = memory_read(0xFF43);			//On récupère la colonne du bg à dessiner

		cur_tile_nb = (bg_y*4) + (bg_x/8);		// Calcul pour savoir quelle tuile : (num_Ligne/8px * 32) + (num_colonne/8)
		get_tyle(cur_tile_nb, &tyle);			// On récupère la tuile correspondante	

		cur_tile_px_x = bg_x % 8;			//On récupère la position en x du pixel sur la tuile à dessiner
		cur_tile_px_y = bg_y % 8;			//On récupère la position en y du pixel sur la tuile à dessiner

		while(current_pixel < 160){			//On parcourt toute la ligne
			gpu_screen[currentline][current_pixel++] = tyle[cur_tile_px_y][cur_tile_px_x++];
			bg_x++;
			if(cur_tile_px_x > 7){ 
				cur_tile_px_x = 0;
				cur_tile_nb++;
				get_tyle(cur_tile_nb, &tyle, BACKGROUND);
			}
			if(bg_x > 255){
				bg_x = 0;
				cur_tile_nb = bg_y*4;
				get_tyle(cur_tile_nb, &tyle);
			}
		}
	}
	if(lcd_cont & 0x20){ //Si Window établie
		window_y = memory_read(0xFF4A);
		window_x = memory_read(0xFF4B) - 7;
		if(currentline >= window_y && window_x < 160){

			cur_tile_nb = ((window_y - currentline)*4) + (window_x/8);
			get_tyle(cur_tile_nb, &tyle, WINDOW);

			cur_tile_px_x = window_x % 8;
			cur_tile_px_y = window_y % 8;

			while(window_x < 160){	//On parcourt toute la ligne
				if(window_x>=0)
					gpu_screen[currentline][window_x++] = tyle[cur_tile_px_y][cur_tile_px_x++];
				window_x++;
				if(cur_tile_px_x > 7){ 
					cur_tile_px_x = 0;
					cur_tile_nb++;
					get_tyle(cur_tile_nb, &tyle, WINDOW);
				}
			}

		}
	}
	if(lcd_cont & 0x02){ //Si Sprites établis
		displyd_sprites_nb = 0;
		for(i = 0xFE00; i < 0xFE9F; i+= 4){//préparation des sprites pour le tri et récupération des sprites à afficher
			if(currentline >= memory_read(i) && currentline <= memory_read(i) + 7){
				tried_sprites[displyd_sprites_nb++] = i;
			}
		}
		i = 0;
		while(i < displyd_sprites_nb - 1){//tri des sprites
			if(memory_read(tried_sprites[i + 1] + 1) < memory_read(tried_sprites[i] + 1)){
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
			get_tyle(tried_sprites[i], tyle, SPRITES);
			cur_tyle_px_y = (currentline - tried_sprites[i]) % 8;
			for(j = 0; j < 8; j++){
				gpu_screen[currentline][memory_read(tried_sprites[i] + 1) + i] = tyle[cur_tyle_px_y][j];
			}
		}
	}
}

//recupere la tuile 
//type 1 -> sprite
//type 2 -> background
//type 3 -> window
int get_tyle(unsigned short num, BYTE *tyle[16][8], int type){
	int size = 8;// 8x8 ou 8x19
	int lig = 0;
	BYTE bg_tile_table_addr; //table du background
	BYTE wd_tile_table_addr; //table pour la fenetre
	BYTE tile_table_addr; //table des tuiles
	BYTE byte1,byte2,pos; 

	if(lcd_cont & 0x08) bg_tile_table_addr = 0x9C00;
	else bg_tile_table_addr = 0x9800;

	if(lcd_cond & 0x10) tile_table_addr = 0x8000;
	else tile_table_addr = 0x8800;

	if(lcd_cond & 0x40) wd_tile_table_addr = 0x9C00;
	else wd_tile_table_addre = 0x9800;


	switch(type)
	{
		case 1:
			if(lcd_cont & 0x04) size =16;
			pos  = read_memory(tile_table_addr + num);
			break;

		case 2:
			pos  = read_memory(bg_tile_table_addr + num);
			break;

		case 3: 
			pos  = read_memory(wd_tile_table_addr + num);
			break;
	}


	for(int pt=pos; pt=pos+15; pt+=2)
	{
		byte1 = read_memory(pt);
		byte2 = read_memory(pt+1);
		for(int i=0; i<8; i++)
		{
			if(byte1 & (1<<(7-i))) tyle[lig][i] = 1;
			else tyle[lig][i]=0;
			if(byte2 & (1<<(7-i))) tyle[lig][i] |= 2;
		}
		lig++;
	}

	return size;
}

