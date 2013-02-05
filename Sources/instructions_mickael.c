switch(op_code){
	//Maxi table d'instructions
	case 0x00:;break;
	case 0x01:
	//LD BC,**
		z80.B = memory_read(z80.PC);
		z80.C = memory_read(z80.PC+1);
		z80.PC += 2;
		break;
	case 0x02:
	//LD (BC),A
	//Décalage à gauche pour 
	//concaténation des valeurs de registres
		memory_write((z80.B << 8)+z80.C,z80.A);	
		break;
	case 0x03:
	//INC BC
		if(z80.C == 0xff){
			if(z80.B == 0xff){
				z80.B = 0x00;
				z80.C = 0x00;
			} else {
				z80.B += 0x01;
			}
		} else {
			z80.C += 0x01;
		}
		break;
	case 0x04:
	//INC B
		//FLAGS H !
		z80.B += 0x01;
		z80.F &= 0x70;
		if(z80.B == 0) z80.F |= 0x80;
		break;
	case 0x05:
	//DEC B
		//FLAGS H !
		z80.B -= 0x01;
		z80.F |= 0x70;
		if(z80.B == 0) z80.F |= 0x80;
		break;
	case 0x06:
	//LD B,*
		z80.B = memory_read(z80.PC);
		z80.PC++;
		break;
	case 0x07:
	//RLCA
		z80.A << 1;
		z80.F &= 0x80;
		z80.F &= 0x70;
		z80.F &= 0x60;
		//FLAGS C
		break;
	case 0x08:
	//LD (A16),SP
		memory_write(memory_read(z80.PC) << 8 + memory_read(z80.PC+1), z80.SP);
		break;
	case 0x09:
	//ADD HL,BC
		add(z80.H, z80.B);
		add(z80.L, z80.C);
		break;
		//FLAGS
	case 0x0A:
	//LD A,(BC)
		memory_write((z80.B << 8) + z80.C, z80.A);
		//z80.A = memory_read((z80.B << 8) + z80.C);
		break;
	case 0x0B:
	//DEC BC
		z80.BC = ((z80.B << 8)+z80.C) - 1;
		//FLAGS ? 0000 ?
		break;
	case 0x0C:
	//INC C
		z80.C+= 0x01;
		//FLAGS
		//(add(z80.C, (unsigned char)1);)
		break;
	case 0x0D:
	//DEC C
		z80.C-=0x01;
		//FLAGS
		break;
	case 0x0E:
	//LD C,*
		z80.C = memory_read(z80.PC);
		z80.PC++;
		break;
	case 0x0F:
	//RRCA 
		z80.A >> 1;
		//FLAGS
		break;
	case 0x10:
	//STOP
		exit(0);
		break;
	case 0x11:
	//LD DE, d16
		z80.D = memory_read(z80.PC);
		z80.E = memory_read(z80.PC+1);
		z80.PC+=2;
		break;
	case 0x12:
	//LD (DE), A
		memory_write((z80.D << 8) + z80.E, z80.A);
		break;
	case 0x13:
	//INC DE
		if(z80.E == 0xff){
			if(z80.D == 0xff){
				z80.D = 0x00;
				z80.E = 0x00;
			} else {
				z80.D += 0x01;
			}
		} else {
			z80.E += 0x01;
		}
		break;
	case 0x14:
	//INC D
		z80.D+=0x01;
		//FLAGS
		break;
	case 0x15:
	//DEC D
		z80.D-=0x01;
		break;
		//FLAGS
	case 0x16:
	//LD D,d8
		z80.D = memory_read(z80.PC);
		z80.PC++;
		break;
	case 0x17:
	//RLA
		z80.A << 1;
		break;
		//FLAGS
		//DIFF AVEC RLCA ???
	case 0x18:
	//JR r8
		z80.PC = memory_read(z80.PC);
		z80.PC++;
		break;
		//Moins de Bytes que JP
		//???????????????????????????????
	case 0x19:
	//ADD HL,DE
		z80.H += z80.D;
		z80.L += z80.E;
		//FLAGS
		break;
	case 0x1A:
	//LD A,(DE)
		z80.A = memory_read((z80.D << 8) + z80.E);
		break;
	case 0x1B:
	//DEC DE
		if(z80.E == 0x00){
			if(z80.D == 0x00){
				z80.D = 0xFF;
				z80.E = 0xFF;
			} else {
				z80.D -= 0x01;
			}
		} else {
			z80.E -= 0x01;
		}
		break;
	case 0x1C:
	//INC E
		z80.E += 0x01;
		//FLAGS
		break;
	case 0x1D:
	//DEC E
		z80.E -= 0x01;
		//FLAGS
		break;
	case 0x1E:
	//LD E,d8
		z80.E = memory_read(z80.PC);
		z80.PC++;
		break;
	case 0x1F:
	//RRA
		z80.A >> 1;
		break;
		//FLAGS
	case 0x20:
	//JR NZ, R8
		if(z80.F & 0x80 == 0)
			z80.PC = memory_read(z80.PC);
		else z80.PC++;
		break;
	case 0x21:
	//LD HL, d16
		z80.H = memory_read(z80.PC);
		z80.L = memory_read(z80.PC+1);
		z80.PC += 2;
		break;
	case 0x22:
	//LD (HL+), A
		memory_write((z80.H << 8) + z80.L, z80.A);
		z80.L += 0x01;
		break;
		//FLAGS
	
	
	default:
		exit();
		break;
}

