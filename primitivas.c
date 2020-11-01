/* 
 * File:   primitivas.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
 */

#include "lista.h"
#include "primitivas.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// retorna o produto escalar entre os vetores OP e OQ
double prodEscalar (NO * p, NO * q)
{
  return (p->x * q->x + p->y * q->y);
}

// usa a funcao acima para calcular a norma ao quadrado do vetor p
double norma2 (NO * p)
{
  return (prodEscalar (p, p));
}

// recebe três pontos e retorna o quadrado da área do triangulo
// PQR pela lei do determinante.

double area2(NO * p, NO * q, NO * r)
{
    // cada componente do vetor (x,y) em uma posicao
    double a[2] = {p->x, p->y},
           b[2] = {q->x, q->y},
           c[2] = {r->x, r->y};

    return((a[0] * b[1] - a[1] * b[0] +
            a[1] * c[0] - a[0] * c[1] +
            b[0] * c[1] - c[0] * b[1]));
}

/* Esta primitiva consiste em sabermos a posição de um ponto em
 relação a um vetor (segmento orientado). Então dados três pontos
  A, B e C tal que AB é um segmento orientado, temos que a primitiva
  Esquerda(A,B,C) é verdadeira se o ponto C está a esquerda da reta
 formada ao estendermos o segmento AB nos dois sentidos. A primitiva 
 é falsa caso contrário. Analogamente temos a primitiva Direita.
*/
boolean esquerda(NO * p, NO * q, NO * r)
{
   return (area2 (p, q, r) > 0);
}

// calcula o angulo entre o vetor PQ e o vetor PR
double angulo(NO * p, NO * q, NO * r)
{
    // angulo que retornaremos
    double ang;
    
    // vetores PQ e PR
    NO * PQ = criaNo(q->x - p->x, q->y - p->y);
    NO * PR = criaNo(r->x - p->x, r->y - p->y);

    // cos do angulo entre PQ e PR a partir da def
    // de produto escalar
    ang = prodEscalar(PQ, PR) / (sqrt(norma2(PQ)) * sqrt(norma2(PR)));

    free(PQ);
    free(PR);

    return acos(ang); // retorna o angulo no intervalo [0,2pi]
}

// inicializa a lista de angulos
ANGULOS * criarAng(void)
{
    ANGULOS * P = (ANGULOS *) malloc (sizeof(ANGULOS));
    P->inicio = NULL;
    return P;
}

void insere_inicio_ang (ANGULOS * l, double y, NO * x)
{
    ANGS* novo = (ANGS *)malloc(sizeof(ANGS));

    novo->ang = y;
    novo->ponto = x;
    novo->prox = l->inicio;

    l->inicio = novo;
}

// retorna a posicao anterior na lista do ponto com menor angulo
// se NULL, esta na primeira posicao.
ANGS * minAng(ANGULOS * l)
{  
    ANGS * min = l->inicio, * p;

    for (p = min; p->prox != NULL; p = p->prox)
        if (p->prox->ang < min->prox->ang)
            min = p;

    if(l->inicio->ang < min->prox->ang)
        min = NULL;

    return min;
}

// retorna a pos. anterior ao maior angulo da lista l
ANGS * maxAng(ANGULOS * l)
{  
    ANGS * max = l->inicio, * p;

    for (p = max; p->prox != NULL; p = p->prox){
        if (max->prox->ang < p->prox->ang)
            max = p;

    }

    if(l->inicio->ang > max->prox->ang)
        max = NULL;

    return max;
}

// desaloca a lista de angulos
void angs_apagar(ANGULOS **l){
    ANGS *p, *q;

    // se a lista estiver vazia, retorne 
    if (*l == NULL)
        return;

    // inicialização do ponteiro que percorre a lista
    p = (*l)->inicio;
    while(p != NULL)
    {
        q = p->prox;
        free(p);
        p = q;
    }
    
    free(*l);
    
}

// retorna TRUE se P = Q
boolean pontosIguais(NO * p, NO * q)
{
    if(p->x == q->x)
        if(p->y == q->y)
            return TRUE;

    return FALSE;
}