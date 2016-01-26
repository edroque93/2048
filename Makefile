all: 2048.c
	gcc 2048.c -std=c99 -lncurses -Wall -o 2048 -O3

clean:
	rm 2048 *.o -rf
 
