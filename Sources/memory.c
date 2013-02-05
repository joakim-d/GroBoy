#include "memory.h"

void memory_init(){
	int i;
	bank_selector = 0;
	ram_selector = 0;
	cartridge_rom_buffer = (BYTE *) malloc(256);
	cartridge_ram_buffer = (BYTE *) malloc(256);
	for(i = 0; i < 24576; i++){
		internal_ram[i] = 0;
	}
}

BYTE memory_read(short addr){
	return internal_ram[addr];
}

void memory_write(short addr, BYTE data){
	internal_ram[addr] = data;
}
