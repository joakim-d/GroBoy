#include "timer.h"

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
