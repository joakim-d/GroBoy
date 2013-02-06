BYTE_S r8;
switch(op_code){
	//Maxi table d'instructions
	case 0x00:;break;
	case 0x01:
	//LD BC,d16
		ld_reg(z80.B, memory_read(z80.PC));
		ld_reg(z80.C, memory_read(z80.PC));
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
		add(z80.H, z80.B);
		add(z80.L, z80.C);
		break;
		//FLAGS
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
	//STOP
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
		r8 = memory_read(z80.PC);
		z80.PC += r8; 
		z80.PC++;
		break;
	case 0x19:
	//ADD HL,DE
		z80.H += z80.D;
		z80.L += z80.E;
		//FLAGS
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
		r8 = memory_read(z80.PC);
		if(z80.F & 0x80 == 0){
			z80.PC += r8;
			z80.PC++;
		}
		else z80.PC++;
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
		z80.L += 0x01;
		break;
		//FLAGS
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
		r8 = memory_read(z80.PC);
		if(z80.F & 0x80 != 0)
			z80.PC = r8;
		else 
			z80.PC += 1;
		break;
	case 0x29:
	//ADD HL,HL
	z80.H += H;
	z80.L += L;
	//FLAGS
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
	case 0x2E
	//LD L,d8
	ld_reg(z80.L, memory_read(z80.SP));
		break;
	case 0x2F
	//CPL
		cpl();
		break;
	case 0x30:
	//JR NC,r8
	r8 = memory_read(z80.PC);
	if(z80.F & 0x10 == 0){
	z80.PC = r8;
	} else {
		z80.PC ++;
	}

		break;
	case 0x31:
	//LD SP,d16
	ld_reg(z80.SP, memory_read((z80.PC << 8)+z80.PC+1));
	z80.PC++;
		break;
	case 0x32:
	//LD (HL-), A
	ld_mem((z80.H << 8)+z80.L, z80.A);
	z80.L -= 0x01;
	//FLAGS
		break;
	case 0x33:
	//INC SP
	z80.SP += 1;
	if(z80.SP == 0xFF)
		z80.SP = 0;
		break;
	case 0x34:
	//INC (HL)
	memory_write((z80.H << 8) + z80.L, (z80.H << 8)+z80.L + 1);
	//FLAGS
		break;
	case 0x35:
	//DEC (HL)
	memory_write((z80.H << 8) + z80.L, (z80.H << 8)+z80.L -1);
	//FLAGS
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
	r8 = memory_read(z80.PC);
	if(z80.F & 0x10 != 0){
	z80.PC = r8;
	} else {
		z80.PC ++;
	}
		break;
	case 0x39:
	//ADD HL,SP
	BYTE res = (z80.H << 8) + z80.L;
	res += SP;
	z80.H = res & 0xF0;
	z80.L = res & 0x0F;
		break;
		//FLAGS
	case 0x3A:
	//LD A,(HL-)
	ld_reg(z80.A, memory_read((z80.H << 8)+z80.L));
	dec_dbl(z80.H,z80.L);
		break;
	case 0x3B:
	//DEC SP
	z80.SP -= 0x01;
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
	add_a(z80.B);
		break;
	case 0x81:
	//ADD A,C
	add_a(z80.C);
		break;
	case 0x82:
	//ADD A,D
	add_a(z80.D);
		break;
	case 0x83:
	//ADD A,E
	add_a(z80.E);
		break;
	case 0x84:
	//ADD A,H
	add_a(z80.H);
		break;
	case 0x85:
	//ADD A,L
	add_a(z80.L);
		break;
	case 0x86:
	//ADD A,(HL)
	add_a(memory_read((z80.H << 8)+z80.L));
		break;
	case 0x87:
	//ADD A,A
	add_a(z80.A);
		break;
	case 0x88:
	//ADC A,B
	adc_a(z80.B);
		break;
	case 0x89:
	//ADC A,C
	adc_a(z80.C);
		break;
	case 0x8A:
	//ADC A,D
	adc_a(z80.D);
		break;
	case 0x8B:
	//ADC A,E
	adc_a(z80.E);
		break;
	case 0x8C:
	//ADC A,H
	adc_a(z80.H);
		break;
	case 0x8D:
	//ADC A,L
	adc_a(z80.L);
		break;
	case 0x8E:
	//ADC A,(HL)
	adc_a(memory_read((z80.H << 8)+z80.L));
		break;
	case 0x8F:
	//ADC A,A
	adc_a(z80.A);
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
	sbc_a(z80.B);
		break;
	case 0x99:
	//SBC A,C
	sbc_a(z80.C);
		break;
	case 0x9A:
	//SBC A,D
	sbc_a(z80.D);
		break;
	case 0x9B:
	//SBC A,E
	sbc_a(z80.E);
		break;
	case 0x9C:
	//SBC A,H
	sbc_a(z80.H);
		break;
	case 0x9D:
	//SBC A,L
	sbc_a(z80.L);
		break;
	case 0x9E:
	//SBC A,(HL)
	sbc_a(memory_read((z80.H << 8)+z80.L));
		break;
	case 0x9F:
	//SBC A,A
	sbc_a(z80.A);
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
	default:
		exit();
		break;
}

