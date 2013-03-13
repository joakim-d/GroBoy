#include "cpu.h"

z80_t z80;
void cpu_init(){
	z80.PC = 0x0100;
	z80.SP = 0xFFFE;
	z80.A = 0x01;
	z80.F = 0xB0;
	z80.B = 0x00;
	z80.C = 0x13;
	z80.D = 0x00;
	z80.E = 0xD8;
	z80.H = 0x01;
	z80.L = 0x4D;
}

void run(){
	//int interrupt_period;
	//int counter;
	//counter=interrupt_period;
	int i = 0;
	BYTE op_code;
	for(;;)
	{
		op_code=memory_read(z80.PC);
		cycles = z80_cycles[op_code];
		//counter-=cycles[op_code];
		if(DEBUG){
			printf("instruction : %d\n", i++);
			printf("PC: %x\n", z80.PC);
			printf("Opcode :%x\n",op_code);
			printf("AF: %x\n", (z80.A << 8) + z80.F);
			printf("BC: %x\n", (z80.B << 8) + z80.C);
			printf("DE: %x\n", (z80.D << 8) + z80.E);
			printf("HL: %x\n", (z80.H << 8) + z80.L);
			printf("SP: %x\n", z80.SP);
			printf("Top of the stack: %x\n\n", (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP));
		}
		//printf("Flag Z: %d, Flag N: %d, Flag H: %d, Flag C: %d\n\n", z80.F & FLAG_Z, z80.F & FLAG_N, z80.F & FLAG_H, z80.F & FLAG_C);
		z80.PC++;
		switch(op_code){
			//Maxi table d'instructions

			case 0x00:
				//NOP
				break;
			case 0x01:
				//LD BC,d16
				ld_reg(&z80.B, memory_read(z80.PC++));
				ld_reg(&z80.C, memory_read(z80.PC++));
				break;
			case 0x02:
				//LD (BC),A
				//Décalage à gauche pour 
				//concaténation des valeurs de registres
				ld_at((z80.B << 8) + z80.C);
				break;
			case 0x03:
				//INC BC
				inc_dbl(&z80.B,&z80.C);
				break;
			case 0x04:
				//INC B
				inc_smpl(&z80.B);
				break;
			case 0x05:
				//DEC B
				dec_smpl(&z80.B);
				break;
			case 0x06:
				//LD B,*
				ld_reg(&z80.B, memory_read(z80.PC++));
				break;
			case 0x07:
				//RLCA
				rlca();
				break;
			case 0x08:
				//LD (A16),SP
				ld_a16_sp();
				break;
			case 0x09:
				//ADD HL,BC
				add_dbl(&z80.H,&z80.L, (z80.B << 8) + z80.C);
				break;
			case 0x0A:
				//LD A,(BC)
				ld_reg(&z80.A, memory_read((z80.B << 8)+z80.C));
				break;
			case 0x0B:
				//DEC BC
				dec_dbl(&z80.B,&z80.C);
				break;
			case 0x0C:
				//INC C
				inc_smpl(&z80.C);
				break;
			case 0x0D:
				//DEC C
				dec_smpl(&z80.C);
				break;
			case 0x0E:
				//LD C,d8
				ld_reg(&z80.C, memory_read(z80.PC++));
				break;
			case 0x0F:
				//RRCA 
				rrca();
				break;
			case 0x10:
				//STOP 0
				stop();
				break;
			case 0x11:
				//LD DE, d16
				ld_reg(&z80.D, memory_read(z80.PC++));
				ld_reg(&z80.E, memory_read(z80.PC++));
				break;
			case 0x12:
				//LD (DE), A
				ld_mem(memory_read((z80.D << 8)+z80.E), z80.A);
				break;
			case 0x13:
				//INC DE
				inc_dbl(&z80.D,&z80.E);
				break;
			case 0x14:
				//INC D
				inc_smpl(&z80.D);
				break;
			case 0x15:
				//DEC D
				dec_smpl(&z80.D);
				break;
			case 0x16:
				//LD D,d8
				ld_reg(&z80.D, memory_read(z80.PC++));
				break;
			case 0x17:
				//RLA
				rla();
				break;
			case 0x18:
				//JR r8
				jr(memory_read(z80.PC++));
				break;
			case 0x19:
				//ADD HL,DE
				add_dbl(&z80.H,&z80.L, (z80.D << 8) + z80.E);
				break;
			case 0x1A:
				//LD A,(DE)
				ld_reg(&z80.A, memory_read((z80.D << 8)+z80.E));
				break;
			case 0x1B:
				//DEC DE
				dec_dbl(&z80.D,&z80.E);
				break;
			case 0x1C:
				//INC E
				inc_smpl(&z80.E);
				break;
			case 0x1D:
				//DEC E
				dec_smpl(&z80.E);
				break;
			case 0x1E:
				//LD E,d8
				ld_reg(&z80.E, memory_read(z80.PC++));
				break;
			case 0x1F:
				//RRA
				rra();
				break;
			case 0x20:
				//JR NZ, R8
				jr_n_cond(FLAG_Z,memory_read(z80.PC++));
				break;
			case 0x21:
				//LD HL, d16
				ld_reg(&z80.L, memory_read(z80.PC++));
				ld_reg(&z80.H, memory_read(z80.PC++));
				break;
			case 0x22:
				//LD (HL+), A
				ld_mem((z80.H << 8)+z80.L, z80.A);
				inc_dbl(&z80.H,&z80.L);
				break;
			case 0x23:
				//INC HL
				inc_dbl(&z80.H,&z80.L);
				break;
			case 0x24:
				//INC H
				inc_smpl(&z80.H);
				break;
			case 0x25:
				//DEC H
				dec_smpl(&z80.H);
				break;
			case 0x26:
				//LD H,d8
				ld_reg(&z80.H, memory_read(z80.PC++));
				break;
			case 0x27:
				//DAA
				daa();
				break;
			case 0x28:
				//JR Z,r8
				jr_cond(FLAG_Z,memory_read(z80.PC++));
				break;
			case 0x29:
				//ADD HL,HL
				add_dbl(&z80.H,&z80.L, (z80.H << 8) + z80.L);
				break;
			case 0x2A:
				//LD A,(HL+)
				ld_reg(&z80.A, memory_read((z80.H << 8)+z80.L));
				inc_dbl(&z80.H,&z80.L);
				break;
			case 0x2B:
				//DEC HL
				dec_dbl(&z80.H, &z80.L);
				break;
			case 0x2C:
				//INC L
				inc_smpl(&z80.L);
				break;
			case 0x2D:
				//DEC L
				dec_smpl(&z80.L);
				break;
			case 0x2E:
				//LD L,d8
				ld_reg(&z80.L, memory_read(z80.PC++));
				break;
			case 0x2F:
				//CPL
				cpl();
				break;
			case 0x30:
				//JR NC,r8
				jr_n_cond(FLAG_C,memory_read(z80.PC++));
				break;
			case 0x31:
				//LD SP,d16
				ld_sp((memory_read(z80.PC + 1) << 8)+memory_read(z80.PC));
				z80.PC+=2;
				break;
			case 0x32:
				//LD (HL-), A
				ld_mem((z80.H << 8)+z80.L, z80.A);
				dec_dbl(&z80.H,&z80.L);
				break;
			case 0x33:
				//INC SP
				inc_sp();
				break;
			case 0x34:
				//INC (HL)
				inc_at((z80.H << 8) + z80.L);
				break;
			case 0x35:
				//DEC (HL)
				dec_at((z80.H << 8) + z80.L);
				break;
			case 0x36:
				//LD (HL),d8
				ld_mem((z80.H << 8)+z80.L, memory_read(z80.PC++));
				break;
			case 0x37:
				//SCF
				scf();
				break;
			case 0x38:
				//JR C,r8
				jr_cond(FLAG_C,memory_read(z80.PC++));
				break;
			case 0x39:
				//ADD HL,SP
				add_dbl(&z80.H,&z80.L, z80.SP);
				break;
			case 0x3A:
				//LD A,(HL-)
				ld_reg(&z80.A, memory_read((z80.H << 8)+z80.L));
				dec_dbl(&z80.H,&z80.L);
				break;
			case 0x3B:
				//DEC SP
				dec_sp();
				break;
			case 0x3C:
				//INC A
				inc_smpl(&z80.A);
				break;
			case 0x3D:
				//DEC A
				dec_smpl(&z80.A);
				break;
			case 0x3E:
				//LD A,d8
				ld_reg(&z80.A, memory_read(z80.PC++));
				break;
			case 0x3F:
				//CCF
				ccf();
				break;
			case 0x40:
				//LD B,B
				ld_reg(&z80.B,z80.B);
				break;
			case 0x41:
				//LD B,C
				ld_reg(&z80.B,z80.C);
				break;
			case 0x42:
				//LD B,D
				ld_reg(&z80.B,z80.D);
				break;
			case 0x43:
				//LD B,E
				ld_reg(&z80.B,z80.E);
				break;
			case 0x44:
				//LD B,H
				ld_reg(&z80.B,z80.H);
				break;
			case 0x45:
				//LD B,L
				ld_reg(&z80.B,z80.L);
				break;
			case 0x46:
				//LD B,(HL)
				ld_reg(&z80.B, memory_read((z80.H << 8)+z80.L));
				break;
			case 0x47:
				//LD B,A
				ld_reg(&z80.B,z80.A);
				break;
			case 0x48:
				//LD C,B
				ld_reg(&z80.C,z80.B);
				break;
			case 0x49:
				//LD C,C
				ld_reg(&z80.C,z80.C);
				break;
			case 0x4A:
				//LD C,D
				ld_reg(&z80.C,z80.D);
				break;
			case 0x4B:
				//LD C,E
				ld_reg(&z80.C,z80.E);
				break;
			case 0x4C:
				//LD C,H
				ld_reg(&z80.C,z80.H);
				break;
			case 0x4D:
				//LD C,L
				ld_reg(&z80.C,z80.L);
				break;
			case 0x4E:
				//LD C,(HL)
				ld_reg(&z80.C, memory_read((z80.H << 8)+z80.L));
				break;
			case 0x4F:
				//LD C,A
				ld_reg(&z80.C,z80.A);
				break;
			case 0x50:
				//LD D,B
				ld_reg(&z80.D,z80.B);
				break;
			case 0x51:
				//LD D,C
				ld_reg(&z80.D,z80.C);
				break;
			case 0x52:
				//LD D,D
				ld_reg(&z80.D,z80.D);
				break;
			case 0x53:
				//LD D,E
				ld_reg(&z80.D,z80.E);
				break;
			case 0x54:
				//LD D,H
				ld_reg(&z80.D,z80.H);
				break;
			case 0x55:
				//LD D,L
				ld_reg(&z80.D,z80.L);
				break;
			case 0x56:
				//LD D,(HL)
				ld_reg(&z80.D, memory_read((z80.H << 8) + z80.L));
				break;
			case 0x57:
				//LD D,A
				ld_reg(&z80.D,z80.A);
				break;
			case 0x58:
				//LD E,B
				ld_reg(&z80.E,z80.B);
				break;
			case 0x59:
				//LD E,C
				ld_reg(&z80.E,z80.C);
				break;
			case 0x5A:
				//LD E,D
				ld_reg(&z80.E,z80.D);
				break;
			case 0x5B:
				//LD E,E
				ld_reg(&z80.E,z80.E);
				break;
			case 0x5C:
				//LD E,H
				ld_reg(&z80.E,z80.H);
				break;
			case 0x5D:
				//LD E,L
				ld_reg(&z80.E,z80.L);
				break;
			case 0x5E:
				//LD E,(HL)
				ld_reg(&z80.E,memory_read((z80.H << 8)+z80.L));
				break;
			case 0x5F:
				//LD E,A
				ld_reg(&z80.E,z80.A);
				break;
			case 0x60:
				//LD H,B
				ld_reg(&z80.H,z80.B);
				break;
			case 0x61:
				//LD H,C
				ld_reg(&z80.H,z80.C);
				break;
			case 0x62:
				//LD H,D
				ld_reg(&z80.H,z80.D);
				break;
			case 0x63:
				//LD H,E
				ld_reg(&z80.H,z80.E);
				break;
			case 0x64:
				//LD H,H
				ld_reg(&z80.H,z80.H);
				break;
			case 0x65:
				//LD H,L
				ld_reg(&z80.H,z80.L);
				break;
			case 0x66:
				//LD H,(HL)
				ld_reg(&z80.H,memory_read((z80.H << 8) + z80.L));
				break;
			case 0x67:
				//LD H,A
				ld_reg(&z80.H,z80.A);
				break;
			case 0x68:
				//LD L,B
				ld_reg(&z80.L,z80.B);
				break;
			case 0x69:
				//LD L,C
				ld_reg(&z80.L,z80.C);
				break;
			case 0x6A:
				//LD L,D
				ld_reg(&z80.L,z80.D);
				break;
			case 0x6B:
				//LD L,E
				ld_reg(&z80.L,z80.E);
				break;
			case 0x6C:
				//LD L,H
				ld_reg(&z80.L,z80.H);
				break;
			case 0x6D:
				//LD L,L
				ld_reg(&z80.L,z80.L);
				break;
			case 0x6E:
				//LD L,(HL)
				ld_reg(&z80.L,memory_read((z80.H << 8)+z80.L));
				break;
			case 0x6F:
				//LD L,A
				ld_reg(&z80.L,z80.A);
				break;
			case 0x70:
				//LD (HL),B
				ld_mem((z80.H << 8)+z80.L,z80.B);
				break;
			case 0x71:
				//LD (HL),C
				ld_mem((z80.H << 8)+z80.L,z80.C);
				break;
			case 0x72:
				//LD (HL),D
				ld_mem((z80.H << 8)+z80.L,z80.D);
				break;
			case 0x73:
				//LD (HL),E
				ld_mem((z80.H << 8)+z80.L,z80.E);
				break;
			case 0x74:
				//LD (HL),H
				ld_mem((z80.H << 8)+z80.L,z80.H);
				break;
			case 0x75:
				//LD (HL),L
				ld_mem((z80.H << 8)+z80.L,z80.L);
				break;
			case 0x76:
				//HALT
				//exit(0);
				break;
			case 0x77:
				//LD (HL),A
				ld_mem((z80.H << 8)+z80.L,z80.A);
				break;
			case 0x78:
				//LD A,B
				ld_reg(&z80.A,z80.B);
				break;
			case 0x79:
				//LD A,C
				ld_reg(&z80.A,z80.C);
				break;
			case 0x7A:
				//LD A,D
				ld_reg(&z80.A,z80.D);
				break;
			case 0x7B:
				//LD A,E 
				ld_reg(&z80.A,z80.E);
				break;
			case 0x7C:
				//LD A,H
				ld_reg(&z80.A,z80.H);
				break;
			case 0x7D:
				//LD A,L
				ld_reg(&z80.A,z80.L);
				break;
			case 0x7E:
				//LD A,(HL)
				ld_reg(&z80.A,memory_read((z80.H << 8)+z80.L));
				break;
			case 0x7F:
				//LD A,A
				ld_reg(&z80.A,z80.A);
				break;

			case 0x80:
				//ADD A,B
				add(z80.B);
				break;
			case 0x81:
				//ADD A,C
				add(z80.C);
				break;
			case 0x82:
				//ADD A,D
				add(z80.D);
				break;
			case 0x83:
				//ADD A,E
				add(z80.E);
				break;
			case 0x84:
				//ADD A,H
				add(z80.H);
				break;
			case 0x85:
				//ADD A,L
				add(z80.L);
				break;
			case 0x86:
				//ADD A,(HL)
				add(memory_read((z80.H << 8)+z80.L));
				break;
			case 0x87:
				//ADD A,A
				add(z80.A);
				break;
			case 0x88:
				//ADC A,B
				adc(z80.B);
				break;
			case 0x89:
				//ADC A,C
				adc(z80.C);
				break;
			case 0x8A:
				//ADC A,D
				adc(z80.D);
				break;
			case 0x8B:
				//ADC A,E
				adc(z80.E);
				break;
			case 0x8C:
				//ADC A,H
				adc(z80.H);
				break;
			case 0x8D:
				//ADC A,L
				adc(z80.L);
				break;
			case 0x8E:
				//ADC A,(HL)
				adc(memory_read((z80.H << 8)+z80.L));
				break;
			case 0x8F:
				//ADC A,A
				adc(z80.A);
				break;
			case 0x90:
				//SUB B
				sub(z80.B);
				break;
			case 0x91:
				//SUB C
				sub(z80.C);
				break;
			case 0x92:
				//SUB D
				sub(z80.D);
				break;
			case 0x93:
				//SUB E
				sub(z80.E);
				break;
			case 0x94:
				//SUB H
				sub(z80.H);
				break;
			case 0x95:
				//SUB L
				sub(z80.L);
				break;
			case 0x96:
				//SUB (HL)
				sub(memory_read((z80.H << 8)+z80.L));
				break;
			case 0x97:
				//SUB A
				sub(z80.A);
				break;
			case 0x98:
				//SBC A,B
				sbc(z80.B);
				break;
			case 0x99:
				//SBC A,C
				sbc(z80.C);
				break;
			case 0x9A:
				//SBC A,D
				sbc(z80.D);
				break;
			case 0x9B:
				//SBC A,E
				sbc(z80.E);
				break;
			case 0x9C:
				//SBC A,H
				sbc(z80.H);
				break;
			case 0x9D:
				//SBC A,L
				sbc(z80.L);
				break;
			case 0x9E:
				//SBC A,(HL)
				sbc(memory_read((z80.H << 8)+z80.L));
				break;
			case 0x9F:
				//SBC A,A
				sbc(z80.A);
				break;
			case 0xA0:
				//AND B
				and(z80.B);
				break;
			case 0xA1:
				//AND C
				and(z80.C);
				break;
			case 0xA2:
				//AND D
				and(z80.D);
				break;
			case 0xA3:
				//AND E
				and(z80.E);
				break;
			case 0xA4:
				//AND H
				and(z80.H);
				break;
			case 0xA5:
				//AND L
				and(z80.L);
				break;
			case 0xA6:
				//AND (HL)
				and(memory_read((z80.H << 8)+z80.L));
				break;
			case 0xA7:
				//AND A
				and(z80.A);
				break;
			case 0xA8:
				//XOR B
				xor(z80.B);
				break;
			case 0xA9:
				//XOR C
				xor(z80.C);
				break;
			case 0xAA:
				//XOR D
				xor(z80.D);
				break;
			case 0xAB:
				//XOR E
				xor(z80.E);
				break;
			case 0xAC:
				//XOR H
				xor(z80.H);
				break;
			case 0xAD:
				//XOR L
				xor(z80.L);
				break;
			case 0xAE:
				//XOR (HL)
				xor(memory_read((z80.H << 8)+z80.L));
				break;
			case 0xAF:
				//XOR A
				xor(z80.A);
				break;

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
				ret_n_cond(FLAG_Z);
				break;
			case 0xC1: //POP BC
				pop(&z80.B, &z80.C);
				break;
			case 0xC2: //JP NZ,a16
				jp_n_cond(FLAG_Z);
				break;
			case 0xC3: //JP a16
				jp((memory_read(z80.PC + 1) << 8) + memory_read(z80.PC));
				break;
			case 0xC4: //CALL NZ,a16
				call_n_cond(0x80);
				break;
			case 0xC5: //PUSH BC
				push(z80.B, z80.C);
				break;
			case 0xC6: //ADD A,d8
				add(memory_read(z80.PC++));
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
				op_code = memory_read(z80.PC++);
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
					case 0x60: //BIT 4,B
						bit((0x01 << 4),z80.B);
						break;
					case 0x61: //BIT 4,C
						bit((0x01 << 4),z80.C);
						break;
					case 0x62: //BIT 4,D
						bit((0x01 << 4),z80.D);
						break;
					case 0x63: //BIT 4,E
						bit((0x01 << 4),z80.E);
						break;
					case 0x64: //BIT 4,H
						bit((0x01 << 4),z80.H);
						break;
					case 0x65: //BIT 4,L
						bit((0x01 << 4),z80.L);
						break;
					case 0x66: //BIT 4,HL
						bit((0x01 << 4),memory_read((z80.H << 8) + z80.L));
						break;
					case 0x67: //BIT 4,A
						bit((0x01 << 4),z80.A);
						break;
					case 0x68: //BIT 5,B
						bit((0x01 << 5),z80.B);
						break;
					case 0x69: //BIT 5,C
						bit((0x01 << 5),z80.C);
						break;
					case 0x6A: //BIT 5,D
						bit((0x01 << 5),z80.D);
						break;
					case 0x6B: //BIT 5,E
						bit((0x01 << 5),z80.E);
						break;
					case 0x6C: //BIT 5,H
						bit((0x01 << 5),z80.H);
						break;
					case 0x6D: //BIT 5,L
						bit((0x01 << 5),z80.L);
						break;
					case 0x6E: //BIT 5,HL
						bit((0x01 << 5),memory_read((z80.H << 8) + z80.L));
						break;
					case 0x6F: //BIT 5,A
						bit((0x01 << 5),z80.A);
						break;
					case 0x70: //BIT 6,B
						bit((0x01 << 6),z80.B);
						break;
					case 0x71: //BIT 6,C
						bit((0x01 << 6),z80.C);
						break;
					case 0x72: //BIT 6,D
						bit((0x01 << 6),z80.D);
						break;
					case 0x73: //BIT 6,E
						bit((0x01 << 6),z80.E);
						break;
					case 0x74: //BIT 6,H
						bit((0x01 << 6),z80.H);
						break;
					case 0x75: //BIT 6,L
						bit((0x01 << 6),z80.L);
						break;
					case 0x76: //BIT 6,HL
						bit((0x01 << 6),memory_read((z80.H << 8) + z80.L));
						break;
					case 0x77: //BIT 6,A
						bit((0x01 << 6),z80.A);
						break;
					case 0x78: //BIT 7,B
						bit((0x01 << 7),z80.B);
						break;
					case 0x79: //BIT 7,C
						bit((0x01 << 7),z80.C);
						break;
					case 0x7A: //BIT 7,D
						bit((0x01 << 7),z80.D);
						break;
					case 0x7B: //BIT 7,E
						bit((0x01 << 7),z80.E);
						break;
					case 0x7C: //BIT 7,H
						bit((0x01 << 7),z80.H);
						break;
					case 0x7D: //BIT 7,L
						bit((0x01 << 7),z80.L);
						break;
					case 0x7E: //BIT 7,HL
						bit((0x01 << 7),memory_read((z80.H << 8) + z80.L));
						break;
					case 0x7F: //BIT 7,A
						bit((0x01 << 7),z80.A);
						break;
					case 0x80: //RES 0,B
						res(0,&z80.B);
						break;
					case 0x81: //RES 0,C
						res(0,&z80.C);
						break;
					case 0x82: //RES 0,D
						res(0,&z80.D);
						break;
					case 0x83: //RES 0,E
						res(0,&z80.E);
						break;
					case 0x84: //RES 0,H
						res(0,&z80.H);
						break;
					case 0x85: //RES 0,L
						res(0,&z80.L);
						break;
					case 0x86: //RES 0,HL
						res_hl(0);
						break;
					case 0x87: //RES 0,A
						res(0,&z80.A);
						break;
					case 0x88: //RES 1,B
						res(1,&z80.B);
						break;
					case 0x89: //RES 1,C
						res(1,&z80.C);
						break;
					case 0x8A: //RES 1,D
						res(1,&z80.D);
						break;
					case 0x8B: //RES 1,E
						res(1,&z80.E);
						break;
					case 0x8C: //RES 1,H
						res(1,&z80.H);
						break;
					case 0x8D: //RES 1,L
						res(1,&z80.L);
						break;
					case 0x8E: //RES 1,HL
						res_hl(1);
						break;
					case 0x8F: //RES 1,A
						res(1,&z80.A);
						break;
					case 0x90: //RES 2,B
						res(2,&z80.B);
						break;
					case 0x91: //RES 2,C
						res(2,&z80.C);
						break;
					case 0x92: //RES 2,D
						res(2,&z80.D);
						break;
					case 0x93: //RES 2,E
						res(2,&z80.E);
						break;
					case 0x94: //RES 2,H
						res(2,&z80.H);
						break;
					case 0x95: //RES 2,L
						res(2,&z80.L);
						break;
					case 0x96: //RES 2,HL
						res_hl(2);
						break;
					case 0x97: //RES 2,A
						res(2,&z80.A);
						break;
					case 0x98: //RES 3,B
						res(3,&z80.B);
						break;
					case 0x99: //RES 3,C
						res(3,&z80.C);
						break;
					case 0x9A: //RES 3,D
						res(3,&z80.D);
						break;
					case 0x9B: //RES 3,E
						res(3,&z80.E);
						break;
					case 0x9C: //RES 3,H
						res(3,&z80.H);
						break;
					case 0x9D: //RES 3,L
						res(3,&z80.L);
						break;
					case 0x9E: //RES 3,HL
						res_hl(3);
						break;
					case 0x9F: //RES 3,A
						res(3,&z80.A);
						break;
					case 0xA0: //RES 4,B
						res(4,&z80.B);
						break;
					case 0xA1: //RES 4,C
						res(4,&z80.C);
						break;
					case 0xA2: //RES 4,D
						res(4,&z80.D);
						break;
					case 0xA3: //RES 4,E
						res(4,&z80.E);
						break;
					case 0xA4: //RES 4,H
						res(4,&z80.H);
						break;
					case 0xA5: //RES 4,L
						res(4,&z80.L);
						break;
					case 0xA6: //RES 4,HL
						res_hl(4);
						break;
					case 0xA7: //RES 4,A
						res(4,&z80.A);
						break;
					case 0xA8: //RES 5,B
						res(5,&z80.B);
						break;
					case 0xA9: //RES 5,C
						res(5,&z80.C);
						break;
					case 0xAA: //RES 5,D
						res(5,&z80.D);
						break;
					case 0xAB: //RES 5,E
						res(5,&z80.E);
						break;
					case 0xAC: //RES 5,H
						res(5,&z80.H);
						break;
					case 0xAD: //RES 5,L
						res(5,&z80.L);
						break;
					case 0xAE: //RES 5,HL
						res_hl(5);
						break;
					case 0xAF: //RES 5,A
						res(5,&z80.A);
						break;
					case 0xB0: //RES 6,B
						res(6,&z80.B);
						break;
					case 0xB1: //RES 6,C
						res(6,&z80.C);
						break;
					case 0xB2: //RES 6,D
						res(6,&z80.D);
						break;
					case 0xB3: //RES 6,E
						res(6,&z80.E);
						break;
					case 0xB4: //RES 6,H
						res(6,&z80.H);
						break;
					case 0xB5: //RES 6,L
						res(6,&z80.L);
						break;
					case 0xB6: //RES 6,Hl
						res_hl(6);
						break;
					case 0xB7: //RES 6,A
						res(6,&z80.A);
						break;
					case 0xB8: //RES 7,B
						res(7,&z80.B);
						break;
					case 0xB9: //RES 7,C
						res(7,&z80.C);
						break;
					case 0xBA: //RES 7,D
						res(7,&z80.D);
						break;
					case 0xBB: //RES 7,E
						res(7,&z80.E);
						break;
					case 0xBC: //RES 7,H
						res(7,&z80.H);
						break;
					case 0xBD: //RES 7,L
						res(7,&z80.L);
						break;
					case 0xBE: //RES 7,HL
						res_hl(7);
						break;
					case 0xBF: //RES 7,A
						res(7,&z80.A);
						break;
					case 0xC0: //SET 0,B
						set(0,&z80.B);
						break;
					case 0xC1: //SET 0,C
						set(0,&z80.C);
						break;
					case 0xC2: //SET 0,D
						set(0,&z80.D);
						break;
					case 0xC3: //SET 0,E
						set(0,&z80.E);
						break;
					case 0xC4: //SET 0,H
						set(0,&z80.H);
						break;
					case 0xC5: //SET 0,L
						set(0,&z80.L);
						break;
					case 0xC6: //SET 0,HL
						set_hl(0);
						break;
					case 0xC7: //SET 0,A
						set(0,&z80.A);
						break;
					case 0xC8: //SET 1,B
						set(1,&z80.B);
						break;
					case 0xC9: //SET 1,C
						set(1,&z80.C);
						break;
					case 0xCA: //SET 1,D
						set(1,&z80.D);
						break;
					case 0xCB: //SET 1,E
						set(1,&z80.E);
						break;
					case 0xCC: //SET 1,H
						set(1,&z80.H);
						break;
					case 0xCD: //SET 1,L
						set(1,&z80.L);
						break;
					case 0xCE: //SET 1,HL
						set_hl(1);
						break;
					case 0xCF: //SET 1,A
						set(1,&z80.A);
						break;
					case 0xD0: //SET 2,B
						set(2,&z80.B);
						break;
					case 0xD1: //SET 2,C
						set(2,&z80.C);
						break;
					case 0xD2: //SET 2,D
						set(2,&z80.D);
						break;
					case 0xD3: //SET 2,E
						set(2,&z80.E);
						break;
					case 0xD4: //SET 2,H
						set(2,&z80.H);
						break;
					case 0xD5: //SET 2,L
						set(2,&z80.L);
						break;
					case 0xD6: //SET 2,HL
						set_hl(2);
						break;
					case 0xD7: //SET 2,A
						set(2,&z80.A);
						break;
					case 0xD8: //SET 3,B
						set(3,&z80.B);
						break;
					case 0xD9: //SET 3,C
						set(3,&z80.C);
						break;
					case 0xDA: //SET 3,D
						set(3,&z80.D);
						break;
					case 0xDB: //SET 3,E
						set(3,&z80.E);
						break;
					case 0xDC: //SET 3,H
						set(3,&z80.H);
						break;
					case 0xDD: //SET 3,L
						set(3,&z80.L);
						break;
					case 0xDE: //SET 3,Hl
						set_hl(3);
						break;
					case 0xDF: //SET 3,A
						set(3,&z80.A);
						break;
					case 0xE0: //SET 4,B
						set(4,&z80.B);
						break;
					case 0xE1: //SET 4,C
						set(4,&z80.C);
						break;
					case 0xE2: //SET 4,D
						set(4,&z80.D);
						break;
					case 0xE3: //SET 4,E
						set(4,&z80.E);
						break;
					case 0xE4: //SET 4,H
						set(4,&z80.H);
						break;
					case 0xE5: //SET 4,L
						set(4,&z80.L);
						break;
					case 0xE6: //SET 4,HL
						set_hl(4);
						break;
					case 0xE7: //SET 4,A
						set(4,&z80.A);
						break;
					case 0xE8: //SET 5,B
						set(5,&z80.B);
						break;
					case 0xE9: //SET 5,C
						set(5,&z80.C);
						break;
					case 0xEA: //SET 5,D
						set(5,&z80.D);
						break;
					case 0xEB: //SET 5,E
						set(5,&z80.E);
						break;
					case 0xEC: //SET 5,H
						set(5,&z80.H);
						break;
					case 0xED: //SET 5,L
						set(5,&z80.L);
						break;
					case 0xEE: //SET 5,HL
						set_hl(5);
						break;
					case 0xEF: //SET 5,A
						set(5,&z80.A);
						break;
					case 0xF0: //SET 6,B
						set(6,&z80.B);
						break;
					case 0xF1: //SET 6,C
						set(6,&z80.C);
						break;
					case 0xF2: //SET 6,D
						set(6,&z80.D);
						break;
					case 0xF3: //SET 6,E
						set(6,&z80.E);
						break;
					case 0xF4: //SET 6,H
						set(6,&z80.H);
						break;
					case 0xF5: //SET 6,L
						set(6,&z80.L);
						break;
					case 0xF6: //SET 6,HL
						set_hl(6);
						break;
					case 0xF7: //SET 6,A
						set(6,&z80.A);
						break;
					case 0xF8: //SET 7,B
						set(7,&z80.B);
						break;
					case 0xF9: //SET 7,C
						set(7,&z80.C);
						break;
					case 0xFA: //SET 7,D
						set(7,&z80.D);
						break;
					case 0xFB: //SET 7,E
						set(7,&z80.E);
						break;
					case 0xFC: //SET 7,H
						set(7,&z80.H);
						break;
					case 0xFD: //SET 7,L
						set(7,&z80.L);
						break;
					case 0xFE: //SET 7,HL
						set_hl(7);
						break;
					case 0xFF: //SET 7,A
						set(7,&z80.A);
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
				adc(memory_read(z80.PC++));
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
				ld_at((memory_read(z80.PC + 1) << 8) + memory_read(z80.PC));
				z80.PC += 2;
				break;
			case 0xEE: //XOR d8
				xor(z80.PC++);
				break;
			case 0xEF: //RST 28H
				rst(0x28);
				break;
			case 0xF0: //LDH A,(a8)
				ld_reg(&z80.A, memory_read(0xFF00 + memory_read(z80.PC++)));
				break;
			case 0xF1: //POP AF
				pop(&z80.A, &z80.F);
				break;
			case 0xF2: //LD A,(C)
				ld_reg(&z80.A, memory_read(0xFF00 + z80.C));
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
				ld_sp((z80.H << 8) + z80.L);
				break;
			case 0xFA: //LD A,(a16)
				ld_reg(&z80.A, memory_read((memory_read(z80.PC + 1) << 8) + memory_read(z80.PC)));
				z80.PC +=2;
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
		gpu_update(cycles);	
		handle_interrupts(&z80);

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


static inline void halfcarry_8bit_update(BYTE old_value, BYTE new_value, BYTE type){
	BYTE old_nibble, new_nibble;
	old_nibble = old_value & 0xF;
	new_nibble = new_value & 0xF;
	if(type == ADD){
		if(new_nibble < old_nibble) z80.F |= 0x20;
	}
	else{
		if(new_nibble > old_nibble) z80.F |= 0x20;
	}
}

static inline void halfcarry_16bit_update(unsigned short old_value, unsigned short new_value, BYTE type){
	BYTE old_nibble, new_nibble;
	old_nibble = (old_value & 0xF00)>>8;
	new_nibble = (new_value & 0xF00)>>8;
	if(type == ADD){
		if(new_nibble < old_nibble) z80.F |= 0x20;
	}
	else{
		if(new_nibble > old_nibble) z80.F |= 0x20;
	}
}

//-----------Misc/control instructions------------------

static inline void di(){
	reset_IME();
}
static inline void ei(){
	set_IME();
}
static inline void halt(){

}
static inline void stop(){

}
//----------Jumps/calls--------------------------------

static inline void call(){
	memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
	memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
	z80.SP -= 2;
	z80.PC = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC);
}

static inline void call_cond(BYTE cond){
	if(z80.F & cond){
		memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
		memory_write(z80.SP - 2, (z80.PC + 2) & 0x00FF);
		z80.SP -= 2;
		z80.PC = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC); 
		cycles += 12;
	}
	else z80.PC += 2;
}

static inline void call_n_cond(BYTE cond){
	if(!(z80.F & cond)){
		memory_write(z80.SP - 1, ((z80.PC + 2) & 0xFF00) >> 8);
		memory_write(z80.SP - 2, (z80.PC + 2) & 0xFF);
		z80.SP -= 2;
		z80.PC = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC); 
		cycles += 12;
	}
	else z80.PC += 2;
}

