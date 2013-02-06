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

void run(){
	int interrupt_period;
	int counter;
	counter=interrupt_period;

	BYTE op_code;
	for(;;)
	{
		op_code=memory_read(z80.PC++);
		//counter-=cycles[op_code];

		switch(op_code){
			//Maxi table d'instructions

			case 0x00:
				//NOP
				counter=0;
				break;
			case 0x01:
				//LD BC,d16
				ld_reg(z80.B, memory_read(z80.PC));
				ld_reg(z80.C, memory_read(z80.PC+1));
				z80.PC += 2;
				break;
			case 0x02:
				//LD (BC),A
				//Décalage à gauche pour 
				//concaténation des valeurs de registres
				ld_mem((z80.B << 8)+z80.C, z80.A);
				break;
			case 0x03:
				//INC BC
				inc_dbl(z80.B,z80.C);
				break;
			case 0x04:
				//INC B
				inc_smpl(z80.B);
				break;
			case 0x05:
				//DEC B
				dec_smpl(z80.B);
				break;
			case 0x06:
				//LD B,*
				ld_reg(z80.B, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x07:
				//RLCA
				rlca();
				break;
			case 0x08:
				//LD (A16),SP
				ld_mem(memory_read(z80.PC) << 8 + memory_read(z80.PC+1), z80.SP);
				break;
			case 0x09:
				//ADD HL,BC
				add_dbl(z80.H,z80.L, (z80.B << 8) + z80.C);
				break;
			case 0x0A:
				//LD A,(BC)
				ld_reg(z80.A, memory_read((z80.B << 8)+z80.C));
				break;
			case 0x0B:
				//DEC BC
				dec_dbl(z80.B,z80.C);
				break;
			case 0x0C:
				//INC C
				inc_smpl(z80.C);
				break;
			case 0x0D:
				//DEC C
				dec_smpl(z80.C);
				break;
			case 0x0E:
				//LD C,d8
				ld_reg(z80.C, memory_read(z80.PC));
				z80.PC++;
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
				ld_reg(z80.D, memory_read(z80.PC));
				ld_reg(z80.E, memory_read(z80.PC+1));
				z80.PC+=2;
				break;
			case 0x12:
				//LD (DE), A
				ld_mem((z80.D << 8)+z80.E, z80.A);
				break;
			case 0x13:
				//INC DE
				inc_dbl(z80.D,z80.E);
				break;
			case 0x14:
				//INC D
				inc_smpl(z80.D);
				break;
			case 0x15:
				//DEC D
				dec_smpl(z80.D);
				break;
			case 0x16:
				//LD D,d8
				ld_reg(z80.D, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x17:
				//RLA
				rla();
				break;
			case 0x18:
				//JR r8
				jr((BYTE_S)memory_read(z80.PC));
				break;
			case 0x19:
				//ADD HL,DE
				add_dbl(z80.H,z80.L, (z80.D << 8) + z80.E);
				break;
			case 0x1A:
				//LD A,(DE)
				ld_reg(z80.A, memory_read((z80.D << 8)+z80.E));
				break;
			case 0x1B:
				//DEC DE
				dec_dbl(z80.D,z80.E);
				break;
			case 0x1C:
				//INC E
				inc_smpl(z80.E);
				break;
			case 0x1D:
				//DEC E
				dec_smpl(z80.E);
				break;
			case 0x1E:
				//LD E,d8
				ld_reg(z80.E, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x1F:
				//RRA
				rra();
				break;
			case 0x20:
				//JR NZ, R8
				jr_n_cond(0x80,memory_read(z80.PC));
				break;
			case 0x21:
				//LD HL, d16
				ld_reg(z80.H, memory_read(z80.PC));
				ld_reg(z80.L, memory_read(z80.PC+1));
				z80.PC += 2;
				break;
			case 0x22:
				//LD (HL+), A
				ld_mem((z80.H << 8)+z80.L, z80.A);
				inc_dbl(z80.H,z80.L);
				break;
			case 0x23:
				//INC HL
				inc_dbl(z80.H,z80.L);
				break;
			case 0x24:
				//INC H
				inc_smpl(z80.H);
				break;
			case 0x25:
				//DEC H
				dec_smpl(z80.H);
				break;
			case 0x26:
				//LD H,d8
				ld_reg(z80.H, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x27:
				//DAA
				daa();
				break;
			case 0x28:
				//JR Z,r8
				jr_cond(0x80,memory_read(z80.PC));
				break;
			case 0x29:
				//ADD HL,HL
				add_dbl(z80.H,z80.L, (z80.H << 8) + z80.L);
				break;
			case 0x2A:
				//LD A,(HL+)
				ld_reg(z80.A, memory_read((z80.H << 8)+z80.L));
				inc_dbl(z80.H,z80.L);
				break;
			case 0x2B:
				//DEC HL
				dec_dbl(z80.H, z80.L);
				break;
			case 0x2C:
				//INC L
				inc_smpl(z80.L);
				break;
			case 0x2D:
				//DEC L
				dec_smpl(z80.L);
				break;
			case 0x2E:
				//LD L,d8
				ld_reg(z80.L, memory_read(z80.SP));
				break;
			case 0x2F:
				//CPL
				cpl();
				break;
			case 0x30:
				//JR NC,r8
				jr_n_cond(0x10,memory_read(z80.PC));
				break;
			case 0x31:
				//LD SP,d16
				ld_reg(z80.SP, memory_read((z80.PC << 8)+z80.PC+1));
				z80.PC++;
				break;
			case 0x32:
				//LD (HL-), A
				ld_mem((z80.H << 8)+z80.L, z80.A);
				dec_dbl(z80.H,z80.L);
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
				memory_write((z80.H << 8) + z80.L, (z80.H << 8)+z80.L -1);
				//FLAGS ====================================================================================
				break;
			case 0x36:
				//LD (HL),d8
				ld_mem((z80.H << 8)+z80.L, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x37:
				//SCF
				scf();
				break;
			case 0x38:
				//JR C,r8
				jr_cond(0x10,memory_read(z80.PC));
				break;
			case 0x39:
				//ADD HL,SP
				add_dbl(z80.H,z80.L, z80.SP);
				break;
			case 0x3A:
				//LD A,(HL-)
				ld_reg(z80.A, memory_read((z80.H << 8)+z80.L));
				dec_dbl(z80.H,z80.L);
				break;
			case 0x3B:
				//DEC SP
				dec_sp();
				break;
			case 0x3C:
				//INC A
				inc_smpl(z80.A);
				break;
			case 0x3D:
				//DEC A
				dec_smpl(z80.A);
				break;
			case 0x3E:
				//LD A,d8
				ld_reg(z80.A, memory_read(z80.PC));
				z80.PC++;
				break;
			case 0x3F:
				//CCF
				ccf();
				break;
			case 0x40:
				//LD B,B
				ld_reg(z80.B,z80.B);
				break;
			case 0x41:
				//LD B,C
				ld_reg(z80.B,z80.C);
				break;
			case 0x42:
				//LD B,D
				ld_reg(z80.B,z80.D);
				break;
			case 0x43:
				//LD B,E
				ld_reg(z80.B,z80.E);
				break;
			case 0x44:
				//LD B,H
				ld_reg(z80.B,z80.H);
				break;
			case 0x45:
				//LD B,L
				ld_reg(z80.B,z80.L);
				break;
			case 0x46:
				//LD B,(HL)
				z80.B = memory_read((z80.H << 8) + z80.L);
				ld_reg(z80.B, memory_read((z80.H << 8)+z80.L));
				break;
			case 0x47:
				//LD B,A
				ld_reg(z80.B,z80.A);
				break;
			case 0x48:
				//LD C,B
				ld_reg(z80.C,z80.B);
				break;
			case 0x49:
				//LD C,C
				ld_reg(z80.C,z80.C);
				break;
			case 0x4A:
				//LD C,D
				ld_reg(z80.C,z80.D);
				break;
			case 0x4B:
				//LD C,E
				ld_reg(z80.C,z80.E);
				break;
			case 0x4C:
				//LD C,H
				ld_reg(z80.C,z80.H);
				break;
			case 0x4D:
				//LD C,L
				ld_reg(z80.C,z80.L);
				break;
			case 0x4E:
				//LD C,(HL)
				ld_reg(z80.C, memory_read((z80.H << 8)+z80.L));
				break;
			case 0x4F:
				//LD C,A
				ld_reg(z80.C,z80.A);
				break;
			case 0x50:
				//LD D,B
				ld_reg(z80.D,z80.B);
				break;
			case 0x51:
				//LD D,C
				ld_reg(z80.D,z80.C);
				break;
			case 0x52:
				//LD D,D
				ld_reg(z80.D,z80.D);
				break;
			case 0x53:
				//LD D,E
				ld_reg(z80.D,z80.E);
				break;
			case 0x54:
				//LD D,H
				ld_reg(z80.D,z80.H);
				break;
			case 0x55:
				//LD D,L
				ld_reg(z80.D,z80.L);
				break;
			case 0x56:
				//LD D,(HL)
				ld_reg(z80.D, memory_read((z80.H << 8) + z80.L));
				break;
			case 0x57:
				//LD D,A
				ld_reg(z80.D,z80.A);
				break;
			case 0x58:
				//LD E,B
				ld_reg(z80.E,z80.B);
				break;
			case 0x59:
				//LD E,C
				ld_reg(z80.E,z80.C);
				break;
			case 0x5A:
				//LD E,D
				ld_reg(z80.E,z80.D);
				break;
			case 0x5B:
				//LD E,E
				ld_reg(z80.E,z80.E);
				break;
			case 0x5C:
				//LD E,H
				ld_reg(z80.E,z80.H);
				break;
			case 0x5D:
				//LD E,L
				ld_reg(z80.E,z80.L);
				break;
			case 0x5E:
				//LD E,(HL)
				ld_reg(memory_read((z80.H << 8)+z80.L));
				break;
			case 0x5F:
				//LD E,A
				ld_reg(z80.E,z80.A);
				break;
			case 0x60:
				//LD H,B
				ld_reg(z80.H,z80.B);
				break;
			case 0x61:
				//LD H,C
				ld_reg(z80.H,z80.C);
				break;
			case 0x62:
				//LD H,D
				ld_reg(z80.H,z80.D);
				break;
			case 0x63:
				//LD H,E
				ld_reg(z80.H,z80.E);
				break;
			case 0x64:
				//LD H,H
				ld_reg(z80.H,z80.H);
				break;
			case 0x65:
				//LD H,L
				ld_reg(z80.H,z80.L);
				break;
			case 0x66:
				//LD H,(HL)
				ld_reg(z80.H,memory_read((z80.H << 8) + z80.L));
				break;
			case 0x67:
				//LD H,A
				ld_reg(z80.H,z80.A);
				break;
			case 0x68:
				//LD L,B
				ld_reg(z80.L,z80.B);
				break;
			case 0x69:
				//LD L,C
				ld_reg(z80.L,z80.C);
				break;
			case 0x6A:
				//LD L,D
				ld_reg(z80.L,z80.D);
				break;
			case 0x6B:
				//LD L,E
				ld_reg(z80.L,z80.E);
				break;
			case 0x6C:
				//LD L,H
				ld_reg(z80.L,z80.H);
				break;
			case 0x6D:
				//LD L,L
				ld_reg(z80.L,z80.L);
				break;
			case 0x6E:
				//LD L,(HL)
				ld_reg(z80.L,memory_read((z80.H << 8)+z80.L));
				break;
			case 0x6F:
				//LD L,A
				ld_reg(z80.L,z80.A);
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
				exit(0);
				break;
			case 0x77:
				//LD (HL),A
				ld_mem((z80.H << 8)+z80.L,z80.A);
				break;
			case 0x78:
				//LD A,B
				ld_reg(z80.A,z80.B);
				break;
			case 0x79:
				//LD A,C
				ld_reg(z80.A,z80.C);
				break;
			case 0x7A:
				//LD A,D
				ld_reg(z80.A,z80.D);
				break;
			case 0x7B:
				//LD A,E 
				ld_reg(z80.A,z80.E);
				break;
			case 0x7C:
				//LD A,H
				ld_reg(z80.A,z80.H);
				break;
			case 0x7D:
				//LD A,L
				ld_reg(z80.A,z80.L);
				break;
			case 0x7E:
				//LD A,(HL)
				ld_reg(z80.A,memory_read((z80.H << 8)+z80.L));
				break;
			case 0x7F:
				//LD A,A
				ld_reg(z80.A,z80.A);
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
	if(!z80.F & cond){
		z80.PC = (memory_read(z80.SP + 1) << 8) + memory_read(z80.SP);;
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

void add(BYTE data){
	z80.F = 0;
	if (z80.A + data == 0) z80.F |= 0x80;
	if (z80.A + data > 0xFF) z80.F |= 0x10;
	if (z80.A <= 0xF && z80.A + data > 0xF) z80.F |= 0x20;
	z80.A = z80.A + data;
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
	z80.A = z80.A + data + c;
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

	if (z80.A - data - c == 0) z80.F |= 0x80;
	if (z80.A - data - c < 0) z80.F |= 0x10;
	if(z80.A > 0xF && z80.A - data - c <= 0xF) z80.F |= 0x20;
	z80.A = (z80.A - data - c);
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
	if(z80.SP + data > 0xFFFF || z80.SP + data < 0) z80.F |= 0x10;
	if((z80.SP > 0xFF && z80.SP + data <= 0xFF) || (z80.SP <= 0xFF && z80.SP + data > 0xFF)) z80.F |= 0x20;
	Z80.SP += data;
}

void di(){

}
void ld_hl_sp_p_r8(){
	BYTE_S r8 = memory_read(z80.PC);
	unsigned short a16 = (z80.H << 8) + z80.L;
	if(a16 + z80.SP + r8 > 0xFFFF || a16 + z80.SP + r8 < 0) z80.F |= 0x10;
	if((a16 > 0xFF && a16 + z80.SP + r8 <= 0xFF) || (a16 <= 0xFF && a16 + z80.SP + r8) > 0xFF) z80.F |= 0x20;
	Z80.H = ((Z80.SP + r8) & 0xFF00) >> 8 ;
	Z80.L = ((Z80.SP + r8) & 0xFF);
	z80.PC++;
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
void xor(BYTE data){
	z80.A ^= data;
	if (z80.A == 0)	z80.F &= 0x80;
	else z80.F &= 0x00;
}
void ld_reg(BYTE *reg1, BYTE data){
	//LD reg1, reg2
	//LD reg1, (reg2)
	reg1 = data;
}
void ld_mem(BYTE reg1, BYTE reg2){
	//LD (reg1),reg2
	//LD (reg1), d
	memory_write(reg1, reg2);

}
void inc_dbl(BYTE *reg1, BYTE *reg2){
	// - - - -
	short temp;
	temp = (*reg1 << 8) + *reg2 + 0x01;
	*reg1 =( 0xFF00 & data2) >> 8;
	*reg2 =( 0x00FF & data2);
}
void inc_smpl(BYTE *reg1){
	// Z 0 H -
	*reg1+=0x01;
	if(reg1 == 0x00) z80.F |= 0x80;
	z80.F &= ~(0x40);
	if(reg1 > 0xF) z80.F |= 0x20;
	else z80.F ~(0x20);
}

void inc_at(short addr){
	// Z 0 H -
	memory_write(addr, memory_read(addr) + 1);
	if(memory_read(addr) == 0)
		z80.F |= 0x80;
	else 
		z80.F &= ~(0x80);
	z80.F &= ~(0x40);
	if(memory_read(addr) > 0xF)
		z80.F |= 0x20;
	else
		z80.F &= ~(0x20);
}

void dec_at(short addr){
	// Z 1 H -
	memory_write(addr, memory_read(addr) - 1);
	if(memory_read(addr) == 0)
		z80.F |= 0x80;
	else 
		z80.F &= ~(0x80);
	z80.F |= 0x40;
	if(memory_read(addr) > 0xF)
		z80.F |= 0x20;
	else
		z80.F &= ~(0x20);
}

void dec_sp(){
	// - - - -
	z80.SP -= 1;
}

void inc_sp(){
	// - - - -
	z80.SP += 1;
}

void dec_dbl(BYTE *reg1, BYTE *reg2){
	// - - - - 
	short temp;
	temp = (*reg1 << 8) + *reg2 - 0x01;
	*reg1 =( 0xFF00 & data2) >> 8;
	*reg2 =( 0x00FF & data2);
}

void dec_smpl(BYTE *reg1){
	// Z 1 H -
	*reg1 -= 0x01;
	if(reg1 == 0) z80.F |= 0x80;
	z80.F |= 0x40;
	if(reg1 > 0xF)z80.F |= 0x20;
	else z80.F &= ~(0x20);
}
void rlca(){
	// 0 0 0 C
	// bit 7 => bit 0
	// bit 7 => carry
	BYTE bit7;
	if(z80.A & 0x80){
		bit7 = 0x01;
	}
	else  {
		bit7 = 0x00;
	}
	z80.A << 1;
	if(bit7 == 0x01){
		z80.A |= 0x01;
		z80.F |= 0x10;
		}
	else {
		z80.A &= ~(0x01);
		z80.F &= ~(0x10);

		}
	z80.F &= ~(0x80); //Z
	z80.F &= ~(0x40); //N
	z80.F &= ~(0x20); //H
}
void rrca(){
	// 0 0 0 C
	// bit 0 => bit 7
	// bit 0 => carry
	BYTE bit0;
	if(z80.A & 0x80){
		bit0 = 0x01;
	}
	else  {
		bit0 = 0x00;
	}
	z80.A >> 1;
	if(bit0 == 0x01){
		z80.A |= 0x80;
		z80.F |= 0x10;
		}
	else {
		z80.A &= ~(0x80);
		z80.F &= ~(0x10);

		}
	z80.F &= ~(0x80); //Z
	z80.F &= ~(0x40); //N
	z80.F &= ~(0x20); //H

}
void stop(){
	int i=0;
}

void rla(){
	// 0 0 0 C
	// Carry => bit 0
	// bit 7 => carry
	BYTE carry;
	if(z80.F & 0x10){
		carry = 0x01;
	}
	else  {
		carry = 0x00;
	}
	z80.A << 1;
	if(carry == 0x01)
		z80.A |= 0x01;
	else 
		z80.A &= ~(0x01);
	if(z80.A & 0x80)
		z80.F |= 0x10;
	else
		z80.F &= ~(0x10);
	z80.F &= ~(0x80); //Z
	z80.F &= ~(0x40); //N
	z80.F &= ~(0x20); //H
}

void rra(){
	// 0 0 0 C
	// Carry => bit 7
	// bit 0 => carry
	BYTE carry;
	if(z80.F & 0x10){
		carry = 0x01;
	}
	else  {
		carry = 0x00;
	}
	z80.A >> 1;
	if(carry == 0x01)
		z80.A |= 0x80;
	else 
		z80.A &= ~(0x80);
	if(z80.A & 0x01)
		z80.F |= 0x10;
	else
		z80.F &= ~(0x10);
	z80.F &= ~(0x80); //Z
	z80.F &= ~(0x40); //N
	z80.F &= ~(0x20); //H
}	
void cpl(){
	// - 1 1 -
	//Inverse tous les bits de A
	//( == A XOR 0xFF)
	z80.A = z80.A ^ 0xFF;
	z80.F |= 0x40; // N
	z80.F |= 0x20; // H
}
void scf(){
	// - 0 0 1
	z80.F &= ~(0x40); // N 
	z80.F &= ~(0x20); // H
	z80.F |= 0x10; // C

}
void ccf(){
	// - 0 0 C
	//Inverse le Cary Flag
	//Clear le N flag et le H flag
	z80.F ^ 0x10; // C
	z80.F &= ~(0x40); // N
	z80.F &= ~(0x20); // H
}
void daa(){
	//ajustement pour addition hexa????????????????????
}
void add_dbl(BYTE *reg1, BYTE *reg2, short data){
	// - 0 H C
	short data2 = (*reg1 << 8) + *reg2 + data;
	*reg1 =( 0xFF00 & data2) >> 8;
	*reg2 =( 0x00FF & data2);
	z80.F &= ~(0x40); // N	

}

void jr(BYTE_S d){
	// - - - -
	//Jump Relatif
	z80.PC += d; 
	z80.PC++;
}

void jr_cond(BYTE cond, BYTE_S d){
	// - - - -
	if(z80.F & cond != 0)
		z80.PC += d;
	else z80.PC += 1;
}

void jr_n_cond(BYTE cond, BYTE_S d){
	// - - - -
	if(z80.F & cond == 0)
		z80.PC = d;
	else z80.PC += 1;
}

