#ifndef GPU_H
#define GPU_H

#include "def.h"
#include "memory.h"
#include <tr1/functional>
#include "interrupts.h"
#define LY_VISIBLE_MAX 144
#define LY_MAX 154
#define ONE_LINE_CYCLES 456
#define SPRITES 1
#define BACKGROUND 2
#define WINDOW 3

enum{
    WHITE,
    LIGHT_GRAY,
    DARK_GRAY,
    BLACK
};

struct tile_t{
    BYTE px[16][8];
    BYTE size;
    BYTE x_flip;
    BYTE y_flip;
    BYTE palette;
};

struct sprite_t{
    BYTE x;
    BYTE y;
    BYTE pattern_nb;
    BYTE attributes;
};

class Gpu {
public:
    static const int BUFFER_WIDTH = 160;
    static const int BUFFER_HEIGHT = 144;
    Gpu();

    void update(int cycles);
    BYTE *get_buffer();
    void set_memory(Memory *memory);
    void set_ready_callback(std::tr1::function<void()> const &callback);
    void set_request_callback(std::tr1::function<void(int)> const &callback);
private:
    void gpu_drawblackline();
    void gpu_drawline();
    void get_tile(BYTE num, tile_t *tile, int type);
    void tile_flip(tile_t *tile, int flipx_y, int size);
    void swap_sprites(sprite_t *spr1, sprite_t *spr2);

    int current_line_;
    unsigned int vblank_clock_counter_;
    unsigned int line_clock_counter_;
    int frame_skip_;
    int frame_counter_;
    int timer1_;
    int timer2_;
    int cycle_length_;

    Memory *memory_;
    BYTE buffer_[BUFFER_WIDTH * BUFFER_HEIGHT];
    std::tr1::function<void()> ready_callback_;
    std::tr1::function<void(int)> request_callback_;
};


//#include "save_manager.h"
/*
#define LY_VISIBLE_MAX 144
#define LY_MAX 154
#define ONE_LINE_CYCLES 456
#define SPRITES 1
#define BACKGROUND 2
#define WINDOW 3
#define SDL_VIDEO_FLAGS (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)



int screen_mode; //0 pour fenetré , 1 pour plein ecran
BYTE current_line;
BYTE gpu_screen[144][160];
SDL_Surface* sdl_matrix[144][160];
SDL_Surface *sdl_screen;
SDL_Surface *sdl_screenTemp;
int timer1;
int timer2;
int cycle_length;
unsigned int vblank_clock_counter_;
unsigned int line_clock_counter_;
BYTE frame_skip;
BYTE frame_counter;

//SDL_Rect **resolutions; //test pour savoir les resolutions possibles
void gpu_init(SDL_Surface *sdl_scr);
void gpu_update(int cycles);
int save_gpu(FILE* file);
void restore_gpu(FILE *file);*/

#endif
