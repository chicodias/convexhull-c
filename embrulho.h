// file: embrulho.h
// authors:  Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
#ifndef EMBRULHO_H
#define EMBRULHO_H

#include "lista.h"
#include "primitivas.h"

typedef struct angulos ANGS;

struct angulos
{
    double ang;
    NO * ponto;
    ANGS * prox;
};

typedef struct lista_angulos
{
    ANGS * inicio;
} ANGULOS;

/* retorna uma lista circular com o poligono convexo a partir dos pontos em L
*/
LISTA * embrulho(char pos, int inic, LISTA * L);

/* Encontra o ponto com a menor coordenada y  e retorna-o */
NO * findlowest(LISTA * L);

/* calcula um angulo entre um segmento PQ e um ponto r.
retorna o angulo */
double angulo(NO * p, NO * q, NO * r);

// procura o angulo minimo em um vetor e retorna sua posicao
int minAng(double * vet, int n);

int maxAng(double * vet, int n);

ANGULOS * criarAng(int n);

// insere um elemento na lista ordenadamente de acordo com a coordenada y.
void ins_ang (ANGULOS * l, double y, NO * x);

void angs_imprimir(ANGULOS *l);

#endif