static inline void jp(unsigned short addr){
	z80.PC = addr;
}

static inline void jp_cond(BYTE cond){
	if(z80.F & cond){
		z80.PC = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC);
		cycles += 4;
	}
	else z80.PC += 2;
}

static inline void jp_n_cond(BYTE cond){
	if((z80.F & cond) == 0){
		z80.PC = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC);
		cycles += 4;
	}
	else z80.PC += 2;
}

static inline void jr(BYTE_S d){
	// - - - -
	//Jump Relatif
	z80.PC += d; 
}

static inline void jr_cond(BYTE cond, BYTE_S d){
	// - - - -
	if(z80.F & cond){
		z80.PC += d;
		cycles += 4;
	}
}

static inline void jr_n_cond(BYTE cond, BYTE_S d){
	// - - - -
	if(!(z80.F & cond)){
		z80.PC += d;
		cycles += 4;
	}
}

static inline void ret(){
	z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);
	z80.SP += 2;
}

static inline void ret_cond(BYTE cond){
	if(z80.F & cond){
		z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);
		z80.SP += 2;
		cycles += 12;
	}
}

static inline void ret_n_cond(BYTE cond){
	if(!(z80.F & cond)){
		z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);;
		z80.SP += 2;
		cycles += 12;
	}
}

static inline void reti(){
	//PLUS FLAG IF
	z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);
	z80.SP += 2;
	set_IME();
}

