char echange;
switch(op_code){
	//Maxi table d'instructions
	case 0x00:;break;
	case 0x01:
	//LD BC,**
		z80.B = op_code;
		z80.C = op_code+1;
		op_code += 2;
		break;
	case 0x02:
	//LD (BC),A
	//Décalage à gauche pour 
	//concaténation des valeurs de registres
		addr[(z80.B << 8)+z80.C] = z80.A;	
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
		//FLAGS
		z80.B += 0x01;
		break;
	case 0x05:
	//DEC B
		//FLAGS
		z80.B -= 0x01;
		break;
	case 0x06:
	//LD B,*
		z80.B = op_code;
		op_code++;
		break;
	case 0x07:
	//RLCA
		z80.A << 1;
		//FLAGS
		break;
	case 0x08:
	//EX AF,AF'
		echange = z80.A;
		z80.A = A';
		z80.A' = echange;
		echange = z80.F;
		z80.F = F';
		z80.F' = echange;
		break;
	case 0x09:
	//ADD HL,BC
		add(z80.H, z80.B);
		add(z80.L, z80.C);
		break;
		//FLAGS
	case 0x0A:
	//LD A,(BC)
		z80.A = addr[(z80.B << 8) + z80.C];
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
		z80.C = op_code;
		op_code++;
		break;
	case 0x0F:
	//RRCA 
		z80.A >> 1;
		//FLAGS
		break;
	
	
	default:
		exit();
		break;
}

void add_f(unsigned char a, unsigned char b){
	if(a + b >= 0xff){
		//FLAGS
	} else {
		a += b;
	}
}
