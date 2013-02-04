int cycles = 0;
switch(opcode){
	case 0x50:
		if((z80.B & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x51:
		if((z80.C & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x52:
		if((z80.D & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x53:
		if((z80.E & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x54:
		if((z80.H & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x55:
		if((z80.L & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x56:
		if((z80.HL & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x57:
		if((z80.A & (0x01 << 2)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x58:
		if((z80.B & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x59:
		if((z80.C & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x5A:
		if((z80.D & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x5B:
		if((z80.E & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x5C:
		if((z80.H & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x5D:
		if((z80.L & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x5E:
		if((z80.HL & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x5F:
		if((z80.A & (0x01 << 3)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x60:
		if((z80.B & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x61:
		if((z80.C & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x62:
		if((z80.D & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x63:
		if((z80.E & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x64:
		if((z80.H & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x65:
		if((z80.L & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x66:
		if((z80.HL & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x67:
		if((z80.A & (0x01 << 4)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x68:
		if((z80.B & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x69:
		if((z80.C & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x6A:
		if((z80.D & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x6B:
		if((z80.E & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x6C:
		if((z80.H & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x6D:
		if((z80.L & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x6E:
		if((z80.HL & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x6F:
		if((z80.A & (0x01 << 5)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x70:
		if((z80.B & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x71:
		if((z80.C & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x72:
		if((z80.D & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x73:
		if((z80.E & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x74:
		if((z80.H & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x75:
		if((z80.L & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x76:
		if((z80.HL & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x77:
		if((z80.A & (0x01 << 6)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x78:
		if((z80.B & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x79:
		if((z80.C & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x7A:
		if((z80.D & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x7B:
		if((z80.E & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x7C:
		if((z80.H & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x7D:
		if((z80.L & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x7E:
		if((z80.HL & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 16;
		break;
	case 0x7F:
		if((z80.A & (0x01 << 7)) == 0)
		{
			z80.F |= 0x80;
		} 
		else
		{
			z80.F &= ~(1 << 7);
		} 	
		z80.F &= ~(1 << 6);
		z80.F |= 0x20;
		cycles = 8;
		break;
	case 0x80:
		z80.B &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x81:
		z80.C &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x82:
		z80.D &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x83:
		z80.E &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x84:
		z80.H &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x85:
		z80.L &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x86:
		z80.HL &= ~(1 << 0);
		cycles = 16;
		break;
	case 0x87:
		z80.A &= ~(1 << 0);
		cycles = 8;
		break;
	case 0x88:
		z80.B &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x89:
		z80.C &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x8A:
		z80.D &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x8B:
		z80.E &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x8C:
		z80.H &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x8D:
		z80.L &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x8E:
		z80.HL &= ~(1 << 1);
		cycles = 16;
		break;
	case 0x8F:
		z80.A &= ~(1 << 1);
		cycles = 8;
		break;
	case 0x90:
		z80.B &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x91:
		z80.C &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x92:
		z80.D &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x93:
		z80.E &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x94:
		z80.H &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x95:
		z80.L &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x96:
		z80.HL &= ~(1 << 2);
		cycles = 16;
		break;
	case 0x97:
		z80.A &= ~(1 << 2);
		cycles = 8;
		break;
	case 0x98:
		z80.B &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x99:
		z80.C &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x9A:
		z80.D &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x9B:
		z80.E &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x9C:
		z80.H &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x9D:
		z80.L &= ~(1 << 3);
		cycles = 8;
		break;
	case 0x9E:
		z80.HL &= ~(1 << 3);
		cycles = 16;
		break;
	case 0x9F:
		z80.A &= ~(1 << 3);
		cycles = 8;
		break;
	case 0xA0:
		z80.B &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA1:
		z80.C &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA2:
		z80.D &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA3:
		z80.E &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA4:
		z80.H &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA5:
		z80.L &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA6:
		z80.HL &= ~(1 << 4);
		cycles = 16;
		break;
	case 0xA7:
		z80.A &= ~(1 << 4);
		cycles = 8;
		break;
	case 0xA8:
		z80.B &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xA9:
		z80.C &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xAA:
		z80.D &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xAB:
		z80.E &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xAC:
		z80.H &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xAD:
		z80.L &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xAE:
		z80.HL &= ~(1 << 5);
		cycles = 16;
		break;
	case 0xAF:
		z80.A &= ~(1 << 5);
		cycles = 8;
		break;
	case 0xB0:
		z80.B &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB1:
		z80.C &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB2:
		z80.D &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB3:
		z80.E &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB4:
		z80.H &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB5:
		z80.L &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB6:
		z80.HL &= ~(1 << 6);
		cycles = 16;
		break;
	case 0xB7:
		z80.A &= ~(1 << 6);
		cycles = 8;
		break;
	case 0xB8:
		z80.B &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xB9:
		z80.C &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xBA:
		z80.D &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xBB:
		z80.E &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xBC:
		z80.H &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xBD:
		z80.L &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xBE:
		z80.HL &= ~(1 << 7);
		cycles = 16;
		break;
	case 0xBF:
		z80.A &= ~(1 << 7);
		cycles = 8;
		break;
	case 0xC0:
		z80.B |= (1 << 0);
		cycles = 8;
		break;
	case 0xC1:
		z80.C |= (1 << 0);
		cycles = 8;
		break;
	case 0xC2:
		z80.D |= (1 << 0);
		cycles = 8;
		break;
	case 0xC3:
		z80.E |= (1 << 0);
		cycles = 8;
		break;
	case 0xC4:
		z80.H |= (1 << 0);
		cycles = 8;
		break;
	case 0xC5:
		z80.L |= (1 << 0);
		cycles = 8;
		break;
	case 0xC6:
		z80.HL |= (1 << 0);
		cycles = 16;
		break;
	case 0xC7:
		z80.A |= (1 << 0);
		cycles = 8;
		break;
	case 0xC8:
		z80.B |= (1 << 1);
		cycles = 8;
		break;
	case 0xC9:
		z80.C |= (1 << 1);
		cycles = 8;
		break;
	case 0xCA:
		z80.D |= (1 << 1);
		cycles = 8;
		break;
	case 0xCB:
		z80.E |= (1 << 1);
		cycles = 8;
		break;
	case 0xCC:
		z80.H |= (1 << 1);
		cycles = 8;
		break;
	case 0xCD:
		z80.L |= (1 << 1);
		cycles = 8;
		break;
	case 0xCE:
		z80.HL |= (1 << 1);
		cycles = 16;
		break;
	case 0xCF:
		z80.A |= (1 << 1);
		cycles = 8;
		break;
	case 0xD0:
		z80.B |= (1 << 2);
		cycles = 8;
		break;
	case 0xD1:
		z80.C |= (1 << 2);
		cycles = 8;
		break;
	case 0xD2:
		z80.D |= (1 << 2);
		cycles = 8;
		break;
	case 0xD3:
		z80.E |= (1 << 2);
		cycles = 8;
		break;
	case 0xD4:
		z80.H |= (1 << 2);
		cycles = 8;
		break;
	case 0xD5:
		z80.L |= (1 << 2);
		cycles = 8;
		break;
	case 0xD6:
		z80.HL |= (1 << 2);
		cycles = 16;
		break;
	case 0xD7:
		z80.A |= (1 << 2);
		cycles = 8;
		break;
	case 0xD8:
		z80.B |= (1 << 3);
		cycles = 8;
		break;
	case 0xD9:
		z80.C |= (1 << 3);
		cycles = 8;
		break;
	case 0xDA:
		z80.D |= (1 << 3);
		cycles = 8;
		break;
	case 0xDB:
		z80.E |= (1 << 3);
		cycles = 8;
		break;
	case 0xDC:
		z80.H |= (1 << 3);
		cycles = 8;
		break;
	case 0xDD:
		z80.L |= (1 << 3);
		cycles = 8;
		break;
	case 0xDE:
		z80.HL |= (1 << 3);
		cycles = 16;
		break;
	case 0xDF:
		z80.A |= (1 << 3);
		cycles = 8;
		break;
	case 0xE0:
		z80.B |= (1 << 4);
		cycles = 8;
		break;
	case 0xE1:
		z80.C |= (1 << 4);
		cycles = 8;
		break;
	case 0xE2:
		z80.D |= (1 << 4);
		cycles = 8;
		break;
	case 0xE3:
		z80.E |= (1 << 4);
		cycles = 8;
		break;
	case 0xE4:
		z80.H |= (1 << 4);
		cycles = 8;
		break;
	case 0xE5:
		z80.L |= (1 << 4);
		cycles = 8;
		break;
	case 0xE6:
		z80.HL |= (1 << 4);
		cycles = 16;
		break;
	case 0xE7:
		z80.A |= (1 << 4);
		cycles = 8;
		break;
	case 0xE8:
		z80.B |= (1 << 5);
		cycles = 8;
		break;
	case 0xE9:
		z80.C |= (1 << 5);
		cycles = 8;
		break;
	case 0xEA:
		z80.D |= (1 << 5);
		cycles = 8;
		break;
	case 0xEB:
		z80.E |= (1 << 5);
		cycles = 8;
		break;
	case 0xEC:
		z80.H |= (1 << 5);
		cycles = 8;
		break;
	case 0xED:
		z80.L |= (1 << 5);
		cycles = 8;
		break;
	case 0xEE:
		z80.HL |= (1 << 5);
		cycles = 16;
		break;
	case 0xEF:
		z80.A |= (1 << 5);
		cycles = 8;
		break;
	case 0xF0:
		z80.B |= (1 << 6);
		cycles = 8;
		break;
	case 0xF1:
		z80.C |= (1 << 6);
		cycles = 8;
		break;
	case 0xF2:
		z80.D |= (1 << 6);
		cycles = 8;
		break;
	case 0xF3:
		z80.E |= (1 << 6);
		cycles = 8;
		break;
	case 0xF4:
		z80.H |= (1 << 6);
		cycles = 8;
		break;
	case 0xF5:
		z80.L |= (1 << 6);
		cycles = 8;
		break;
	case 0xF6:
		z80.HL |= (1 << 6);
		cycles = 16;
		break;
	case 0xF7:
		z80.A |= (1 << 6);
		cycles = 8;
		break;
	case 0xF8:
		z80.B |= (1 << 7);
		cycles = 8;
		break;
	case 0xF9:
		z80.C |= (1 << 7);
		cycles = 8;
		break;
	case 0xFA:
		z80.D |= (1 << 7);
		cycles = 8;
		break;
	case 0xFB:
		z80.E |= (1 << 7);
		cycles = 8;
		break;
	case 0xFC:
		z80.H |= (1 << 7);
		cycles = 8;
		break;
	case 0xFD:
		z80.L |= (1 << 7);
		cycles = 8;
		break;
	case 0xFE:
		z80.HL |= (1 << 7);
		cycles = 16;
		break;
	case 0xFF:
		z80.A |= (1 << 7);
		cycles = 8;
		break;	

}

//fonction BIT
static void bit(char a, int b) 
{
        if ((a & (0x01 << b)) == 0)	z80.F |= 0x80;
        else	z80.F &= ~(1 << 7);
	z80.F &= ~(1 << 6);
	z80.F |= 0x20;
}


//fonction SET
static inline char set(char a, int b) 
{
        return a |= (1 << b);
}

//fonction RES
static inline char res(char a, int b) 
{
        return a &= ~(1 << b);
}
