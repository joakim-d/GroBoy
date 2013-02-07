#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int main(int argc, char * argv[]){
	memory_init();
	read_rom_info(argv[1]);
	
	return 0;
}
