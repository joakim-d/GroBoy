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

	if(memcmp(rom_buffer + 0x0100, "\x00\xC3", 2) != 0){ //On vérifie si la rom est bien un fichier gb
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
		case 0xA4: printf("Konami");break;
	}
	printf("\nVersion number %c\n", rom_buffer[0x014C]);
}

void run(){
	int interrupt_period;
	int counter, c;
	BYTE hl, d8;
	counter=interrupt_period;

	unsigned char op_code;
	for(;;)
	{
		op_code=rom_buffer[z80.PC++];
		//counter-=cycles[op_code];

		switch(op_code){
			//Maxi table d'instructions
			
			case 0xB0: //OR B
			z80.A |= z80.B;
			if (z80.A == 0)	z80.F &= 0x80;
			else z80.F &= 0x00;
			break;

			case 0xB1: //OR C
			z80.A |= z80.C;
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB2: //OR D
			z80.A |= z80.D;
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;

			case 0xB3: //OR E
			z80.A |= z80.E;
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB4: //OR H
			z80.A |= z80.H;
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB5: //OR L
			z80.A |= z80.L;
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB6: //OR (HL)
			z80.A |= memory_read((z80.H << 8) + (z80.L));
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB7: //OR A
			if (z80.A == 0) z80.F &= 0x80;
			else z80.F &= 0x00;
			break;
			
			case 0xB8: //CP B
			z80.F = 0x40;
			if (z80.A - z80.B == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.B <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.B < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xB9: //CP C
			z80.F = 0x40;
			if (z80.A - z80.C == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.C <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.C < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xBA: //CP D
			z80.F = 0x40;
			if (z80.A - z80.D == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.D <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.D < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xBB: //CP E
			z80.F = 0x40;
			if (z80.A - z80.E == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.E <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.E < 0) z80.E |= 0x10;//C 0x10
			break;
			
			case 0xBC: //CP H
			z80.F = 0x40;
			if (z80.A - z80.H == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.H <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.H < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xBD: //CP L
			z80.F = 0x40;
			if (z80.A - z80.L == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - z80.L <= 0xF) z80.F |= 0x20;
			if (z80.A - z80.L < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xBE: //CP (HL)
			hl = memory_read(z80.H << 8 + z80.L);
			z80.F = 0x40;
			if (z80.A - hl == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - hl <= 0xF) z80.F |= 0x20;
			if (z80.A - hl < 0) z80.F |= 0x10;//C 0x10
			break;
			
			case 0xBF: //CP A
			z80.F = 0xC0; //Flag Z, N (A - A = 0)
			if (z80.A > 0xF) z80.F |= 0x20;
			break;

			case 0xC0: //RET NZ
			if(z80.F & 0x80 == 0){
				z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);;
				z80.SP += 2;
			}
			break;

			case 0xC1: //POP BC
			z80.C = memory_read(z80.SP);
			z80.B = memory_read(z80.SP + 1);
			z80.SP += 2;
			break;

			case 0xC2: //JP NZ,a16
			if(z80.F & 0x80 == 0)
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
			else z80.PC += 2;
			break;

			case 0xC3: //JP a16
			z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			break;

			case 0xC4: //CALL NZ,a16
			if(z80.F & 0x80 == 0){
				memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
				memory_write(z80.SP - 2, (z80.PC + 2) & 0xFF);
				z80.SP -= 2;
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			}
			else z80.PC += 2;
			break;
			
			case 0xC5: //PUSH BC
			memory_write(z80.SP - 1, z80.B);
			memory_write(z80.SP - 2, z80.C);
			z80.SP -= 2;
			break;

			case 0xC6: //ADD A,d8
			d8 = memory_read(z80.PC);
			z80.F = 0;
			if ((z80.A + d8) % 256 == 0) z80.F |= 0x80;
			if (z80.A + d8 > 0xFF) z80.F |= 0x10;
			if(z80.A <= 0xF && z80.A + d8 > 0xF) z80.F |= 0x20;
			z80.A = (z80.A + d8) % 256;
			
			z80.PC++;
			break;

			case 0xC7: //RST 00H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0;
			z80.SP -= 2;
			break;
			
			case 0xC8: //RET Z
			if(z80.F & 0x80 != 0){
				z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
				z80.SP += 2;
			}
			break;
			
			case 0xC9: //RET 
			z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
			z80.SP += 2;
			break;

			case 0xCA: //JP Z,a16
			if(z80.F & 0x80 != 0)
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			else z80.PC += 2;
			break;
			
			case 0xCB: //Two bytes Opcode
				op_code = rom_buffer[z80.PC++];
				switch(op_code){

				}
			break;

			case 0xCC: //CALL Z,a16
			if(z80.F & 0x80 != 0){
				memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
				memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
				z80.SP -= 2;
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			}
			else z80.PC += 2;
			break;

			case 0xCD: //CALL a16
			memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
			memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
			z80.SP -= 2;
			z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			break;

			case 0xCE: //ADC A,d8
			
			if (z80.F & 0x10) c = 1;
			else c = 0;
			
			d8 = memory_read(z80.PC);
			if ((z80.A + d8 + c) % 256 == 0) z80.F |= 0x80;
			if (z80.A + d8 + c > 0xFF) z80.F |= 0x10;
			if(z80.A <= 0xF && z80.A + d8 + c > 0xF) z80.F |= 0x20;
			z80.A = (z80.A + d8 + c) % 256;
			z80.PC++;
			break;
			
			case 0xCF: //RST 08H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x08;
			z80.SP -= 2;
			break;
			
			case 0xD0: //RET NC 
			if(z80.F & 0x10 == 0){
				z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);;
				z80.SP += 2;
			}
			break;
				
			case 0xD1: //POP DE
			z80.D = memory_read(z80.SP);
			z80.E = memory_read(z80.SP + 1);
			z80.SP += 2;
			break;

			case 0xD2: //JP NC,a16
			if(z80.F & 0x10 == 0)
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
			else z80.PC += 2;
			break;

			case 0xD4: //CALL NC,a16
			if(z80.F & 0x10 == 0){
				memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
				memory_write(z80.SP - 2, (z80.PC + 2) & 0xFF);
				z80.SP -= 2;
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			}
			else z80.PC += 2;
			break;
			
			case 0xD5: //PUSH DE
			memory_write(z80.SP - 1, z80.D);
			memory_write(z80.SP - 2, z80.E);
			z80.SP -= 2;
			break;

			case 0xD6: //SUB d8
			z80.F |= 0x40;
			d8 = memory_read(z80.PC);
			if (z80.A - d8 == 0) z80.F |= 0x80;
			if (z80.A - d8 < 0) z80.F |= 0x10;
			if(z80.A >= 0xF && z80.A - d8 < 0xF) z80.F |= 0x20;
			break;
			
			case 0xD7: //RST 10H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x10;
			z80.SP -= 2;
			break;
			
			case 0xD8: //RET C
			if(z80.F & 0x10 != 0){
				z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
				z80.SP += 2;
			}
			break;
			
			case 0xD9: //RETI
			//PLUS FLAG IF
			z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
			z80.SP += 2;
			break;

			case 0xDA: //JP C,a16
			if(z80.F & 0x10 != 0)
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			else z80.PC += 2;
			break;
			
			case 0xDC: //CALL C,a16
			if(z80.F & 0x10 != 0){
				memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
				memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
				z80.SP -= 2;
				z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
			}
			else z80.PC += 2;
			break;

			case 0xDE: //SBC A,d8	
			if (z80.F & 0x10) c = 1;
			else c = 0;
			
			d8 = memory_read(z80.PC);
			if (z80.A + d8 + c == 0) z80.F |= 0x80;
			if (z80.A + d8 + c > 0xFF) z80.F |= 0x10;
			if(z80.A > 0xF && z80.A + d8 + c <= 0xF) z80.F |= 0x20;
			z80.A = (z80.A + d8 + c);
			z80.PC++;
			break;
			
			case 0xDF: //RST 18H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x18;
			z80.SP -= 2;
			break;
			
			case 0xE0: //LDH (a8),A
			memory_write(0xFF00 + memory_read(z80.PC), z80.A);
			z80.PC++;
			break;

			case 0xE1: //POP HL
			z80.H = memory_read(z80.SP);
			z80.L = memory_read(z80.SP + 1);
			z80.SP += 2;
			break;
			
			
		}

		/*if(Counter<=0)
		  {
		// Check for interrupts and do other
		// cyclic tasks here                
		...
		Counter+=InterruptPeriod;
		if(ExitRequired) break;
		}*/
	}
}
