#ifndef MEMORY_H
#define MEMORY_H
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "def.h"
BYTE cartridge_type;
BYTE enable_ram;
BYTE rom_mode;
BYTE rom_selector;				//Permet de savoir quelle banque de rom est sélectionnée
BYTE ram_selector;				//Permet de savoir quelle banque de ram est sélectionnée
BYTE *cartridge_rom_buffer;			//Le buffer qui contient toutes les données d'une rom (représentant toutes les banques alignées)
						//chaque banque contient 0x4000 octets (16384)
BYTE *cartridge_ram_buffer;			//Le buffer de ram représentant la mémoire vive d'une cartouche
BYTE internal_ram[32768];			//Mémoire interne de la GB (n'étant pas en rapport avec la cartouche)
unsigned short memory_init();				//Méthode initialisant les buffers cartridge_rom_buffer et cartrigde_ram_buffer
BYTE memory_read(unsigned short addr);	
void memory_write(unsigned short addr, BYTE data);
void write_mbc1(unsigned short addr, BYTE data);
void alloc_rom_mem(size_t size);
void alloc_ram_mem(size_t size);
void read_rom_info(char* rom_path);
#endif
