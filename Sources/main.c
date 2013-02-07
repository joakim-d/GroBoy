#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
int main(int argc, char * argv[]){
	unsigned short pc_addr;
	pc_addr = memory_init(argv[1]);
	cpu_init(pc_addr);
	run();
	
	return 0;
}