static inline void rst(BYTE addr){
	memory_write(z80.SP - 1, (z80.PC & 0xFF00) >> 8);
	memory_write(z80.SP - 2, z80.PC & 0xFF);
	z80.PC = addr;
	z80.SP -= 2;
}

//--------8bit load/store/move instructions

static inline void ld_reg(BYTE *reg, BYTE data){
	//printf("%d\n",data);
	*reg = data;
}
static inline void ld_mem(unsigned short addr, BYTE data){
	//LD (reg1),reg2
	//LD (reg1), d
	memory_write(addr, data);

}
//--------16bit load/store/move instructions

static inline void ld_a16_sp(){
	unsigned short addr = (memory_read(z80.PC + 1) << 8) + memory_read(z80.PC);
	memory_write(addr, (z80.SP & 0xFF00) >> 8);
	memory_write(addr, (z80.SP & 0x00FF));
	z80.PC += 2;
}

static inline void ld_at(unsigned short addr){
	memory_write(addr, z80.A);
}

static inline void ld_from_a8(unsigned short addr){
	z80.A = memory_read(addr);
}

static inline void ld_hl_sp_p_r8(){
	BYTE_S r8 = memory_read(z80.PC);
	unsigned short a16 = z80.SP + r8;
	if(z80.SP + r8 > 0xFFFF || z80.SP + r8 < 0) z80.F |= 0x10;
	if(r8 > 0) halfcarry_16bit_update(z80.SP, a16, ADD);
	else if (r8 < 0) halfcarry_16bit_update(z80.SP, a16, SUB);
	z80.H = (a16 & 0xFF00) >> 8 ;
	z80.L = (a16 & 0xFF);
	z80.PC++;
}

