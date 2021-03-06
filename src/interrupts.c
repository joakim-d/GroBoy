#include "interrupts.h"

void interrupts_init(){
	IME = 0;
}

void set_IME(){
	IME = 1;
}

void reset_IME(){
	IME = 0;
}

void handle_interrupts(z80_t *z80){
	BYTE IE;
	BYTE IF;

	if(IME){
		IE = memory_read(0xFFFF);
		IF = memory_read(0xFF0F);
		if(IE & IF & 0x01){
			memory_write(0xFF0F, IF & 0xFE);
			execute_interrupt(V_BLANK, z80);
		}
		else if(IE & IF & 0x02){
			memory_write(0xFF0F, IF & 0xFD);
			execute_interrupt(LCD_STAT, z80);
		}
		else if (IE & IF & 0x04){
			memory_write(0xFF0F, IF & 0xFB);
			execute_interrupt(TIMER, z80);
		}
		else if(IE & IF & 0x08){
			memory_write(0xFF0F, IF & 0xF7);
			execute_interrupt(SERIAL, z80);
		}
		else if(IE & IF & 0x10){
			memory_write(0xFF0F, IF & 0xEF);
			execute_interrupt(JOYPAD, z80);
		}
	}
	else if(is_halted()){
		IE = memory_read(0xFFFF);
		IF = memory_read(0xFF0F);
		if(IE & IF & 0x1F){
			reset_halt();
		}
	}
}

void make_request(BYTE type){
	switch(type){
		case V_BLANK:
			memory_write(0xFF0F, memory_read(0xFF0F) | 0x01);
			break;
		case LCD_STAT:
			memory_write(0xFF0F, memory_read(0xFF0F) | 0x02);
			break;
		case TIMER:
			memory_write(0xFF0F, memory_read(0xFF0F) | 0x04);
			break;
		case SERIAL:
			memory_write(0xFF0F, memory_read(0xFF0F) | 0x08);
			break;
		case JOYPAD:
			memory_write(0xFF0F, memory_read(0xFF0F) | 0x10);
			break;
	}
}

void execute_interrupt(BYTE type, z80_t *z80){
	reset_IME();
	reset_halt();
	memory_write(z80->SP - 1, (z80->PC & 0xFF00) >> 8);
	memory_write(z80->SP - 2, (z80->PC & 0x00FF));
	z80->SP -= 2;

	switch(type){
		case V_BLANK:	
			z80->PC = 0x40;
			break;
		case LCD_STAT:
			z80->PC = 0x48;
			break;
		case TIMER:
			z80->PC = 0x50;
			break;
		case SERIAL:
			z80->PC = 0x58;
			break;
		case JOYPAD:
			z80->PC = 0x60;
			break;
	}
}

int save_interrupt(FILE* file)
{
	int nb=0;
	int nb_elements=1;
	nb = fwrite(&IME,sizeof(BYTE),1,file);
	if(nb!=nb_elements) printf("Error when writing interrupt variables\n");
	return nb;
}

void restore_interrupt(FILE* file)
{
	if(fread(&IME,sizeof(BYTE),1,file) == 0) printf("Error when reading interrupt variables\n");
}
