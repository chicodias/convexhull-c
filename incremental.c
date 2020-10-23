#include "incremental.h"
#include "primitivas.h"

LISTA incremental(char pos, int inic, LISTA * l){

/* Utilizamos as primitivas Esquerda-Direita
para saber a posição de um dado ponto em relação
a um lado do fecho convexo atual. A idéia é simplificada
se assumirmos que o polígono tem a seqüência de seus
vértices orientada em um sentido (anti-horário, 
por exemplo). Assim, basta verificar se um novo
ponto está a esquerda (se o sentido adotado for
anti-horário) de todos os lados do fecho. Se isso
for verdade, então trata-se de um ponto interno ao 
fecho, senão é um ponto externo que será colocado como
um vértice. */

// anti-horario

void anthorario(LISTA *l, NO *p, NO *q, NO *r, int sentido){

	// criando o convexo atual
	NO * PQ = criaNo(q->x - p->x, q->y - p->y);
    NO * PR = criaNo(r->x - p->x, r->y - p->y);
    NO * QR = criaNo(r->x - q->x, r->y - q->y);

	if(sentido == 1){

		/* Percorrer a lista, verificar se tem um x menor do que o atual de acordo com as retas, caso tenha "conserte" o convexo */

		/* code */
	}

}

void horario(LISTA *l, NO *p, NO *q, NO *r, int sentido){

	// criando o convexo atual
	NO * PQ = criaNo(q->x - p->x, q->y - p->y);
    NO * PR = criaNo(r->x - p->x, r->y - p->y);
    NO * QR = criaNo(r->x - q->x, r->y - q->y);

    if(sentido == 0){

    	/* Percorrer a lista, verificar se tem um x menor do que o atual de acordo com as retas, caso tenha "conserte" o convexo */

    	/* code */
    }
}