static inline void ld_sp(unsigned short data){
	z80.SP = data;
}

static inline void pop(BYTE *reg1, BYTE *reg2){
	*reg1 = memory_read(z80.SP);
	*reg2 = memory_read(z80.SP + 1);
	z80.SP += 2;
}

static inline void push(BYTE reg1, BYTE reg2){
	memory_write(z80.SP - 1, reg1);
	memory_write(z80.SP - 2, reg2);
	z80.SP -= 2;
}

//--------8bit arithmetic/logical instructions


static inline void adc(BYTE data){
	BYTE c;
	BYTE op;
	if (z80.F & 0x10) c = 1;
	else c = 0;
	op = z80.A + data + c;

	if (op == 0) z80.F |= 0x80;
	if (z80.A + data + c > 0xFF) z80.F |= 0x10;
 	halfcarry_8bit_update(z80.A, op, ADD);
	z80.A = z80.A + data + c;
}

static inline void add(BYTE data){
	BYTE op = z80.A + data;
	z80.F = 0;
	if (op == 0) z80.F |= 0x80;
	if (op < z80.A) z80.F |= 0x10;
 	halfcarry_8bit_update(z80.A, op, ADD);
	z80.A = op;
}

static inline void and(BYTE data){
	z80.F = 0x20;
	z80.A &= data;
	if (z80.A == 0) z80.F |= 0x80;
}

