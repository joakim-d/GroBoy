#include "interrupts.h"

void set_IME(){
	IME = 1;
}

void reset_IME(){
	IME = 0;
}

void handle_interrupts(BYTE cycles){
	handle_timer(cycles);
}
static inline void handle_timer(BYTE cycles){
	static unsigned int div_timer = 0;
	static unsigned int tac_timer = 0;
	BYTE timer_control;
	BYTE timer_counter;
	div_timer += cycles;
	if(div_timer % DIVIDER_TIMER_SPEED < div_timer){
		memory_write(0xFF04, memory_read(0xFF04) + 1);
		div_timer %= DIVIDER_TIMER_SPEED;
	}
	timer_control = memory_read(0xFF07);
	if(timer_control & 0x04){
		tac_timer += cycles;
		if(tac_timer % tac_speed[timer_control & 0x03]){
			timer_counter = memory_read(0xFF05);
			if(timer_counter + 1 > 0xFF){
				make_request(TIMER);
				timer_counter = memory_read(0xFF06);
			}
			else timer_counter++;
			memory_write(0xFF05, timer_counter);
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

void execute_interrupt(z80_t *z80, BYTE type){
	BYTE IE;
	BYTE IF;
	
	IME = 0;
	IE = memory_read(0xFFFF);
	IF = memory_read(0xFF0F);
	
	memory_write(z80->SP - 1, (z80->PC & 0xFF00) >> 8);
	memory_write(z80->SP - 2, (z80->PC & 0x00FF));
	z80->SP -= 2;
	

	switch(type){
		case V_BLANK:
			if(IE & IF & 0x01){
				z80->PC = 0x40;
			}
		break;
		case LCD_STAT:
			if(IE & IF & 0x02){
				z80->PC = 0x48;
			}
		break;
		case TIMER:
			if(IE & IF & 0x04){
				z80->PC = 0x50;
			}
		break;
		case SERIAL:
			if(IE & IF & 0x08){
				z80->PC = 0x58;
			}
		break;
		case JOYPAD:
			if(IE & IF & 0x10){
				z80->PC = 0x60;
			}
		break;
	}
}
