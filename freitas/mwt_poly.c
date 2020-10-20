#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cg.h"
#include "erro.h"
#include "fgrafico.h"
#include "mindist.h"
#include "vetor.h"
#include "mwt_poly.h"

#define DDEBUG(x)

extern int pare, animacao, writemode;

int is_poly(int i, int j, _poly p) 
{
  if (i == j || (i+1)%p.np == j || (j+1)%p.np == i) return TRUE;
  return FALSE;
}

#define INFINITO ((unsigned) -1)

void imprime(estrutura *e)
{
  int i, j;
  printf("\n\n");
  for (i=0; i<e->n; i++) {
    printf ("\ni=%3d:", i);
    for (j=0; j<e->n; j++)
      printf ("  %10f", e->matriz[i][j]);
  }
  printf ("\n\nK");
  for (i=0; i<e->n; i++) {
    printf ("\ni=%3d:", i);
    for (j=0; j<e->n; j++)
      printf ("  %10d", e->escolha_k[i][j]);
  }
  printf ("\n");
}

estrutura *procura_otimo(_poly poly)
{
  estrutura *e, *pre;
  int n, i, j, k, l;
  double q;
  int ani = animacao;
  animacao = 0;

  pre = (estrutura *) malloc(sizeof(estrutura));
  e = (estrutura *) malloc(sizeof(estrutura));
  if (e == NULL || pre == NULL) runerror (FALTA_MEMORIA);

  e->n = n = poly.np;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++) {
      e->matriz[i][j] = INFINITO;
      e->escolha_k[i][j] = INFINITO;
      pre->escolha_k[i][j] = is_poly(i, j, poly) | is_diagonal(i, j, poly);
    }

  for (i=0; i<n; i++) {
    e->matriz[i][i]   = 0.0;
    e->matriz[i][i+1] = 0.0;

  }

  DDEBUG(imprime(e);)

  for (l=2; l<n && !pare; l++) {
    for (i=0; i<n-l && !pare; i++) {
      j = i+l;
      for (k=i+1; k<j && !pare; k++) {
        DDEBUG(printf ("i:%d j:%d k:%d l:%d\n", i, j, k, l);)

	if (pre->escolha_k[i][k] && pre->escolha_k[k][j]) {
          q = e->matriz[i][k] + e->matriz[k][j] +
              qdist (poly.p[i], poly.p[j]);
          DDEBUG(printf ("q:%f\n", q);)
          if (e->matriz[i][j] == INFINITO || q < e->matriz[i][j]) {
            DDEBUG(printf("Chegou...\n");)
            e->matriz[i][j] = q;
            e->escolha_k[i][j] = k;
          }
        }
      }
    }
  }
  DDEBUG(imprime (e);)
  animacao = ani;
  free(pre);
  return e;
}

int is_diagonal (int i, int f, _poly p)
{
  int k, ant, prox, ri, rf;
  if (is_poly(i,f,p)) return 0;
  for (k=0; k<p.np; k++) {
    if (intersec (p.p[k], p.p[(k+1)%p.np], p.p[i], p.p[f]))
      return FALSE;
  }
  
  ant  = i? i-1: p.np-1;
  prox = (i+1)%p.np;
  if (right (p.p[i], p.p[ant], p.p[prox])) {
    if (!left  (p.p[i], p.p[ant],  p.p[f]) &&
        !right (p.p[i], p.p[prox], p.p[f]))
      ri = TRUE;
    else ri = FALSE;
  }
  else if (left (p.p[i], p.p[ant], p.p[prox])) {
    if (left  (p.p[i], p.p[ant],  p.p[f]) &&
        right (p.p[i], p.p[prox], p.p[f]))
      ri = FALSE;
    else ri = TRUE;  
  }
  else if (p.p[ant][X] == p.p[prox][X] && p.p[ant][Y] == p.p[prox][Y]) ri = TRUE;
  else if (!left (p.p[i], p.p[ant], p.p[f])) ri = TRUE;
  else ri = FALSE;

  ant  = f? f-1: p.np-1;
  prox = (f+1)%p.np;
  if (right (p.p[f], p.p[ant], p.p[prox])) {
    if (!left  (p.p[f], p.p[ant],  p.p[i]) &&
        !right (p.p[f], p.p[prox], p.p[i]))
      rf = TRUE;
    else rf = FALSE;
  }
  else if (left (p.p[f], p.p[ant], p.p[prox])) { 
    if (left  (p.p[f], p.p[ant],  p.p[i]) &&
        right (p.p[f], p.p[prox], p.p[i]))
      rf = FALSE;
    else rf = TRUE;
  }
  else if (p.p[ant][X] == p.p[prox][X] && p.p[ant][Y] == p.p[prox][Y]) rf = TRUE;
  else if (!left (p.p[f], p.p[ant], p.p[i])) rf = TRUE;
  else rf = FALSE;

  return (ri && rf);
}

/* nao esta funcionando corretamente (falta o caso especial) */
/*int is_internal (_point p, _poly poly)
{
  _point inf;
  int i, ni = 0;
  inf[X] = p[X];
  inf[Y] = MAX_I;
  for (i=0; i<poly.np; i++) {
    if (intersec (poly.p[i], poly.p[(i+1)%poly.np], p, inf))
      ni++;
  }
  return (ni % 2);
}*/

_poly pol;

void mostra_diagonais (estrutura *e, int i, int j)
{
  DDEBUG(printf ("\nmostra_diagonais (%d, %d)", i, j);)
  if (i < 0 || j < 0) return;
  if (!is_poly (i, j, pol)) {
    DDEBUG (printf ("\nmostrando (%d, %d)", i, j);)
    line (pol.p[i][X], pol.p[i][Y], pol.p[j][X], pol.p[j][Y]);
  }
  mostra_diagonais (e, i, e->escolha_k[i][j]);
  mostra_diagonais (e, e->escolha_k[i][j], j);
}

double mwt_poligono (_poly poly)
{
  estrutura *e;
  double t;
  if (animacao) {
    setcolor(LIGHTBLUE);
    setwritemode(writemode);
  }
  e = procura_otimo(poly);
  if (!pare) {
    pol = poly;
    mostra_diagonais (e, 0, poly.np-1);
  }
  t = e->matriz[0][poly.np-1] - qdist (poly.p[0], poly.p[poly.np-1]);
  if (animacao) {
    setcolor(WHITE);
    setwritemode(0);
  }
  free(e);
  return t;
}