static inline void ccf(){
	// - 0 0 C
	//Inverse le Carry Flag
	//Clear le N flag et le H flag
	BYTE F = z80.F;
	if(z80.F & 0x10) z80.F = F & FLAG_Z;
	else z80.F = (F & FLAG_Z) + 0x10;
}

static inline void cp(BYTE data){
	BYTE op = z80.A - data;
	z80.F = 0x40;
	if (op == 0) z80.F |= 0x80;
	if (op > z80.A) z80.F |= 0x10;
 	halfcarry_8bit_update(z80.A, op, SUB);
}

static inline void cpl(){
	// - 1 1 -
	//Inverse tous les bits de A
	//( == A XOR 0xFF)
	z80.A ^= 0xFF;
	z80.F |= 0x60; // H + N
}

static inline void daa()
{
	unsigned int temp = z80.A;
	if(z80.F & 0x40)
	{
		if(z80.F & (0x20))
		{
			temp = (temp - 6) & 0xff;
		}
		if(z80.F & (0x10))
		{
			temp -= 0x96;
		}
	}
	else
	{
		if(z80.F & (0x20) || (temp & 0x0f) > 9)
		{
			temp += 6;
		}
		if(z80.F & (0x10) || (temp >> 4 ) > 9)
		{
			temp += 0x96;
		}
	}

	if(temp & 0x100)
	{
		z80.F |= 0x10;
	}

	temp &= ~(0x20);

	temp &= 0xff;
	if(z80.A == 0) z80.F |= 0x80;
	else z80.F &= ~(0x80);

	z80.A = temp;
}

