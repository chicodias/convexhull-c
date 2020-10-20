#include "lista.h"
#include "embrulho.h"
// pra usar esse bereguenaits tem que compilar com a tag -lm
#include <math.h>


/* Algoritmo do embrulho.
Inicialmente, determine o ponto P do conjunto com menor coordenada y.
Em seguida encontre o ponto Q, dentre os que outros pontos, com menor ângulo
em relação a P e a linha horizontal. Esse é um ponto que certamente é vértice 
do fecho convexo, e mais, a aresta PQ pertence ao fecho convexo. A partir daí,
basta encontrar o próximo ponto com menor ângulo em relação a aresta PQ, e assim
por diante. A execução termina quando o ponto P inicial é encontrado novamente.
*/

NO * findlowest(LISTA * L)
{
    return L->inicio;
}

double angulo(NO * p, NO * q, NO * r)
{
    double ang;
    NO * PQ, * PR;

    PQ->x = q->x - p->x;
    PQ->y = q->y - p->y;

    PR->x = r->x - p->x;
    PR->y = r->y - p->y;

    // cosseno do trem
    ang = (PQ->x*PR->x + PQ->y*PR->y) /
          sqrt(PQ->x * PQ->x + PQ->y * PQ->y) * sqrt(PR->x * PR->x + PR->y * PR->y);

    return acos(ang); //produto escalar daquele treco
}
