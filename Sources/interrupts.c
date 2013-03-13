#include "interrupts.h"

void set_IME(){
	IME = 1;
}

void reset_IME(){
	IME = 0;
}

void handle_interrupts(z80_t *z80){
	BYTE IE;
	BYTE IF;

	IE = memory_read(0xFFFF);
	IF = memory_read(0xFF0F);
	if(IE & IF & 0x01){
		execute_interrupt(V_BLANK, z80);
	}
	else if(IE & IF & 0x02){
		execute_interrupt(LCD_STAT, z80);
	}
	else if (IE & IF & 0x04){
		execute_interrupt(TIMER, z80);
	}
	else if(IE & IF & 0x08){
		execute_interrupt(SERIAL, z80);
	}
	else if(IE & IF & 0x10){
		execute_interrupt(JOYPAD, z80);
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
