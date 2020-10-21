

all: embrulho.o lista.o fila.o main.o 
	gcc -lm -std=c99 -Wall fila.o main.o -o proj-1

embrulho.o:
	gcc -lm -std=c99 -Wall embrulho.c

lista.o:
	gcc -lm -std=c99 -Wall lista.c

fila.o:
	gcc -lm -std=c99 -Wall fila.c

main.o:
	gcc -lm -c -std=c99 -Wall main.c
	
run:
	./proj-1; 