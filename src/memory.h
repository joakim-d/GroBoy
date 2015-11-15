#ifndef MEMORY_H
#define MEMORY_H

#include "def.h"

#include "cartridge.h"

enum type_mem{ALL,
              FIRST_CART,
              SWITCH_CART,
              VRAM, EXTRAM,
              FIRST_WRAM,
              SWITCH_WRAM,ECHO,
              OAM,
              NONUSED,
              IO,
              HRAM,
              IER,
              DETAIL_VIDEO,
              DETAIL_SOUND,
              DETAIL_JOY,
              DETAIL_TIMER,
              DETAIL_INTERRUPTS
             };

class Memory{
public:
    Memory();
    ~Memory();

    BYTE read(int addr);
    void write(int addr, BYTE data);


    void load_cartridge(const std::string &path);
    void set_force_write(bool force);

    void reset();
private:
    void dma_transfer(BYTE data);

    Cartridge *cartridge_;
    BYTE internal_ram_[0x10000];
    bool force_write_;
};

/*BYTE cartridge_type;
BYTE enable_ram;
BYTE cartridge_ram_enabled;
BYTE rom_mode;
BYTE rom_selector;				//Permet de savoir quelle banque de rom est sélectionnée
BYTE ram_selector;				//Permet de savoir quelle banque de ram est sélectionnée
BYTE force_write;
char *game_name;
                        //chaque banque contient 0x4000 octets (16384)
BYTE *cartridge_ram_buffer;			//Le buffer de ram représentant la mémoire vive d'une cartouche
BYTE *cartridge_rom_buffer;			//Le buffer qui contient toutes les données d'une rom (représentant toutes les banques alignées)
BYTE internal_ram[0x10000];			//Mémoire interne de la GB (n'étant pas en rapport avec la cartouche)
void memory_init();				//Méthode initialisant les buffers cartridge_rom_buffer et cartrigde_ram_buffer
BYTE read(unsigned short addr);
void write(unsigned short addr, BYTE data);
void set_force_write();
void reset_force_write();
void memory_dump(int part); // un dump mémoire, ou part représente la partie de la mémoire à dumper
int save_memory(FILE *file);
void restore_memory(FILE* file);
void save_cartridge(FILE* file);
void restore_cartridge(size_t size);
char * getName();*/


#endif
