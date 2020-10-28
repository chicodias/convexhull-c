/* 
 * File:   pilha.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
 
 */

#ifndef PILHA_H
#define PILHA_H

#include "lista.h"
#include <stdlib.h>

typedef struct reg {
   NO * ponto; 
   struct reg *prox;
} PILHA;

PILHA * criapilha (void);

void empilha (PILHA * pi, NO * y);

NO * desempilha (PILHA * pi);

boolean pilha_vazia(PILHA * pi);

PILHA * topo (PILHA * pi);

LISTA * pilhatolist(PILHA * pi);

#endif //PILHA.H
