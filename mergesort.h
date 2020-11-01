#ifndef MERGE_H
#define MERGE_H

#include "primitivas.h"
#include "lista.h"

// tamanho da lista atual
int length(ANGS* current);

// junta as duas partes ordenadas de uma lista ANGULOS
void merge(ANGS** start1, ANGS** end1,  
          ANGS** start2, ANGS** end2);

// mergesort iterativo para ordenação da lista ANGULOS 
void mergeSort(ANGS** head);

//coloca um NO na frente da lista
void push(ANGS** head_ref,  
          int new_data);


#endif
