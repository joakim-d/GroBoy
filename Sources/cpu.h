#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
void run();
void init();
void read_rom_info(char * rom_path);	
char *rom_buffer;
typedef struct {
	unsigned short PC;
	unsigned short SP;
	unsigned char A;
	unsigned char F;
	unsigned char B;
	unsigned char C;
	unsigned char D;
	unsigned char E;
	unsigned char H;
	unsigned char L;	
	unsigned char A2;
	unsigned char F2;
	unsigned char B2;
	unsigned char C2;
	unsigned char D2;
	unsigned char E2;
	unsigned char H2;
	unsigned char L2;
	unsigned char I;
	unsigned char R;
} z80_t;
#endif
