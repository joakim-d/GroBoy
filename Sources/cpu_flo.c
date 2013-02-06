int cycles = 0;
switch(opcode){
	case 0x50: //BIT 2,B
		bit((0x01 << 2),z80.B);
		break;
	case 0x51: //BIT 2,C
		bit((0x01 << 2),z80.C);
		break;
	case 0x52: //BIT 2,D
		bit((0x01 << 2),z80.D);
		break;
	case 0x53: //BIT 2,E
		bit((0x01 << 2),z80.E);
		break;
	case 0x54: //BIT 2,H
		bit((0x01 << 2),z80.H);
		break;
	case 0x55: //BIT 2,L
		bit((0x01 << 2),z80.L);
		break;
	case 0x56: //BIT 2,HL
		bit((0x01 << 2),z80.HL);
		break;
	case 0x57: //BIT 2,A
		bit((0x01 << 2),z80.A);
		break;
	case 0x58: //BIT 3,B
		bit((0x01 << 3),z80.B);
		break;
	case 0x59: //BIT 3,C
		bit((0x01 << 3),z80.C);
		break;
	case 0x5A: //BIT 3,D
		bit((0x01 << 3),z80.D);
		break;
	case 0x5B: //BIT 3,E
		bit((0x01 << 3),z80.E);
		break;
	case 0x5C: //BIT 3,H
		bit((0x01 << 3),z80.H);
		break;
	case 0x5D: //BIT 3,L
		bit((0x01 << 3),z80.L);
		break;
	case 0x5E: //BIT 3,HL
		bit((0x01 << 3),z80.HL);
		break;
	case 0x5F: //BIT 3,A
		bit((0x01 << 3),z80.A);
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
		bit((0x01 << 4),z80.HL);
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
		bit((0x01 << 5),z80.HL);
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
		bit((0x01 << 6),z80.HL);
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
		bit((0x01 << 7),z80.HL);
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
		res(0,&z80.HL);
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
		res(1,&z80.HL);
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
		res(2,&z80.HL);
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
		res(3,&z80.HL);
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
		res(4,&z80.HL);
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
		res(5,&z80.HL);
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
		res(6,&z80.HL);
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
		res(7,&z80.HL);
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
		set(0,&z80.HL);
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
		set(1,&z80.HL);
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
		set(2,&z80.HL);
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
		set(3,&z80.HL);
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
		set(4,&z80.HL);
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
		set(5,&z80.HL);
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
		set(6,&z80.HL);
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
		set(7,&z80.HL);
		break;
	case 0xFF: //SET 7,A
		set(7,&z80.A);
		break;	

}


//fonction daa
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

//fonction BIT
static void bit(BYTE a, int b) 
{
        if ((a & (0x01 << b)) == 0)	z80.F |= 0x80;
        else	z80.F &= ~(1 << 7);
	z80.F &= ~(1 << 6);
	z80.F |= 0x20;
}


//fonction SET
static inline char set(int b, BYTE *a) 
{
         a |= (1 << b);
}

//fonction RES
static inline char res(int b, BYTE *a) 
{
         a &= ~(1 << b);
}
