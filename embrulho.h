// file: embrulho.h
// authors:  Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
#ifndef EMBRULHO_H
#define EMBRULHO_H

#include "lista.h"

/* retorna uma lista circular com o poligono convexo a partir dos pontos em L
*/
LISTA * embrulho(char pos, int inic, LISTA * L);

/* Encontra o ponto com a menor coordenada y  e retorna-o */
NO * findlowest(LISTA * L);

/* calcula um angulo entre um segmento PQ e um ponto r.
retorna o angulo */
double angulo(NO p, NO q, NO r);



#endif
