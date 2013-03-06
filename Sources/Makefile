SRC= cpu.c gpu.c memory.c main.c interrupts.c timer.c sound.c
CC=gcc
CFLAGS = -std=c99 -lSDL 
EXEC= groboy
OBJ= $(SRC: .c=.o)

all:
	echo "make, trois options possibles : groboy, debug et clean"
groboy:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)
debug: 
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) -g
clean:
	rm groboy 
	rm *.o
