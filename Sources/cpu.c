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
	
	rom_buffer = (BYTE *) malloc(file_stat.st_size);
	
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
	printf("\nVersion number %d\n", rom_buffer[0x014C]);
}

void run(){
	int interrupt_period;
	int counter;
	counter=interrupt_period;

	char op_code;
	for(;;)
	{
		op_code=memory_read(z80.PC++);
		//counter-=cycles[op_code];

		switch(op_code){
			//Maxi table d'instructions
			case 0xB0: //OR B
				or(z80.B);
				break;
			case 0xB1: //OR C
				or(z80.C);
				break;
			case 0xB2: //OR D
				or(z80.D);
				break;
			case 0xB3: //OR E
				or(z80.E);
				break;
			case 0xB4: //OR H
				or(z80.H);
				break;
			case 0xB5: //OR L
				or(z80.L);
				break;
			case 0xB6: //OR (HL)
				or(memory_read((z80.H << 8) + z80.L));
				break;
			case 0xB7: //OR A
				or(z80.A);
				break;
			case 0xB8: //CP B
				cp(z80.B);
				break;
			case 0xB9: //CP C
				cp(z80.C);
				break;
			case 0xBA: //CP D
				cp(z80.D);
				break;
			case 0xBB: //CP E
				cp(z80.E);
				break;
			case 0xBC: //CP H
				cp(z80.H);
				break;
			case 0xBD: //CP L
				cp(z80.L);
				break;
			case 0xBE: //CP (HL)
				cp(memory_read((z80.H << 8) + z80.L));
				break;
			case 0xBF: //CP A
				cp(z80.A);
				break;
			case 0xC0: //RET NZ
				ret_n_cond(0x80);
				break;
			case 0xC1: //POP BC
				pop(&z80.B, &z80.C);
				break;
			case 0xC2: //JP NZ,a16
				jp_n_cond(0x80);
				break;
			case 0xC3: //JP a16
				jp((memory_read(z80.PC) << 8) + memory_read(++z80.PC));
				break;
			case 0xC4: //CALL NZ,a16
				call_n_cond(0x80);
				break;
			case 0xC5: //PUSH BC
				push(z80.B, z80.C);
				break;
			case 0xC6: //ADD A,d8
				add_d8();
				break;
			case 0xC7: //RST 00H
				rst(0);
				break;
			case 0xC8: //RET Z
				ret_cond(0x80);
				break;
			case 0xC9: //RET 
				ret();
				break;
			case 0xCA: //JP Z,a16
				jp_cond(0x80);
				break;
			case 0xCB: //Two bytes Opcode
				op_code = rom_buffer[z80.PC++];
				switch(op_code){
					case 0x00: //RLC B
						rlc(&z80.B);
						break;
					case 0x01: //RLC C
						rlc(&z80.C);
						break;
					case 0x02: //RLC D
						rlc(&z80.D);
						break;
					case 0x03: //RLC E
						rlc(&z80.E);
						break;
					case 0x04: //RLC H
						rlc(&z80.H);
						break;
					case 0x05: //RLC L
						rlc(&z80.L);
						break;
					case 0x06: //RLC (HL)
						rlc_hl();
						break;
					case 0x07: //RLC A
						rlc(&z80.A);
						break;
					case 0x08: //RRC B
						rrc(&z80.B);
						break;
					case 0x09: //RRC C
						rrc(&z80.C);
						break;
					case 0x0A: //RRC D
						rrc(&z80.D);
						break;
					case 0x0B: //RRC E
						rrc(&z80.E);
						break;
					case 0x0C: //RRC H
						rrc(&z80.H);
						break;
					case 0x0D: //RRC L
						rrc(&z80.L);
						break;
					case 0x0E: //RRC (HL)
						rrc_hl();
						break;
					case 0x0F: //RRC A
						rrc(&z80.A);
						break;
					case 0x10: //RL B
						rl(&z80.B);
						break;
					case 0x11: //RL C
						rl(&z80.C);
						break;
					case 0x12: //RL D
						rl(&z80.D);
						break;
					case 0x13: //RL E
						rl(&z80.E);
						break;
					case 0x14: //RL H
						rl(&z80.H);
						break;
					case 0x15: //RL L
						rl(&z80.L);
						break;
					case 0x16: //RL (HL)
						rl_hl();
						break;
					case 0x17: //RL A
						rl(&z80.A);
						break;
					case 0x18: //RR B
						rr(&z80.B);
						break;
					case 0x19: //RR C
						rr(&z80.C);
						break;
					case 0x1A: //RR D
						rr(&z80.D);
						break;
					case 0x1B: //RR E
						rr(&z80.E);
						break;
					case 0x1C: //RR H
						rr(&z80.H);
						break;
					case 0x1D: //RR L
						rr(&z80.L);
						break;
					case 0x1E: //RR (HL)
						rr_hl();
						break;
					case 0x1F: //RR A
						rr(&z80.A);
						break;
					case 0x20: //SLA B
						sla(&z80.B);
						break;
					case 0x21: //SLA C
						sla(&z80.C);
						break;
					case 0x22: //SLA D
						sla(&z80.D);
						break;
					case 0x23: //SLA E
						sla(&z80.E);
						break;
					case 0x24: //SLA H
						sla(&z80.H);
						break;
					case 0x25: //SLA L
						sla(&z80.L);
						break;
					case 0x26: //SLA (HL)
						sla_hl();
						break;
					case 0x27: //SLA A
						sla(&z80.A);
						break;
					case 0x28: //SRA B
						sra(&z80.B);
						break;
					case 0x29: //SRA C
						sra(&z80.C);
						break;
					case 0x2A: //SRA D
						sra(&z80.D);
						break;
					case 0x2B: //SRA E
						sra(&z80.E);
						break;
					case 0x2C: //SRA H
						sra(&z80.H);
						break;
					case 0x2D: //SRA L
						sra(&z80.L);
						break;
					case 0x2E: //SRA (HL)
						sra_hl();
						break;
					case 0x2F: //SRA A
						sra(&z80.A);
						break;
					case 0x30: //SWAP B
						swap(&z80.B);
						break;
					case 0x31: //SWAP C
						swap(&z80.C);
						break;
					case 0x32: //SWAP D
						swap(&z80.D);
						break;
					case 0x33: //SWAP E
						swap(&z80.E);
						break;
					case 0x34: //SWAP H
						swap(&z80.H);
						break;
					case 0x35: //SWAP L
						swap(&z80.L);
						break;
					case 0x36: //SWAP (HL)
						swap_hl();
						break;
					case 0x37: //SWAP A
						swap(&z80.A);
						break;
					case 0x38: //SRL B
						srl(&z80.B);
						break;
					case 0x39: //SRL C
						srl(&z80.C);
						break;
					case 0x3A: //SRL D
						srl(&z80.D);
						break;
					case 0x3B: //SRL E
						srl(&z80.E);
						break;
					case 0x3C: //SRL H
						srl(&z80.H);
						break;
					case 0x3D: //SRL L
						srl(&z80.L);
						break;
					case 0x3E: //SRL (HL)
						srl_hl();
						break;
					case 0x3F: //SRL A
						srl(&z80.A);
						break;
					case 0x40: //BIT 0,B
						bit(0x01, z80.B);
						break;
					case 0x41: //BIT 0,C
						bit(0x01, z80.C);
						break;
					case 0x42: //BIT 0,D
						bit(0x01, z80.D);
						break;
					case 0x43: //BIT 0,E
						bit(0x01, z80.E);
						break;
					case 0x44: //BIT 0,H
						bit(0x01, z80.H);
						break;
					case 0x45: //BIT 0,L
						bit(0x01, z80.L);
						break;
					case 0x46: //BIT 0,(HL)
						bit(0x01, memory_read((z80.H << 8) + z80.L));
						break;
					case 0x47: //BIT 0,A
						bit(0x01, z80.A);
						break;
					case 0x48: //BIT 1,B
						bit(0x02, z80.B);
						break;
					case 0x49: //BIT 1,C
						bit(0x02, z80.C);
						break;
					case 0x4A: //BIT 1,D
						bit(0x02, z80.D);
						break;
					case 0x4B: //BIT 1,E
						bit(0x02, z80.E);
						break;
					case 0x4C: //BIT 1,H
						bit(0x02, z80.H);
						break;
					case 0x4D: //BIT 1,L
						bit(0x02, z80.L);
						break;
					case 0x4E: //BIT 1,(HL)
						bit(0x02, memory_read((z80.H << 8) + z80.L));
						break;
					case 0x4F: //BIT 1,A
						bit(0x02, z80.A);
						break;
					case 0x50: //BIT 2,B
						bit(0x04, z80.B);
						break;
					case 0x51: //BIT 2,C
						bit(0x04, z80.C);
						break;
					case 0x52: //BIT 2,D
						bit(0x04, z80.D);
						break;
					case 0x53: //BIT 2,E
						bit(0x04, z80.E);
						break;
					case 0x54: //BIT 2,H
						bit(0x04, z80.H);
						break;
					case 0x55: //BIT 2,L
						bit(0x04, z80.L);
						break;
					case 0x56: //BIT 2,(HL)
						bit(0x04, memory_read((z80.H << 8) + z80.L));
						break;
					case 0x57: //BIT 2,A
						bit(0x04, z80.A);
						break;
					case 0x58: //BIT 3,B
						bit(0x08, z80.B);
						break;
					case 0x59: //BIT 3,C
						bit(0x08, z80.C);
						break;
					case 0x5A: //BIT 3,D
						bit(0x08, z80.D);
						break;
					case 0x5B: //BIT 3,E
						bit(0x08, z80.E);
						break;
					case 0x5C: //BIT 3,H
						bit(0x08, z80.H);
						break;
					case 0x5D: //BIT 3,L
						bit(0x08, z80.L);
						break;
					case 0x5E: //BIT 3,(HL)
						bit(0x08, memory_read((z80.H << 8) + z80.L));
						break;
					case 0x5F: //BIT 3,A
						bit(0x08, z80.A);
						break;
				}
				break;
			case 0xCC: //CALL Z,a16
				call_cond(0x80);
				break;
			case 0xCD: //CALL a16
				call();
				break;
			case 0xCE: //ADC A,d8
				adc_d8();
				break;
			case 0xCF: //RST 08H
				rst(0x08);
				break;
			case 0xD0: //RET NC 
				ret_n_cond(0x10);
				break;
			case 0xD1: //POP DE
				pop(&z80.D, &z80.E);
				break;
			case 0xD2: //JP NC,a16
				jp_n_cond(0x10);
				break;
			case 0xD4: //CALL NC,a16
				call_n_cond(0x10);
				break;
			case 0xD5: //PUSH DE
				push(z80.D, z80.E);
				break;
			case 0xD6: //SUB d8
				sub(memory_read(z80.PC++));
				break;
			case 0xD7: //RST 10H
				rst(0x10);
				break;
			case 0xD8: //RET C
				ret(0x10);
				break;
			case 0xD9: //RETI
				reti();
				break;
			case 0xDA: //JP C,a16
				jp_cond(0x10);
				break;
			case 0xDC: //CALL C,a16
				call_cond(0x10);
				break;
			case 0xDE: //SBC A,d8	
				sbc(memory_read(z80.PC++));
				break;
			case 0xDF: //RST 18H
				rst(0x18);
				break;
			case 0xE0: //LDH (a8),A
				ld_at(0xFF00 + memory_read(z80.PC++));
				break;
			case 0xE1: //POP HL
				pop(&z80.H, &z80.L);
				break;
			case 0xE2: //LD (C), A
				ld_at(0xFF00 + z80.C);
				break;
			case 0xE5: //PUSH HL
				push(z80.H, z80.L);
				break;
			case 0xE6: //AND d8
				and(memory_read(z80.PC++));
				break;
			case 0xE7: //RST 20H
				rst(0x20);
				break;
			case 0xE8: //ADD SP,r8
				add_sp_r8(memory_read(z80.PC++));
				break;
			case 0xE9: //JP (HL)
				jp(memory_read((z80.H << 8) + z80.L));	
				break;
			case 0xEA: //LD (a16),A
				ld_at((memory_read(z80.PC++) << 8) + memory_read(z80.PC++));
				break;
			case 0xEE: //XOR d8
				xor(z80.PC++);
				break;
			case 0xEF: //RST 28H
				rst(0x28);
				break;
			case 0xF0: //LDH A,(a8)
				ld_reg_data(&z80.A, memory_read(0xFF00 + z80.PC++));
				break;
			case 0xF1: //POP AF
				pop(&z80.A, &z80.F);
				break;
			case 0xF2: //LD A,(C)
				ld_reg_data(&z80.A, memory_read(0xFF00 + z80.C));
				break;
			case 0xF3: //DI
				di();
				break;

			case 0xF5: //PUSH AF
				push(z80.A, z80.F);
				break;
			case 0xF6: //OR d8
				or(memory_read(z80.PC++));
				break;
			case 0xF7: //RST 30H
				rst(0x30);
				break;
			case 0xF8: //LD HL,SP+r8
				ld_hl_sp_p_r8();
				break;
			case 0xF9: //LD SP,HL
				ld_sp_hl();
				break;
			case 0xFA: //LD A,(a16)
				ld_reg_data(&z80.A, memory_read(memory_read(z80.PC++) << 8 + memory_read(Z80.PC++)));
				break;
			case 0xFB: //EI

				break;
			case 0xFE: //CP d8
				cp(memory_read(z80.PC++));
				break;

			case 0xFF: //RST 38H
				rst(0x38);
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

void or(BYTE data){
	z80.A |= data;
	if (z80.A == 0)	z80.F &= 0x80;
	else z80.F &= 0x00;
}

void cp(BYTE data){
	z80.F = 0x40;
	if (z80.A - data == 0) z80.F |= 0x80;
	if (z80.A > 0xF && z80.A - data <= 0xF) z80.F |= 0x20;
	if (z80.A - data < 0) z80.F |= 0x10;
}

void ret_n_cond(BYTE cond){
	if(z80.F & cond == 0){
		z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);;
		z80.SP += 2;
	}
}

void pop(BYTE *reg1, BYTE *reg2){
	*reg1 = memory_read(z80.SP);
	*reg2 = memory_read(z80.SP + 1);
	z80.SP += 2;
}

void jp_n_cond(BYTE cond){
	if(z80.F & cond == 0)
		z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
	else z80.PC += 2;
}

void jp(short addr){
	z80.PC = addr;
}

void call_n_cond(BYTE cond){
	if(z80.F & cond == 0){
		memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
		memory_write(z80.SP - 2, (z80.PC + 2) & 0xFF);
		z80.SP -= 2;
		z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
	}
	else z80.PC += 2;
}

void push(BYTE reg1, BYTE reg2){
	memory_write(z80.SP - 1, reg1);
	memory_write(z80.SP - 2, reg2);
	z80.SP -= 2;
}

void add_d8(){
	BYTE d8 = memory_read(z80.PC);
	z80.F = 0;
	if ((z80.A + d8) % 256 == 0) z80.F |= 0x80;
	if (z80.A + d8 > 0xFF) z80.F |= 0x10;
	if(z80.A <= 0xF && z80.A + d8 > 0xF) z80.F |= 0x20;
	z80.A = (z80.A + d8) % 256;																
	z80.PC++;
}

void rst(BYTE addr){
	memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
	memory_write(z80.SP - 2, z80.PC & 0xFF);
	z80.PC = addr;
	z80.SP -= 2;
}

void ret_cond(BYTE cond){
	if(z80.F & cond != 0){
		z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
		z80.SP += 2;
	}
}

void ret(){
	z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
	z80.SP += 2;
}

void jp_cond(BYTE cond){
	if(z80.F & cond != 0)
		z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
	else z80.PC += 2;
}

void jp(){
	z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
}

void call_cond(BYTE cond){
	if(z80.F & cond != 0){
		memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
		memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
		z80.SP -= 2;
		z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1); 
	}
	else z80.PC += 2;
}

void call(){
	memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
	memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
	z80.SP -= 2;
	z80.PC = memory_read(z80.PC) << 8 + memory_read(z80.PC + 1);
}

void adc(BYTE data){
	if (z80.F & 0x10) c = 1;
	else c = 0;

	if ((z80.A + data + c) % 256 == 0) z80.F |= 0x80;
	if (z80.A + data + c > 0xFF) z80.F |= 0x10;
	if(z80.A <= 0xF && z80.A + data + c > 0xF) z80.F |= 0x20;
	z80.A = (z80.A + data + c) % 256;
}

void sub(BYTE data){
	if (z80.A - data == 0) z80.F |= 0x80;
	if (z80.A - data < 0) z80.F |= 0x10;
	if(z80.A >= 0xF && z80.A - data < 0xF) z80.F |= 0x20;
	break;
}
void reti(){
	//PLUS FLAG IF
	z80.PC = memory_read(z80.SP + 1) << 8 + memory_read(z80.SP);
	z80.SP += 2;
}

void sbc(BYTE data){
	if (z80.F & 0x10) c = 1;
	else c = 0;

	if (z80.A + data + c == 0) z80.F |= 0x80;
	if (z80.A + data + c > 0xFF) z80.F |= 0x10;
	if(z80.A > 0xF && z80.A + data + c <= 0xF) z80.F |= 0x20;
	z80.A = (z80.A + data + c);
}

void ld_at(short addr){
	memory_write(addr, z80.A);
}

void ld_from_a8(short addr){
	z80.A = memory_read(addr);
}

void and(BYTE data){
	z80.F &= 0x20;
	z80.A &= data;
	if (z80.A == 0) z80.F |= 0x80;
}
void add_sp_r8(BYTE_S data){
	Z80.SP += data;
	//FLAGS
}

void xor(BYTE data){
	z80.A ^= data;
	if(z80.A == 0) z80.F &= 0x80;
	else z80.F = 0;
}

void di(){

}
void ld_hl_sp_p_r8(){
	r8 = memory_read(z80.PC);
	Z80.H = ((Z80.SP + r8) & 0xFF00) >> 8 ;
	Z80.L = ((Z80.SP + r8) & 0xFF);
	z80.PC++;
	//FLAGS
}
void ld_sp_hl(){
	z80.SP = (Z80.H << 8) + z80.L;
}

void ld_reg_data(BYTE *reg, BYTE data){
	*reg = data;
}

void rlc(BYTE *data){
	if (*data & 0x80 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if(z80.F & 0x10) *data |= 1;
	if (*data == 0) z80.F |= 0x80;
}

void rlc_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	if (hl & 0x80 == 0) z80.F = 0x00;
	else z80.F = 0x10;
	hl <<= 1;
	if(z80.F & 0x10) hl |= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void rrc(BYTE *data){
	if (*data & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if(z80.F & 0x10) *data |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

void rrc_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	if (hl & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if(z80.F & 0x10) hl |= 0x80;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void rl(BYTE *data){
	BYTE F = z80.F & 0x10;
	if (*data & 0x80 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if(F) *data |= 1;
	if (*data == 0) z80.F |= 0x80;
}

void rl_hl(){
	hl = memory_read(z80.PC);
	hl = memory_read((z80.H << 8) + z80.L);
	F = z80.F;
	if (hl & 0x80 == 0) z80.F = 0;
	else z80.F = 0x10;
	hl <<= 1;
	if(F & 0x10) hl |= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void rr(BYTE *data){
	BYTE F = z80.F & 0x10;
	if (*data & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if(F) *data |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

void rr_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	F = z80.F;
	if (hl & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if(F & 0x10) hl |= 0x80;
	if (z80.A == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void sla(BYTE *data){
	if (*data & 0x80 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if (*data == 0) z80.F |= 0x80;
}

void sla_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	if (z80.B & 0x80 == 0) z80.F = 0;
	else z80.F = 0x10;
	z80.B <<= 1;
	if (z80.B == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void sra(BYTE *data){
	BYTE F = *data & 0x80;
	if(*data & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if (F) z80.B |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

void sra_hl(){
	hl = memory_read((z80.H << 8) +z80.L);
	F = hl & 0x80;
	if(hl & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if (F) hl |= 0x80;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

void swap(BYTE *data){
	BYTE low_nibble = (*data & 0xF0) >> 4;
	BYTE high_nibble = (*data & 0x0F) << 4;
	*data = high_nibble + low_nibble;
	if(*data == 0) z80.F = 0x80;
	else z80.F = 0;
}

void swap_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	BYTE low_nibble = (z80.B & 0xF0) >> 4;
	BYTE high_nibble = (z80.B & 0x0F) << 4;
	z80.B = high_nibble + low_nibble;
	if(z80.B == 0) z80.F = 0x80;
	else z80.F = 0;
	memory_write((z80.H << 8) + z80.L, hl);
}

void srl(BYTE *data){
	if(*data & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if (*data == 0) z80.F |= 0x80;
}

void srl_hl(){
	hl = memory_read((z80.H << 8) + z80.L);
	if(hl & 0x01 == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L);
}

void bit(BYTE bit, BYTE data){
	if(z80.F & 0x10) z80.F = 0x30;
	else z80.F = 0x20;
	if(data & bit) z80.F |= 0x80;
}
