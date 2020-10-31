/* 
 * File:   primitivas.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
  (baseado no arquivo lista.h de Eliane Gniech Karasawa)
 */

#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H

#include "lista.h"

// RETORNA o produto escalar entre dois vetores 
double prodEscalar (NO * p, NO * q);

//retorna a norma ao quadrado de dois vetoress
double norma2 (NO * p);

// retorna a area do triangulo formado com os pontos p, q e r.
double area2 (NO * p, NO * q, NO * r);

boolean esquerda (NO * p, NO * q, NO * r);

boolean direita (NO * p, NO * q, NO * r);


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

/* calcula um angulo entre um segmento PQ e um ponto r.
retorna o angulo */
double angulo(NO * p, NO * q, NO * r);

// inicializa a lista de angulos
ANGULOS * criarAng(void);

// insere um elemento na lista ordenadamente de acordo com a coordenada y.
void ins_ang (ANGULOS * l, double y, NO * x);

// insere em ordem decrescente
void ins_ang_dec (ANGULOS * l, double y, NO * x);

// imprime a lista de angulos
void angs_imprimir(ANGULOS *l);

void angs_apagar(ANGULOS **l);

void insere_inicio_ang (ANGULOS * l, double y, NO * x);

ANGS * minAng(ANGULOS * l);
ANGS * maxAng(ANGULOS * l);

boolean pontosIguais(NO * p, NO * q);

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(ANGS ** headRef);
ANGS* SortedMerge(ANGS* a, ANGS* b);
void FrontBackSplit(ANGS* source, 
                    ANGS** frontRef, ANGS** backRef);

#endif