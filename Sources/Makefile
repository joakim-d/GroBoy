SRC= cpu.c gpu.c memory.c main.c interrupts.c timer.c sound.c joypad.c
CC=gcc
CFLAGS = -std=c99 -lSDL -Wall
EXEC= groboy
OBJ= $(SRC: .c=.o)
LIBRARY= `sdl-config --cflags --libs` -lSDLmain -lSDL -I /auto_home/jdivet/LIB/include -L /auto_home/jdivet/LIB/lib
all:
	echo "make, trois options possibles : groboy, debug et clean"
groboy:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)
debug: 
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) -g
fac: 
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) $(LIBRARY)
clean:
	rm groboy 2>/dev/null
	rm *.o 2>/dev/null
