// file: incremental.h
// authors:  Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
#ifndef INCREMENTAL_H
#define INCREMENTAL_H

#include "lista.h"
#include "primitivas.h"

// algoritmo incremental de cálculo do fecho convexo
LISTA incremental(char pos, int inic, LISTA * l);

// função que calcula sentido ant horario
void anthorario(LISTA *l, NO *p, NO *q, NO *r, int sentido);

// função que calcula sentido horário
void horario(LISTA *l, NO *p, NO *q, NO *r, int sentido);

#endif
