#include "lista.h"

/* Esta primitiva consiste em sabermos a posição de um ponto em
 relação a um vetor (segmento orientado). Então dados três pontos
  A, B e C tal que AB é um segmento orientado, temos que a primitiva
  Esquerda(A,B,C) é verdadeira se o ponto C está a esquerda da reta
 formada ao estendermos o segmento AB nos dois sentidos. A primitiva 
 é falsa caso contrário. Analogamente temos a primitiva Direita.
*/


double area2(NO * p, NO * q, NO * r)
{
    // cada componente do vetor (x,y) em uma posicao
    // x na pos 0 e y na pos 1.
    double a[2], b[2], c[2];

    a[0] = p->x;
    a[1] = p->y;

    b[0] = q->x; 
    b[1] = q->y;

    c[0] = r->x;
    c[1] = r->y;

    return((a[0] * b[1] - a[1] * b[0] +
            a[1] * c[0] - a[0] * c[1] +
            b[0] * c[1] - c[0] * b[1]));
}

boolean esquerda(NO * p, NO * q, NO * r)
{
   return (area2 (p, q, r) > 0);
}

boolean direita(NO * p, NO * q, NO * r)
{
   return (area2 (p, q, r) < 0);
}