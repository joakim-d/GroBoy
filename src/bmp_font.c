#include "bmp_font.h"

static Uint32 get_pixel32( int x, int y, SDL_Surface *surface )
{
	//Convertie les pixels en 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;

	//Recupere le pixel demande
	return pixels[ ( y * surface->w ) + x ];
}


int load_bmp(char *bmp_path){
	Uint32 bg_color;
	int cell_w;
	int cell_h;
	int p_x;
	int p_w;
	int current_char;
	int rows, cols, p_cols, p_rows;

	bitmap_font = NULL;
	bitmap_font = SDL_LoadBMP(bmp_path);

	if(bitmap_font == NULL){
		return -1;
	}

	bg_color = SDL_MapRGB(bitmap_font->format, 255, 255, 255);
	cell_w = (int) bitmap_font->w/16;
	cell_h = (int) bitmap_font->h/16;

	current_char = 0;
	for(rows = 0; rows < 16; rows++){
		for(cols = 0; cols < 16; cols++){
			chars[current_char].x = cell_w * cols;
			chars[current_char].y = cell_h * rows;

			chars[current_char].w = cell_w;
			chars[current_char].h = cell_h;
			//récupération du début de la position en x de la lettre
			for(p_cols = 0; p_cols < cell_w; p_cols++){
				for(p_rows = 0; p_rows < cell_h; p_rows++){
					p_x = chars[current_char].x + p_cols;
					p_w = chars[current_char].y + p_rows;
					if(get_pixel32(p_x, p_w, bitmap_font) != bg_color){
						chars[current_char].x = p_x;
					}
					p_cols = cell_w;
					p_rows = cell_h;
				}
			}
			//récupération de la largeur de la lettre
			for(p_cols = cell_w -1; p_cols >= 0; p_cols--){
				for(p_rows = 0; p_rows < cell_h; p_rows++){
					p_x = (cell_w * cols) + p_cols;
					p_w = (cell_h * rows) + p_rows;
					if(get_pixel32(p_x, p_w, bitmap_font) != bg_color){
						chars[current_char].w = (p_x - chars[ current_char ].x) + 1;
						p_cols = -1;
						p_rows = cell_h;
					}
				}
			}
			current_char++;
		}
	}
	return 0;
}

void write_text(char *text, int x, int y, SDL_Surface *surface){
	SDL_Rect pos_rect;
	int pos_x, pos_y;
	pos_x = x;
	pos_y = y;

	if(bitmap_font != NULL){
		for(int show = 0; text[show] != '\0'; show++){
			if(text[show] == ' '){
				pos_x += bitmap_font->w / 32;
			}
			else if(text[show] == '\n'){
				pos_y += bitmap_font->h /16;
				pos_x = x;
			}
			else{
				pos_rect.x = pos_x;
				pos_rect.y = pos_y;
				SDL_BlitSurface(bitmap_font, &chars[(int)text[show]], surface, &pos_rect);
				pos_x += chars[(int)text[show]].w + 1;
			}
		}
	}
}
