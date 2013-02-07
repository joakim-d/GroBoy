#include "memory.h"

void read_rom_info(char* rom_path){
	int file_d;
	struct stat file_stat;

	file_d = open(rom_path, O_RDONLY);
	fstat(file_d, &file_stat);

	cartridge_rom_buffer = (BYTE *) malloc(file_stat.st_size);

	read(file_d, cartridge_rom_buffer, file_stat.st_size);
	close(file_d);

	if(memcmp(cartridge_rom_buffer + 0x0100, "\x00\xC3", 2) != 0){ //On vérifie si la rom est bien un fichier gb
		printf("Error reading rom magic code\n");
		exit(-1);
	}

	printf("Game title: %s\n", cartridge_rom_buffer + 0x0134);
	printf("Cartridge type: ");	
	cartridge_type = *(cartridge_rom_buffer + 0x0147);
	switch(*(cartridge_rom_buffer + 0x0147)){
		case 0x00: printf("ROM ONLY");break;
		case 0x01: printf("MBC1");break;
		case 0x02: printf("MBC1+RAM");break;
		case 0x03: printf("MBC1+RAM+BATTERY");break;
		case 0x05: printf("MBC2");break;
		case 0x06: printf("MBC2+BATTERY");break;
		case 0x08: printf("ROM+RAM");break;
		case 0x09: printf("ROM+RAM+BATTERY");break;
		case 0x0B: printf("MMM01");break;
		case 0x0C: printf("MMM01+RAM");break;
		case 0x0D: printf("MMM01+RAM+BATTERY");break;
		case 0x0F: printf("MBC3+TIMER+BATTERY");break;
		case 0x10: printf("MBC3+TIMER+RAM+BATTERY");break;
		case 0x11: printf("MBC3");break;
		case 0x12: printf("MBC3+RAM");break;
		case 0x13: printf("MBC3+RAM+BATTERY");break;
		case 0x15: printf("MBC4");break;
		case 0x16: printf("MBC4+RAM");break;
		case 0x17: printf("MBC4+RAM+BATTERY");break;
		case 0x19: printf("MBC5");break;
		case 0x1A: printf("MBC5+RAM");break;
		case 0x1B: printf("MBC5+RAM+BATTERY");break;
		case 0x1C: printf("MBC5+RUMBLE");break;
		case 0x1D: printf("MBC5+RUMBLE+RAM");break;
		case 0x1E: printf("MBC5+RUMBLE+RAM+BATTERY");break;
		case 0xFC: printf("POCKET CAMERA");break;
		case 0xFD: printf("BANDAI TAMA5");break;
		case 0xFE: printf("HuC3");break;
		case 0xFF: printf("HuC1+RAM+BATTERY");break;
		default: printf("UNKNOWN");break;
	}
	printf("\nRom Size ");
	switch(*(cartridge_rom_buffer + 0x0148)){
		case 0x00: printf("32KB: 2 Banks");break;
		case 0x01: printf("64KB: 4 Banks");break;
		case 0x02: printf("128KB: 8 Banks");break;
		case 0x03: printf("256KB: 16 Banks");break;
		case 0x04: printf("512KB: 32 Banks");break;
		case 0x05: printf("1MB: 64 Banks");break;
		case 0x06: printf("2MB: 128 Banks");break;
		case 0x07: printf("4MB: 256 Banks");break;
		case 0x52: printf("1.1MB: 72 Banks");break;
		case 0x53: printf("1.2MB: 80 Banks");break;
		case 0x54: printf("1.5MB: 96 Banks");break;
	}
	printf("\nRam Size ");
	switch(*(cartridge_rom_buffer + 0x0149)){
		case 0: printf("None");break;
		case 1: printf("2KB: 1 Bank");break;
		case 2: printf("8KB: 1 Bank");break;
		case 3: printf("32KB: 4 Banks");break;
	}
	printf("\nLanguage ");
	switch(*(cartridge_rom_buffer + 0x014A)){
		case 0: printf("Japanese Game");break;
		case 1: printf("English Game");break;
	}
	printf("\nManufacturer");
	switch(*(cartridge_rom_buffer + 0x014B)){
		case 0x33: printf("Nintendo or extended");break;
		case 0x79: printf("Accolade");break;
		case 0xA4: printf("Konami");break;
	}
	printf("\nVersion number %d\n", cartridge_rom_buffer[0x014C]);
}

void memory_init(){
	int i;
	rom_selector = 0;
	ram_selector = 0;
	cartridge_rom_buffer = (BYTE *) malloc(256);
	cartridge_ram_buffer = (BYTE *) malloc(256);
	for(i = 0; i < 24576; i++){
		internal_ram[i] = 0;
	}
}

BYTE memory_read(short addr){
	if(addr >= 0 && addr <= 0x3FFF) 		//demande de lecture dans la banque de rom 0 de la cartouche
		return cartridge_rom_buffer[addr];
	if(addr >= 0x4000 && addr <= 0x7FFF) 		//demande de lecture dans la banque de rom 1..n de la cartouche
		return cartridge_rom_buffer[0x4000*(rom_selector - 1) + addr];
	if(addr >= 0x8000 && addr <= 0x9FFF) 		//demande de lecture vidéo ram
		return internal_ram[addr - 0x8000];
	if(addr >= 0xA000 && addr <= 0xBFFF)		//demande de lecture dans la banque de ram 0..n de la cartouche
		return cartridge_ram_buffer[0x2000*(ram_selector) + addr - 0xA000];
	else 						//demande de lecture dans le reste de la mémoire interne
		return internal_ram[addr - 0xA000];
}


void memory_write(short addr, BYTE data){
	if((addr >= 0 && addr <= 0x7FFF) || (addr >= 0xA000 && addr <= 0xBFFF)){
		if(cartridge_type >= 0x01 || cartridge_type <= 0x03){//MBC1
			write_mbc1(addr, data);		
		}
	}
	else if(addr >= 0x8000 && addr <= 0x9FFF){
		internal_ram[addr - 0x8000] = data;	
	}
	else
		internal_ram[addr - 0xA000] = data;

}

void write_mbc1(short addr, BYTE data){
	if(addr >= 0 && addr <= 0x1FFF){
		if((data & 0x0A) == 0x0A) enable_ram = 1;
		else enable_ram = 0;
	}
	else if(addr >= 0x2000 && addr <= 0x3FFF){
		rom_selector = data & 0x1F;
		if(rom_selector == 0 || rom_selector == 0x20 || rom_selector == 0x40 || rom_selector == 0x60) rom_selector++;
	}
	else if(addr >= 0x4000 && addr <= 0x5FFF){
		ram_selector = (data & 0x60) >> 5; //sélection ram
	}
	else if(addr >= 0x6000 && addr <= 0x7FFF){
		if(data == 0) rom_mode = 1; //rom mode enabled
		else rom_mode = 0; // ram mode enabled
	}
	else
		cartridge_ram_buffer[0x2000*(ram_selector) + addr - 0xA000] = data;
}


