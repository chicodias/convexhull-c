#include "lista.h"
// file: embrulho.h
// authors:  Francisco Rosa Dias de Miranda e Hiago
#ifndef EMBRULHO_H
#define EMBRUHLO_H

/* Algoritmo do embrulho.
Inicialmente, determine o ponto P do conjunto com menor coordenada y.
Em seguida encontre o ponto Q, dentre os que outros pontos, com menor ângulo
em relação a P e a linha horizontal. Esse é um ponto que certamente é vértice 
do fecho convexo, e mais, a aresta PQ pertence ao fecho convexo. A partir daí,
basta encontrar o próximo ponto com menor ângulo em relação a aresta PQ, e assim
por diante. A execução termina quando o ponto P inicial é encontrado novamente.
*/

/* retorna uma lista circular com o poligono convexo a partir dos pontos em L
*/
LISTA * embrulho(char pos, int inic,LISTA * L);

/* calcula um angulo entre um segmento PQ e um ponto r.
retorna o angulo */
double angulo(NO p, NO q, NO r);



#endif
