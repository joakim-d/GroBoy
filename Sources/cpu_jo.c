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
	BYTE hl, d8, F, low_nibble, high_nibble;
	BYTE_S r8;
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
			z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);
			z80.SP += 2;
			break;

			case 0xCA: //JP Z,a16
			if(z80.F & 0x80 != 0)
				z80.PC = (memory_read(z80.PC) << 8) + memory_read(z80.PC + 1); 
			else z80.PC += 2;
			break;
			
			case 0xCB: //Two bytes Opcode
				op_code = rom_buffer[z80.PC++];
				switch(op_code){
					case 0x00: //RLC B
					if (z80.B & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B <<= 1;
					if(z80.F & 0x10) z80.B |= 1;
					if (z80.B == 0) z80.F |= 0x80;
					break;

					case 0x01: //RLC C
					if (z80.C & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.C <<= 1;
					if(z80.F & 0x10) z80.C |= 1;
					if (z80.C == 0) z80.F |= 0x80;
					break;

					case 0x02: //RLC D
					if (z80.D & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.D <<= 1;
					if(z80.F & 0x10) z80.D |= 1;
					if (z80.D == 0) z80.F |= 0x80;
					break;

					case 0x03: //RLC E
					if (z80.E & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.E <<= 1;
					if(z80.F & 0x10) z80.E |= 1;
					if (z80.E == 0) z80.F |= 0x80;
					break;

					case 0x04: //RLC H
					if (z80.H & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.H <<= 1;
					if(z80.F & 0x10) z80.H |= 1;
					if (z80.H == 0) z80.F |= 0x80;
					break;

					case 0x05: //RLC L
					if (z80.L & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.L <<= 1;
					if(z80.F & 0x10) z80.L |= 1;
					if (z80.L == 0) z80.F |= 0x80;
					break;

					case 0x06: //RLC (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if (hl & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					hl <<= 1;
					if(z80.F & 0x10) hl |= 1;
					if (hl == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x07: //RLC A
					if (z80.A & 0x80 == 0) z80.F = 0x00;
					else z80.F = 0x10;
					z80.A <<= 1;
					if(z80.F & 0x10) z80.A |= 1;
					if (z80.A == 0) z80.F |= 0x80;
					break;

					case 0x08: //RRC B
					if (z80.B & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B >>= 1;
					if(z80.F & 0x10) z80.B |= 0x80;
					if (z80.B == 0) z80.F |= 0x80;
					break;

					case 0x09: //RRC C
					if (z80.C & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C >>= 1;
					if(z80.F & 0x10) z80.C |= 0x80;
					if (z80.C == 0) z80.F |= 0x80;
					break;
					
					case 0x0A: //RRC D
					if (z80.D & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D >>= 1;
					if(z80.F & 0x10) z80.D |= 0x80;
					if (z80.D == 0) z80.F |= 0x80;
					break;

					case 0x0B: //RRC E
					if (z80.E & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E >>= 1;
					if(z80.F & 0x10) z80.E |= 0x80;
					if (z80.E == 0) z80.F |= 0x80;
					break;

					case 0x0C: //RRC H
					if (z80.H & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H >>= 1;
					if(z80.F & 0x10) z80.H |= 0x80;
					if (z80.H == 0) z80.F |= 0x80;
					break;

					case 0x0D: //RRC L
					if (z80.L & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L >>= 1;
					if(z80.F & 0x10) z80.L |= 0x80;
					if (z80.M == 0) z80.F |= 0x80;
					break;

					case 0x0E: //RRC (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if (hl & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					hl >>= 1;
					if(z80.F & 0x10) hl |= 0x80;
					if (hl == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x0F: //RRC A
					if (z80.A & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A >>= 1;
					if(z80.F & 0x10) z80.A |= 0x80;
					if (z80.A == 0) z80.F |= 0x80;
					break;

					case 0x10: //RL B
					F = z80.F;
					if (z80.B & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B <<= 1;
					if(F & 0x10) z80.B |= 1;
					if (z80.B == 0) z80.F |= 0x80;
					break;

					case 0x11: //RL C
					F = z80.F;
					if (z80.C & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C <<= 1;
					if(F & 0x10) z80.C |= 1;
					if (z80.C == 0) z80.F |= 0x80;
					break;

					case 0x12: //RL D
					F = z80.F;
					if (z80.D & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D <<= 1;
					if(F & 0x10) z80.D |= 1;
					if (z80.D == 0) z80.F |= 0x80;
					break;

					case 0x13: //RL E
					F = z80.F;
					if (z80.E & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E <<= 1;
					if(F & 0x10) z80.E |= 1;
					if (z80.E == 0) z80.F |= 0x80;
					break;

					case 0x14: //RL H
					F = z80.F;
					if (z80.H & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H <<= 1;
					if(F & 0x10) z80.H |= 1;
					if (z80.H == 0) z80.F |= 0x80;
					break;

					case 0x15: //RL L
					F = z80.F;
					if (z80.L & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L <<= 1;
					if(F & 0x10) z80.L |= 1;
					if (z80.L == 0) z80.F |= 0x80;
					break;

					case 0x16: //RL (HL)
					hl = memory_read(z80.PC);
					hl = memory_read((z80.H << 8) + z80.L);
					F = z80.F;
					if (hl & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					hl <<= 1;
					if(F & 0x10) hl |= 1;
					if (hl == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x17: //RL A
					F = z80.F;
					if (z80.A & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A <<= 1;
					if(F & 0x10) z80.A |= 1;
					if (z80.A == 0) z80.F |= 0x80;
					break;

					case 0x18: //RR B
					F = z80.F;
					if (z80.B & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B >>= 1;
					if(F & 0x10) z80.B |= 0x80;
					if (z80.B == 0) z80.F |= 0x80;
					break;

					case 0x19: //RR C
					F = z80.F;
					if (z80.C & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C >>= 1;
					if(F & 0x10) z80.C |= 0x80;
					if (z80.C == 0) z80.F |= 0x80;
					break;

					case 0x1A: //RR D
					F = z80.F;
					if (z80.D & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D >>= 1;
					if(F & 0x10) z80.D |= 0x80;
					if (z80.D == 0) z80.F |= 0x80;
					break;

					case 0x1B: //RR E
					F = z80.F;
					if (z80.E & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E >>= 1;
					if(F & 0x10) z80.E |= 0x80;
					if (z80.E == 0) z80.F |= 0x80;
					break;

					case 0x1C: //RR H
					F = z80.F;
					if (z80.H & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H >>= 1;
					if(F & 0x10) z80.H |= 0x80;
					if (z80.H == 0) z80.F |= 0x80;
					break;

					case 0x1D: //RR L
					F = z80.F;
					if (z80.L & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L >>= 1;
					if(F & 0x10) z80.L |= 0x80;
					if (z80.L == 0) z80.F |= 0x80;
					break;

					case 0x1E: //RR (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					F = z80.F;
					if (hl & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					hl >>= 1;
					if(F & 0x10) hl |= 0x80;
					if (z80.A == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x1F: //RR A
					F = z80.F;
					if (z80.A & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A >>= 1;
					if(F & 0x10) z80.A |= 0x80;
					if (z80.A == 0) z80.F |= 0x80;
					break;

					case 0x20: //SLA B
					if (z80.B & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B <<= 1;
					if (z80.B == 0) z80.F |= 0x80;
					break;

					case 0x21: //SLA C
					if (z80.C & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C <<= 1;
					if (z80.C == 0) z80.F |= 0x80;
					break;

					case 0x22: //SLA D
					if (z80.D & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D <<= 1;
					if (z80.D == 0) z80.F |= 0x80;
					break;

					case 0x23: //SLA E
					if (z80.E & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E <<= 1;
					if (z80.E == 0) z80.F |= 0x80;
					break;

					case 0x24: //SLA H
					if (z80.H & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H <<= 1;
					if (z80.H == 0) z80.F |= 0x80;
					break;

					case 0x25: //SLA L
					if (z80.L & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L <<= 1;
					if (z80.L == 0) z80.F |= 0x80;
					break;

					case 0x26: //SLA (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if (z80.B & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B <<= 1;
					if (z80.B == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x27: //SLA A
					if (z80.A & 0x80 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A <<= 1;
					if (z80.A == 0) z80.F |= 0x80;
					break;
					
					case 0x28: //SRA B
					F = z80.B & 0x80;
					if(z80.B & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B >>= 1;
					if (F) z80.B |= 0x80;
					if (z80.B == 0) z80.F |= 0x80;
					break;
					
					case 0x29: //SRA C
					F = z80.C & 0x80;
					if(z80.C & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C >>= 1;
					if (F) z80.C |= 0x80;
					if (z80.C == 0) z80.F |= 0x80;
					break;
					
					case 0x2A: //SRA D
					F = z80.D & 0x80;
					if(z80.D & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D >>= 1;
					if (F) z80.D |= 0x80;
					if (z80.D == 0) z80.F |= 0x80;
					break;
					
					case 0x2B: //SRA E
					F = z80.E & 0x80;
					if(z80.E & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E >>= 1;
					if (F) z80.E |= 0x80;
					if (z80.E == 0) z80.F |= 0x80;
					break;
					
					case 0x2C: //SRA H
					F = z80.H & 0x80;
					if(z80.H & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H >>= 1;
					if (F) z80.H |= 0x80;
					if (z80.H == 0) z80.F |= 0x80;
					break;
					
					case 0x2D: //SRA L
					F = z80.L & 0x80;
					if(z80.L & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L >>= 1;
					if (F) z80.L |= 0x80;
					if (z80.L == 0) z80.F |= 0x80;
					break;
					
					case 0x2E: //SRA (HL)
					hl = memory_read((z80.H << 8) +z80.L);
					F = hl & 0x80;
					if(hl & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					hl >>= 1;
					if (F) hl |= 0x80;
					if (hl == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L, hl);
					break;
					
					case 0x2F: //SRA A
					F = z80.A & 0x80;
					if(z80.A & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A >>= 1;
					if (F) z80.A |= 0x80;
					if (z80.A == 0) z80.F |= 0x80;
					break;
					
					case 0x30: //SWAP B
					low_nibble = (z80.B & 0xF0) >> 4;
					high_nibble = (z80.B & 0x0F) << 4;
					z80.B = high_nibble + low_nibble;
					if(z80.B == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x31: //SWAP C
					low_nibble = (z80.C & 0xF0) >> 4;
					high_nibble = (z80.C & 0x0F) << 4;
					z80.C = high_nibble + low_nibble;
					if(z80.C == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x32: //SWAP D
					low_nibble = (z80.D & 0xF0) >> 4;
					high_nibble = (z80.D & 0x0F) << 4;
					z80.D = high_nibble + low_nibble;
					if(z80.D == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x33: //SWAP E
					low_nibble = (z80.E & 0xF0) >> 4;
					high_nibble = (z80.E & 0x0F) << 4;
					z80.E = high_nibble + low_nibble;
					if(z80.E == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x34: //SWAP H
					low_nibble = (z80.H & 0xF0) >> 4;
					high_nibble = (z80.H & 0x0F) << 4;
					z80.H = high_nibble + low_nibble;
					if(z80.H == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x35: //SWAP L
					low_nibble = (z80.L & 0xF0) >> 4;
					high_nibble = (z80.L & 0x0F) << 4;
					z80.L = high_nibble + low_nibble;
					if(z80.L == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x36: //SWAP (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					low_nibble = (z80.B & 0xF0) >> 4;
					high_nibble = (z80.B & 0x0F) << 4;
					z80.B = high_nibble + low_nibble;
					if(z80.B == 0) z80.F = 0x80;
					else z80.F = 0;
					memory_write((z80.H << 8) + z80.L, hl);
					break;

					case 0x37: //SWAP A
					low_nibble = (z80.A & 0xF0) >> 4;
					high_nibble = (z80.A & 0x0F) << 4;
					z80.A = high_nibble + low_nibble;
					if(z80.A == 0) z80.F = 0x80;
					else z80.F = 0;
					break;

					case 0x38: //SRL B
					if(z80.B & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.B >>= 1;
					if (z80.B == 0) z80.F |= 0x80;
					break;
					
					case 0x39: //SRL C
					if(z80.C & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.C >>= 1;
					if (z80.C == 0) z80.F |= 0x80;
					break;
					
					case 0x3A: //SRL D
					if(z80.D & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.D >>= 1;
					if (z80.D == 0) z80.F |= 0x80;
					break;
					
					case 0x3B: //SRL E
					if(z80.E & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.E >>= 1;
					if (z80.E == 0) z80.F |= 0x80;
					break;
					
					case 0x3C: //SRL H
					if(z80.H & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.H >>= 1;
					if (z80.H == 0) z80.F |= 0x80;
					break;
					
					case 0x3D: //SRL L
					if(z80.L & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.L >>= 1;
					if (z80.L == 0) z80.F |= 0x80;
					break;
					
					case 0x3E: //SRL (HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if(hl & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					hl >>= 1;
					if (hl == 0) z80.F |= 0x80;
					memory_write((z80.H << 8) + z80.L);
					break;
					
					case 0x3F: //SRL A
					if(z80.A & 0x01 == 0) z80.F = 0;
					else z80.F = 0x10;
					z80.A >>= 1;
					if (z80.A == 0) z80.F |= 0x80;
					break;
					
					case 0x40: //BIT 0,B
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.B & 0x01) z80.F |= 0x80;
					break;

					case 0x41: //BIT 0,C
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.C & 0x01) z80.F |= 0x80;
					break;

					case 0x42: //BIT 0,D
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.D & 0x01) z80.F |= 0x80;
					break;

					case 0x43: //BIT 0,E
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.E & 0x01) z80.F |= 0x80;
					break;

					case 0x44: //BIT 0,H
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.H & 0x01) z80.F |= 0x80;
					break;

					case 0x45: //BIT 0,L
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.L & 0x01) z80.F |= 0x80;
					break;

					case 0x46: //BIT 0,(HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.hl & 0x01) z80.F |= 0x80;
					break;

					case 0x47: //BIT 0,A
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.A & 0x01) z80.F |= 0x80;
					break;

					case 0x48: //BIT 1,B
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.B & 0x02) z80.F |= 0x80;
					break;

					case 0x49: //BIT 1,C
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.C & 0x02) z80.F |= 0x80;
					break;

					case 0x4A: //BIT 1,D
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.D & 0x02) z80.F |= 0x80;
					break;

					case 0x4B: //BIT 1,E
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.E & 0x02) z80.F |= 0x80;
					break;

					case 0x4C: //BIT 1,H
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.H & 0x02) z80.F |= 0x80;
					break;

					case 0x4D: //BIT 1,L
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.L & 0x02) z80.F |= 0x80;
					break;

					case 0x4E: //BIT 1,(HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.hl & 0x02) z80.F |= 0x80;
					break;

					case 0x4F: //BIT 1,A
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.A & 0x02) z80.F |= 0x80;
					break;

					case 0x50: //BIT 2,B
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.B & 0x04) z80.F |= 0x80;
					break;

					case 0x51: //BIT 2,C
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.C & 0x04) z80.F |= 0x80;
					break;

					case 0x52: //BIT 2,D
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.D & 0x04) z80.F |= 0x80;
					break;

					case 0x53: //BIT 2,E
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.E & 0x04) z80.F |= 0x80;
					break;

					case 0x54: //BIT 2,H
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.H & 0x04) z80.F |= 0x80;
					break;

					case 0x55: //BIT 2,L
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.L & 0x04) z80.F |= 0x80;
					break;

					case 0x56: //BIT 2,(HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.hl & 0x04) z80.F |= 0x80;
					break;

					case 0x57: //BIT 2,A
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.A & 0x04) z80.F |= 0x80;
					break;

					case 0x58: //BIT 3,B
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.B & 0x08) z80.F |= 0x80;
					break;

					case 0x59: //BIT 3,C
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.C & 0x08) z80.F |= 0x80;
					break;

					case 0x5A: //BIT 3,D
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.D & 0x08) z80.F |= 0x80;
					break;

					case 0x5B: //BIT 3,E
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.E & 0x08) z80.F |= 0x80;
					break;

					case 0x5C: //BIT 3,H
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.H & 0x08) z80.F |= 0x80;
					break;

					case 0x5D: //BIT 3,L
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.L & 0x08) z80.F |= 0x80;
					break;

					case 0x5E: //BIT 3,(HL)
					hl = memory_read((z80.H << 8) + z80.L);
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.hl & 0x08) z80.F |= 0x80;
					break;

					case 0x5F: //BIT 3,A
					if(z80.F & 0x10) z80.F = 0x30;
					else z80.F = 0x20;
					if(z80.A & 0x08) z80.F |= 0x80;
					break;

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
			
			case 0xE2: //LD (C), A
			memory_write(0xFF00 + z80.C, z80.A);
			break;

			case 0xE5: //PUSH HL
			memory_write(z80.SP - 1, z80.H);
			memory_write(z80.SP - 2, z80.L);
			z80.SP -= 2;
			break;

			case 0xE6: //AND d8
			z80.F &= 0x20;
			z80.A &= memory_read(z80.PC);
			if (z80.A == 0) z80.F |= 0x80;
			z80.PC++;
			break;

			case 0xE7: //RST 20H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x20;
			z80.SP -= 2;
			break;

			case 0xE8: //ADD SP,r8
			r8 = memory_read(z80.PC);
			Z80.SP += r8;
			//FLAGS
			break;

			case 0xE9: //JP (HL)
			z80.PC = memory_read(z80.H << 8 + z80.L);
			break;

			case 0xEA: //LD (a16),A
			memory_write(memory_read(z80.PC) << 8 + memory_read(z80.PC + 1), z80.A);
			z80.PC += 2;
			break;

			case 0xEE: //XOR d8
			z80.A ^= memory_read(z80.PC);
			if(z80.A == 0) z80.F &= 0x80;
			else z80.F = 0;
			z80.PC++;
			break;
			
			case 0xEF: //RST 28H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x28;
			z80.SP -= 2;
			break;

			case 0xF0: //LDH A,(a8)
			z80.A = memory_read(0xFF00 + memory_read(z80.PC));
			z80.PC++;	
			break;

			case 0xF1: //POP AF
			z80.A = memory_read(z80.SP);
			z80.F = memory_read(z80.SP + 1);
			z80.SP += 2;
			break;

			case 0xF2: //LD A,(C)
			z80.A = memory_read(0xFF00 + z80.C);
			break;

			case 0xF3: //DI

			break;

			case 0xF5: //PUSH AF
			memory_write(z80.SP - 1, z80.A);
			memory_write(z80.SP - 2, z80.F);
			z80.SP -= 2;
			break;
			
			case 0xF6: //OR d8
			d8 = memory_read(z80.PC);
			z80.A |= d8;
			if (z80.A == 0) z80.F |= 0x80;
			else z80.F = 0;
			z80.PC++;
			break;

			case 0xF7: //RST 30H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x30;
			z80.SP -= 2;
			break;

			case 0xF8: //LD HL,SP+r8
			r8 = memory_read(z80.PC);
			Z80.H = ((Z80.SP + r8) & 0xFF00) >> 8 ;
			Z80.L = ((Z80.SP + r8) & 0xFF);
			z80.PC++;
			//FLAGS
			break;

			case 0xF9: //LD SP,HL
			z80.SP = Z80.H << 8 + z80.L;
			break;

			case 0xFA: //LD A,(a16)
			z80.A = memory_read(memory_read(z80.PC) << 8 + memory_read(Z80.PC + 1));
			z80.PC += 2;
			break;

			case 0xFB: //EI

			break;

			case 0xFE: //CP d8
			d8 = memory_read(z80.PC);
			z80.F = 0x40;
			if (z80.A - d8 == 0) z80.F |= 0x80;
			if (z80.A > 0xF && z80.A - d8 <= 0xF) z80.F |= 0x20;
			if (z80.A - d8 < 0) z80.E |= 0x10;//C 0x10
			z80.PC++;
			break;
						
			case 0xFF: //RST 38H
			memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
			memory_write(z80.SP - 2, z80.PC & 0xFF);
			z80.PC = 0x38;
			z80.SP -= 2;
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
