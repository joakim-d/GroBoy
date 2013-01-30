#include "cpu.h"

z80_t z80;
void init(){
	z80.PC = 0;
	z80.SP = 0;
	z80.A = 0;
	z80.F = 0;
	z80.B = 0;
	z80.C = 0;
	z80.D = 0;
	z80.E = 0;
	z80.H = 0;
	z80.L = 0;
	z80.A2 = 0;
	z80.F2 = 0;
	z80.B2 = 0;
	z80.C2 = 0;
	z80.D2 = 0;
	z80.E2 = 0;
	z80.H2 = 0;
	z80.L2 = 0;
	z80.I = 0;
	z80.R = 0;
}

void read_rom_info(char* rom_path){
	int file_d;
	struct stat file_stat;
	char buffer[1024];

	file_d = open(rom_path, O_RDONLY);
	fstat(file_d, &file_stat);
	
	rom_buffer = (char *) malloc(file_stat.st_size);
	
	read(file_d, rom_buffer, file_stat.st_size);
	close(file_d);

	if(memcmp(rom_buffer + 0x0100, "0x00C3", 2) != 0){ //On vérifie si la rom est bien un fichier gb
		printf("Error reading rom magic code\n");
		exit(-1);
	}
	memcpy(&z80.PC, rom_buffer + 0x0102, 2); //On stocke le début du programme dans le registre PC
	
	printf("Game title: %s\n", rom_buffer + 0x0134);
	printf("Cartridge type: ");	
	switch(*(rom_buffer + 0x0147)){
		case 0: printf("ROM ONLY");break;
		case 1: printf("ROM+MBC1");break;
		case 2: printf("ROM+MBC1+RAM");break;
		case 3: printf("ROM+MBC1+RAM+BATTERY");break;
		case 4: printf("ROM+MBC2");break;
		case 5: printf("ROM+MBC2+BATTERY");break;
		default: printf("UNKNOWN");break;
	}
	printf("\nRom Size ");
	switch(*(rom_buffer + 0x0148)){
		case 0: printf("32KB: 2 Banks");break;
		case 1: printf("64KB: 4 Banks");break;
		case 2: printf("128KB: 8 Banks");break;
		case 3: printf("256KB: 16 Banks");break;
		case 4: printf("512KB: 32 Banks");break;
	}
	printf("\nRam Size ");
	switch(*(rom_buffer + 0x0149)){
		case 0: printf("None");break;
		case 1: printf("2KB: 1 Bank");break;
		case 2: printf("8KB: 1 Bank");break;
		case 3: printf("32KB: 4 Banks");break;
	}
	printf("\nLanguage ");
	switch(*(rom_buffer + 0x014A)){
		case 0: printf("Japanese Game");break;
		case 1: printf("English Game");break;
	}
	printf("\nManufacturer");
	switch(*(rom_buffer + 0x014B)){
		case 0x33: printf("Nintendo or extended");break;
		case 0x79: printf("Accolade");break;
		case (char)0xA4: printf("Konami");break;
	}
	printf("\nVersion number %d\n", rom_buffer + 0x014C);
}

void run(){
	/*
	int interrupt_period;
	int counter;

	counter=interrupt_period;

	for(;;)
	{
		OpCode=Memory[PC++];
		Counter-=Cycles[OpCode];

		switch(OpCode)
		{
			case OpCode1:
			case OpCode2:
				...
		}

		if(Counter<=0)
		{
			// Check for interrupts and do other
			// cyclic tasks here                
			...
				Counter+=InterruptPeriod;
			if(ExitRequired) break;
		}
	}
	*/
}