static inline void dec_at(unsigned short addr){
	// Z 1 H -
	BYTE d8 = memory_read(addr);
	if (z80.F & 0x10) z80.F = 0x50;
	else z80.F = 0x40;
	if (d8 == 0) z80.F |= 0x80;
 	halfcarry_8bit_update(d8, d8-1, SUB);
	memory_write(addr, d8);
}

static inline void dec_sp(){
	// - - - -
	z80.SP -= 1;
}

static inline void dec_smpl(BYTE *reg1){
	// Z 1 H -
	if(z80.F & FLAG_C) z80.F = 0x50;
	else z80.F = 0x40;
	halfcarry_8bit_update(*reg1, *reg1 - 1, SUB);
	*reg1 = *reg1 - 1;	
	if(*reg1 == 0) z80.F |= FLAG_Z;
}

static inline void inc_at(unsigned short addr){
	// Z 0 H -
	BYTE d8 = memory_read(addr);
	if (z80.F & 0x10) z80.F = 0x10;
	else z80.F = 0;
	if(d8 == 0) z80.F |= 0x80;
 	halfcarry_8bit_update(d8, d8+1, ADD);
	memory_write(addr, d8);
}


static inline void inc_smpl(BYTE *reg1){
	// Z 0 H -
	*reg1+=0x01;
	if(z80.F & FLAG_C) z80.F = FLAG_C;
	else z80.F = 0;
	if(*reg1 == 0) z80.F |= FLAG_Z;
	else if(*reg1 == 0x10) z80.F |= FLAG_H;
}