void and(BYTE data){
	z80.A &= data;
	if (z80.A == 0)	z80.F &= 0x80;
	else z80.F &= 0x00;
}
void xor(BYTE data){
	z80.A ^= data;
	if (z80.A == 0)	z80.F &= 0x80;
	else z80.F &= 0x00;
}
void add_a(BYTE data){
	if(z80.A > (0xFF - data)){
	z80.F |= 0x10;
	//carry
		//FLAGS
	} else {
	z80.A += data;
	}
	if(z80.A == 0x00){
		z80.F |= 0x80;
	}
}
void adc_a(BYTE data){
	BYTE carry;
	if(z80.F & 0x10)
		carry = 0x01;
	else 
		carry = 0x00;
	if( z80.A + carry > (0xFF - data)){
	z80.F |= 0x10;
	//carry
		//FLAGS
	} else {
	z80.A += data;
	z80.A += carry;
	}
	if(z80.A == 0x00){
		z80.F |= 0x80;
	}
}

void sub(BYTE data){
	z80.A -= data;
	//FLAGS
	if(z80.A == 0x00){
		z80.F |= 0x80;
	}
}

void sbc_a(BYTE data){
	BYTE carry;
	if(z80.F & 0x10)
		carry = 0x01;
	else 
		carry = 0x00;
	z80.A -= data;
	z80.A -= carry;
	//FLAGS
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
		if(reg2 == 0xff){
			if(reg1 == 0xff){
				reg1 = 0x00;
				reg2 = 0x00;
			} else {
				reg1 += 0x01;
			}
		} else {
			reg2 += 0x01;
		}
}
void inc_smpl(BYTE *reg1){
	reg1+=0x01;
		z80.F &= 0x40;
		if(reg1 == 0x00) z80.F |= 0x80;
	//FLAGS
}

void dec_dbl(BYTE *reg1, BYTE *reg2){
	//FLAGS
		if(reg2 == 0x00){
			if(reg1 == 0x00){
				reg1 = 0xFF;
				reg2 = 0xFF;
			} else {
				reg1 -= 0x01;
			}
		} else {
			reg2 -= 0x01;
		}
}

void dec_smpl(BYTE *reg1){
	reg1 -= 0x01;
	z80.F |= 0x40;
	if(reg1 == 0) z80.F |= 0x80;
	//FLAGS
}
void rlca(){
		z80.A << 1;
		z80.F &= 0x80;
		z80.F &= 0x40;
		z80.F &= 0x20;
		//FLAGS
}
void rrca(){
		z80.A >> 1;
		//FLAGS

}
void stop(){
	int i=0;
}

void rla(){
	z80.A << 1;
	//FLAGS
		//DIFF AVEC RLCA ???
}

void rra(){
		z80.A >> 1;
		//FLAGS

}
void cpl(){
	//Inverse tous les bits de A
	//( == A XOR 0xFF)
	z80.A = z80.A ^ 0xFF;
	//FLAGS
}
void scf(){
	z80.F &= 0x40; 
	z80.F &= 0x20; 
	z80.F |= 0x10; 

}
void ccf(){
	//Inverse le Cary Flag, ainsi que le H flag
	//Clear le N flag
	z80.F ^ 0x30;
	z80.F &= 0x40;
}
void daa(){
	//ajustement pour addition hexa????????????????????
}
