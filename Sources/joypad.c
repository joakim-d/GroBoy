#include "joypad.h"

void joypad_update(){
	BYTE joy_stat;
	joy_stat = memory_read(0xFF00);
	joy_stat |= 0xCF;

	//Gestion des touches ici

	memory_write(0xFF00, joy_stat);
}
