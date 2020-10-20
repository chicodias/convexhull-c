/*
 * Sort.c
 */
#include "fstdlib.h"
#include "cg.h"
#include "sort.h"
#include "vetor.h"
#include "mindist.h"

/* Sort para Minima Distancia */
static coordenada; /* faz a ordenacao na coordenada especificada */
int mindist_comp (const void *a, const void *b)
{
  return ((*(_point *) a)[coordenada] - (*(_point *) b)[coordenada]);
}

/* Funcao que faz a ordenacao dos n pontos apontados por p, utilizando
   o quicksort e a funcao de comparacao acima. */
void mindist_sort (_point *p, int coord, unsigned int n)
{
  coordenada = coord;
  qsort (p, n, sizeof (_point), mindist_comp);
}

/* Sort para o fecho convexo (Graham) */
static _point p0; /* usado como base para a ordenacao */
int graham_comp (const void *p1, const void *p2)
{
  long int a;
  _point r1, r2;
  int len1, len2;

  a = area2 (p0, (*(_point *) p1), (*(_point *) p2));

  if (a > 0L)
    return (-1);
  else if (a < 0L)
    return (+1);
  else /* se empatou vamos ver quem esta mais perto */
  {
    subvec ((*(_point *) p1), p0, r1);
    subvec ((*(_point *) p2), p0, r2);
    len1 = length2 (r1);
    len2 = length2 (r2);
    return ((len1 < len2) ? -1 : ((len1 > len2) ? 1 : 0));
  }
}

/* Funcao utilizada pelo Graham para ordenar os n pontos apontados por p,
   utilizando a funcao de comparacao acima e o quicksort. */
void graham_sort (_point *p, unsigned n)
{
  attrib (&p0, p[0]);
  qsort ((p+1), n-1, sizeof (_point), graham_comp);
}

extern _poly poligono; /* poligono a ser ordenado:
                            dados aqui nao serao destruidos */
int triang_comp (const void *a, const void *b)
{
  int a1 = * (int *) a, b1 = * (int *) b;
  if (poligono.p[a1][coordenada] == poligono.p[b1][coordenada])
    return (0);
  if (poligono.p[a1][coordenada] >  poligono.p[b1][coordenada])
    return (1);
  return (-1);
}

/* funcao que ordena em coordenada uma tabela de inteiros que funcionam
   como apontadores para pontos do poligono, utilizando o quicksort e a
   funcao de ordenacao acima. Assim, o poligono nao eh alterado, apenas
   esta lista de inteiros. */
void triang_sort (int *i, int coord, unsigned n)
{
  coordenada = coord;
  qsort (i, n, sizeof (int), triang_comp);
}

int guloso_comp (const void *a, const void *b)
{
  _aresta_p a1 = * (_aresta_p *) a;
  _aresta_p b1 = * (_aresta_p *) b;
  long int d = qdistancia (a1.inicio, a1.fim) -
               qdistancia (b1.inicio, b1.fim);
  if      (d < 0) return (-1);
  else if (d > 0) return (1);
  else            return (0);
}

void guloso_sort (_aresta_p *a, unsigned n)
{
  qsort (a, n, sizeof (_aresta_p), guloso_comp);
}

int aem_comp (const void *a, const void *b)
{
  _aresta a1 = * (_aresta *) a;
  _aresta b1 = * (_aresta *) b;
  if (a1.peso > b1.peso) return 1;
  if (a1.peso < b1.peso) return -1;
  return 0;
}

void aem_sort (_aresta *a, unsigned n)
{
  qsort (a, n, sizeof (_aresta), aem_comp);
}

/* Fim de SORT.C */
