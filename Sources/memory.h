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
#include "sound.h"
enum type_mem{ALL,FIRST_CART, SWITCH_CART, VRAM, EXTRAM, FIRST_WRAM, SWITCH_WRAM,ECHO, OAM, NONUSED, IO, HRAM, IER, DETAIL_VIDEO, DETAIL_SOUND, DETAIL_JOY, DETAIL_TIMER, DETAIL_INTERRUPTS};
BYTE cartridge_type;
BYTE enable_ram;
BYTE cartridge_ram_enabled;
BYTE rom_mode;
BYTE rom_selector;				//Permet de savoir quelle banque de rom est sélectionnée
BYTE ram_selector;				//Permet de savoir quelle banque de ram est sélectionnée
BYTE force_write;
						//chaque banque contient 0x4000 octets (16384)
BYTE *cartridge_ram_buffer;			//Le buffer de ram représentant la mémoire vive d'une cartouche
BYTE *cartridge_rom_buffer;			//Le buffer qui contient toutes les données d'une rom (représentant toutes les banques alignées)
BYTE internal_ram[0x10000];			//Mémoire interne de la GB (n'étant pas en rapport avec la cartouche)
void memory_init();				//Méthode initialisant les buffers cartridge_rom_buffer et cartrigde_ram_buffer
inline BYTE memory_read(unsigned short addr);	
inline void memory_write(unsigned short addr, BYTE data);
void set_force_write();
void reset_force_write();
void memory_dump(int part); // un dump mémoire, ou part représente la partie de la mémoire à dumper
#endif
