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
	BYTE bg_y, bg_x;
	int i;
	BYTE current_pixel = 0; //compteur allant de 0 à 160 pour savoir lorsque l'on finit la ligne
	BYTE cur_tile_px_x; 	//compteur pour savoir quel pixel récupérer sur la tuile et pour passer à la suivante lorsque > 7
	BYTE cur_tile_px_y; 	//Permet de savoir quelle ligne du pixel choisir
	BYTE cur_tile_nb;	//num de la tuile courante
	BYTE tyle[16][8]; 	//tuile courante
	BYTE sprite_size;
	lcd_cont = memory_read(0xFF40);
	if(lcd_cont & 0x01){ //Si background établi
		bg_y = currentline + memory_read(0xFF42); 	//On récupère la ligne du background à dessiner
		bg_x = memory_read(0xFF43);			//On récupère la colonne du bg à dessiner
		
		cur_tile_nb = (bg_y*4) + (bg_x/8);		// Calcul pour savoir quelle tuile : (num_Ligne/8px * 32) + (num_colonne/8)
		get_tyle(cur_tile_nb, &tyle);			// On récupère la tuile correspondante	
		
		cur_tile_px_x = bg_x % 8;			//On récupère la position en x du pixel sur la tuile à dessiner
		cur_tile_px_y = bg_y % sprite_size;		//On récupère la position en y du pixel sur la tuile à dessiner
 
		while(current_pixel < 160){			//On parcourt toute la ligne
			gpu_screen[currentline][current_pixel++] = tyle[cur_tile_px_y][cur_tile_px_x++];
			bg_x++;
			if(cur_tile_px_x > 7){ 
				cur_tile_px_x = 0;
				cur_tile_nb++;
				get_tyle(cur_tile_nb, &tyle);
			}
			if(bg_x > 255){
				bg_x = 0;
				cur_tile_nb = bg_y*4;
				get_tyle(cur_tile_nb, &tyle);
			}
		}
	}
	if(lcd_cont & 0x20){ //Si Window établie

	}
	if(lcd_cont & 0x02){ //Si Sprites établis

	}
}

void get_tyle(unsigned short num, BYTE *tyle[16][8]){
	BYTE bg_tile_table_addr;
	if(lcd_cont & 0x04) bg_tile_table_addr = 0x9C00;
	else bg_tile_table_addr = 0x9800;
	
}
