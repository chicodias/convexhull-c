// file: incremental.h
// authors:  Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
#ifndef INCREMENTAL_H
#define INCREMENTAL_H

#include "lista.h"

// definição do tipo de lista utilizada

// algoritmo incremental de cálculo do fecho convexo
LISTA incremental(char pos, int inic, LISTA * l);

/* Esta primitiva consiste em sabermos a posição de um ponto em
 relação a um vetor (segmento orientado). Então dados três pontos
  A, B e C tal que AB é um segmento orientado, temos que a primitiva
  Esquerda(A,B,C) é verdadeira se o ponto C está a esquerda da reta
 formada ao estendermos o segmento AB nos dois sentidos. A primitiva 
 é falsa caso contrário. Analogamente temos a primitiva Direita.
*/
boolean esquerda(NO a, NO b, NO c);
boolean direita(NO a, NO b, NO c);


#endif
