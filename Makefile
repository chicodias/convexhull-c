

all: embrulho.o lista.o main.o 
	gcc -std=c99 -Wall lista.o embrulho.o main.o -o proj-1 -lm

embrulho.o:
	gcc -std=c99 -Wall embrulho.c -lm
	
run:
	./proj-1; 