static inline void inc_sp(){
	// - - - -
	z80.SP += 1;
}
static inline void sbc(BYTE data){
	BYTE c;
	BYTE op;
	if (z80.F & 0x10) c = 1;
	else c = 0;
	op = z80.A - data - c;
	if (op == 0) z80.F |= 0x80;
	if (op > z80.A) z80.F |= 0x10;
 	halfcarry_8bit_update(z80.A, op, SUB);
	z80.A = (z80.A - data - c);
}

static inline void scf(){
	// - 0 0 1
	if(z80.F & FLAG_Z) z80.F = 0x90;
	else z80.F = 0x10;
}

static inline void sub(BYTE data){
	BYTE op = z80.A - data;
	if (op == 0) z80.F |= 0x80;
	if (op > z80.A) z80.F |= 0x10;
 	halfcarry_8bit_update(z80.A, op, SUB);
	z80.A -= data;
}

static inline void or(BYTE data){
	z80.A |= data;
	if (z80.A == 0)	z80.F = 0x80;
	else z80.F = 0x00;
}

static inline void xor(BYTE data){
	z80.A ^= data;
	if (z80.A == 0)	z80.F = 0x80;
	else z80.F = 0x00;
}

//--------16bit arithmetic/logical instructions

static inline void add_dbl(BYTE *reg1, BYTE *reg2, unsigned short data){
	// - 0 H C
	unsigned short r1r2 = (*reg1 << 8) + *reg2;
	unsigned short op = r1r2 + data;
	if(z80.F & FLAG_Z) z80.F = 0x80;
	else z80.F = 0;
	if (data < r1r2) z80.F |= 0x10;
 	halfcarry_16bit_update(r1r2, op, ADD);
	*reg1 =( 0xFF00 & op) >> 8;
	*reg2 =( 0x00FF & op);
}

