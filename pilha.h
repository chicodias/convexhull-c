/* 
 * File:   pilha.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo 
 */

#ifndef PILHA_H
#define PILHA_H

#include "lista.h"
#include <stdlib.h>

// define o tad de uma pilha
typedef struct reg {
   NO * ponto; 
   struct reg *prox;
} PILHA;

// cria uma pilha alocando-a no heap e devolvendo seu endereço
PILHA * criapilha (void);

// empilha um elemento na pilha
void empilha (PILHA * pi, NO * y);

// desempilha e libera um elemento da pilha
NO * desempilha (PILHA * pi);

// retorna o topo da pilha sem desempilhar
PILHA * topo (PILHA * pi);

// transforma uma pilha numa lista, desempilhando
LISTA * pilhatolist(PILHA * pi);

#endif //PILHA.H
