#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
int main(int argc, char * argv[]){
	memory_init(argv[1]);
	cpu_init();
	run();
	
	return 0;
}
