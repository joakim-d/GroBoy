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
				gpu_draw_line(current_line);
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
