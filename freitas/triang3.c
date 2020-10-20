/*
 * Triang3.c (ordem cubica)
 */
#include "cg.h"
#include "fgrafico.h"
#include "triang3.h"
#include "triang.h"
#include "vetor.h"
#include "mindist.h"
#include "mwt_poly.h"

/* Variaveis de controle */
extern int animacao, pare, writemode;

/* Funcao que triangulariza em O(n*n*n). Recebe um poligono como entrada.
   Mostra o resultado na tela. */
double triang3 (_poly poly)
{
  unsigned int i, j, k, n, t, tl;
  double peso = 0;
  _poly p;
  _point ponto;

  /* O poligono deve estar no sentido anti-horario! */
  if (is_sentido_horario (poly))
  {
    for (i=0; i<((int)poly.np/2); i++)
    {
      attrib (&ponto, poly.p[i]);
      attrib (&poly.p[i], poly.p[poly.np-i-1]);
      attrib (&poly.p[poly.np-i-1], ponto);
    }
  }

  if (animacao)
    setwritemode (writemode);
  setcolor (YELLOW);

  /* fazemos uma copia do poligono para nao estragar seu conteudo */
  p = poly;
  n = p.np;
  i = 0;
  j = 1;
  k = 2;
  /* enquanto ha o que triangularizar */
  while (!pare && n > 3)
  {
    /* procuramos uma orelha */
    /* Ao encontrar tracamos a diagonal e resolvemos o problema para o
       novo poligono com agora um vertice a menos. */
    if (left (p.p[i], p.p[j], p.p[k]))
    {
      t = (i == 0) ? p.np-1 : i-1;
      while (p.p[t][X] == -1)
        t = (t == 0) ? p.np-1 : t-1;

      if (!left (p.p[i], p.p[t], p.p[k]) || !left (p.p[i], p.p[j], p.p[t]))
      {
        t = -1;
        while (TRUE)
        {
          do
            t++;
          while (p.p[t][X] == -1 && t < p.np);
          if (t >= p.np)
          {
            p.p[j][X] = -1;
            line (p.p[i][X], p.p[i][Y], p.p[k][X], p.p[k][Y]);
            peso += qdist (p.p[i], p.p[k]);
            n--;
            break;
          }
          tl = t;
          do
            tl = (tl+1)%p.np;
          while (p.p[tl][X] == -1);
          if (intersec (p.p[i], p.p[k], p.p[t], p.p[tl]))
            break;
        }
        if (n < 4) break;
      }

    }
    do
      i = (i+1)%p.np;
    while (p.p[i][X] == -1);
    j = i;
    do
      j = (j+1)%p.np;
    while (p.p[j][X] == -1);
    k = j;
    do
      k = (k+1)%p.np;
    while (p.p[k][X] == -1);
  }

  if (animacao)
    setwritemode (0);
  setcolor (WHITE);
  return peso;
}
/* Fim TRIANG3.C */
