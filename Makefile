all: build
build: minimax.c
	gcc -o minimax minimax.c -g -Wall -lm -std=c11 -ggdb3
.PHONY: clean
clean:
	rm -f minimax	