static inline void add_sp_r8(BYTE_S data){
	unsigned short op = z80.SP + data;
	if(z80.SP + data > 0xFFFF || z80.SP + data < 0) z80.F |= 0x10;
 	halfcarry_16bit_update(z80.SP, op, ADD);
	z80.SP = op;
}

static inline void dec_dbl(BYTE *reg1, BYTE *reg2){
	// - - - - 
	unsigned short temp;
	temp = (*reg1 << 8) + *reg2 - 0x01;
	*reg1 =( 0xFF00 & temp) >> 8;
	*reg2 =( 0x00FF & temp);
}
static inline void inc_dbl(BYTE *reg1, BYTE *reg2){
	// - - - -
	unsigned short temp;
	temp = (*reg1 << 8) + *reg2 + 0x01;
	*reg1 =( 0xFF00 & temp) >> 8;
	*reg2 =( 0x00FF & temp);
}


//-----------8bit rotations/shifts and bit instructions

static inline void bit(BYTE bit, BYTE data){
	if(z80.F & 0x10) z80.F = 0x30;
	else z80.F = 0x20;
	if(data & bit) z80.F |= 0x80;
}

static inline void res(BYTE b, BYTE *a) 
{
	*a &= ~(1 << b);
}

static inline void res_hl(BYTE b)
{
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	hl &= ~(1 << b);
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void rl(BYTE *data){
	BYTE F = z80.F & 0x10;
	if ((*data & 0x80) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if(F) *data |= 1;
	if (*data == 0) z80.F |= 0x80;
}

static inline void rl_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	BYTE F = z80.F;
	if ((hl & 0x80) == 0) z80.F = 0;
	else z80.F = 0x10;
	hl <<= 1;
	if(F & 0x10) hl |= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void rla(){
	BYTE F = z80.F;
	if(z80.A & 0x80) z80.F |= 0x10;
	else z80.F = 0;
	z80.A <<= 1;
	if(F & 0x10) z80.A |= 1;
}

static inline void rlc(BYTE *data){
	if ((*data & 0x80) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if(z80.F & 0x10) *data |= 1;
	if (*data == 0) z80.F |= 0x80;
}

static inline void rlc_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	if ((hl & 0x80) == 0) z80.F = 0x00;
	else z80.F = 0x10;
	hl <<= 1;
	if(z80.F & 0x10) hl |= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}
static inline void rlca(){
	BYTE bit7 = z80.A & 0x80;
	z80.A <<= 1;
	if(bit7){
		z80.A |= 1;
		z80.F |= 0x10;
	}
	else z80.F = 0;
}
static inline void rr(BYTE *data){
	BYTE F = z80.F & 0x10;
	if ((*data & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if(F) *data |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

static inline void rr_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	BYTE F = z80.F;
	if ((hl & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if(F & 0x10) hl |= 0x80;
	if (z80.A == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void rra(){
	BYTE F = z80.F;
	if(z80.A & 0x01) z80.F |= 0x10;
	else z80.F = 0;
	z80.A >>= 1;
	if(F & 0x10) z80.A |= 0x80;
}	

static inline void rrc(BYTE *data){
	if ((*data & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if(z80.F & 0x10) *data |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

static inline void rrc_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	if ((hl & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if(z80.F & 0x10) hl |= 0x80;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}


static inline void rrca(){
	BYTE bit0 = z80.A & 0x01;
	z80.A >>= 1;
	if(bit0){
		z80.A |= 0x80;
		z80.F |= 0x10;
	}
	else z80.F = 0;
}

static inline void set(BYTE b, BYTE *a) 
{
	*a |= (1 << b);
}

static inline void set_hl(BYTE b)
{
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	hl |= (1 << b);
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void srl(BYTE *data){
	if((*data & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if (*data == 0) z80.F |= 0x80;
}

static inline void srl_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	if((hl & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void sla(BYTE *data){
	if ((*data & 0x80) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data <<= 1;
	if (*data == 0) z80.F |= 0x80;
}

static inline void sla_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	if ((z80.B & 0x80) == 0) z80.F = 0;
	else z80.F = 0x10;
	z80.B <<= 1;
	if (z80.B == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}

static inline void sra(BYTE *data){
	BYTE F = *data & 0x80;
	if((*data & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	*data >>= 1;
	if (F) z80.B |= 0x80;
	if (*data == 0) z80.F |= 0x80;
}

static inline void sra_hl(){
	BYTE hl = memory_read((z80.H << 8) +z80.L);
	BYTE F = hl & 0x80;
	if((hl & 0x01) == 0) z80.F = 0;
	else z80.F = 0x10;
	hl >>= 1;
	if (F) hl |= 0x80;
	if (hl == 0) z80.F |= 0x80;
	memory_write((z80.H << 8) + z80.L, hl);
}	

static inline void swap(BYTE *data){
	BYTE low_nibble = (*data & 0xF0) >> 4;
	BYTE high_nibble = (*data & 0x0F) << 4;
	*data = high_nibble + low_nibble;
	if(*data == 0) z80.F = 0x80;
	else z80.F = 0;
}

static inline void swap_hl(){
	BYTE hl = memory_read((z80.H << 8) + z80.L);
	BYTE low_nibble = (z80.B & 0xF0) >> 4;
	BYTE high_nibble = (z80.B & 0x0F) << 4;
	z80.B = high_nibble + low_nibble;
	if(z80.B == 0) z80.F = 0x80;
	else z80.F = 0;
	memory_write((z80.H << 8) + z80.L, hl);
}

