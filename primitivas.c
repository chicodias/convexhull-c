#include "lista.h"

/* Esta primitiva consiste em sabermos a posição de um ponto em
 relação a um vetor (segmento orientado). Então dados três pontos
  A, B e C tal que AB é um segmento orientado, temos que a primitiva
  Esquerda(A,B,C) é verdadeira se o ponto C está a esquerda da reta
 formada ao estendermos o segmento AB nos dois sentidos. A primitiva 
 é falsa caso contrário. Analogamente temos a primitiva Direita.
*/


// retorna o produto escalar entre os vetores OP e OQ
int prodEscalar (NO * p, NO * q)
{
  return (p->x * q->x + p->y * q->y);
}

// usa a funcao acima para calcular a norma ao quadrado do vetor p
int norma2 (NO * p)
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


boolean esquerda(NO * p, NO * q, NO * r)
{
   return (area2 (p, q, r) > 0);
}

boolean direita(NO * p, NO * q, NO * r)
{
   return (area2 (p, q, r) < 0);
}