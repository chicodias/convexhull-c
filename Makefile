
all: lista.o primitivas.o embrulho.o main.o 
	gcc -std=c99 -Wall lista.o primitivas.o embrulho.o main.o -o proj-1 -lm
	
run:
	./proj-1; 