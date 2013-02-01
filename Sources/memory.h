#ifndef MEMORY_H
#define MEMORY_H
#define BYTE unsigned char

BYTE bank_selector;				//Permet de savoir quelle banque de rom est sélectionnée
BYTE ram_selector;				//Permet de savoir quelle banque de ram est sélectionnée
BYTE *cartridge_rom_buffer;			//Le buffer qui contient toutes les données d'une rom (représentant toutes les banques alignées)
						//chaque banque contient 0x4000 octets (16384)
BYTE *cartridge_ram_buffer;			//Le buffer de ram représentant la mémoire vive d'une cartouche
BYTE internal_ram[24576];			//Mémoire interne de la GB (n'étant pas en rapport avec la cartouche)
void memory_init();				//Méthode initialisant les buffers cartridge_rom_buffer et cartrigde_ram_buffer
BYTE memory_read(short addr);	
void memory_write(short addr, BYTE data);
#endif
