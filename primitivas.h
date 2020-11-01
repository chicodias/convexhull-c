/* 
 * File:   primitivas.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
  (baseado no arquivo lista.h de Eliane Gniech Karasawa)
 */

#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H

#include "lista.h"

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


// RETORNA o produto escalar entre dois vetores 
double prodEscalar (NO * p, NO * q);

//retorna a norma ao quadrado de dois vetores
double norma2 (NO * p);

// retorna a area do triangulo formado com os pontos p, q e r.
double area2 (NO * p, NO * q, NO * r);

//posição de um ponto em relação a um vetor
boolean esquerda (NO * p, NO * q, NO * r);

/* calcula um angulo entre um segmento PQ e um ponto r.
retorna o angulo */
double angulo(NO * p, NO * q, NO * r);

// inicializa a lista de angulos
ANGULOS * criarAng(void);

// insere um elemento no começo da lista
void insere_inicio_ang (ANGULOS * l, double y, NO * x);

// imprime a lista de angulos
void angs_imprimir(ANGULOS *l);

// desaloca uma lista de ANGS
void angs_apagar(ANGULOS **l);

// retorna o minimo de uma lista de ANGS
ANGS * minAng(ANGULOS * l);

// retorna o máximo de uma lista de ANGS
ANGS * maxAng(ANGULOS * l);

// Compara dos pontos p e q
boolean pontosIguais(NO * p, NO * q);

/* // ordenaa lista de angulos
void MergeSort(ANGS ** headRef);

// função de ordenação auxiliar que junta duas listas ordenadas
ANGS* SortedMerge(ANGS* a, ANGS* b);

// função auxiliar que divide a lista ao meio
void FrontBackSplit(ANGS* source, 
                    ANGS** frontRef, ANGS** backRef); */

#endif