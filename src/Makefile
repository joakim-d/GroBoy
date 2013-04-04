SRC= cpu.c gpu.c memory.c main.c interrupts.c timer.c sound.c joypad.c blip_buf.c
CC=gcc
CFLAGS = -std=c99 -lSDL -Wall -O3 -fomit-frame-pointer
GFLAGS = -std=c99 -lSDL -Wall -O0 -g -fno-inline
EXEC= groboy
OBJ= $(SRC: .c=.o)
LIBRARY= `sdl-config --cflags --libs` -lSDLmain -lSDL -I /auto_home/fvinard/LIB/include -L /auto_home/fvinard/LIB/lib
all:
	echo "make, trois options possibles : groboy, debug et clean"
groboy:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)
debug: 
	$(CC) -o $(EXEC) $(SRC) $(GFLAGS) 
fac: 
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) $(LIBRARY)
clean: 
	rm groboy 2> /dev/null
