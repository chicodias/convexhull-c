
all: lista.o primitivas.o embrulho.o pilha.o graham.o tempo.o mergesort.o main.o
	gcc -std=c99 -Wall primitivas.o embrulho.o pilha.o graham.o tempo.o mergesort.o main.o lista.o -o proj-1 -lm
	
run:
	./proj-1; 

clean:
	rm *.o proj-1