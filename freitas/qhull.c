/*
 * QHull.c
 */
#include "fmath.h"
#include "fstdlib.h"
#include "cg.h"
#include "erro.h"
#include "fgrafico.h"
#include "pilha.h"
#include "qhull.h"
#include "vetor.h"

/* vetor com os pontos */
extern _point pontos[];
/* variaveis de controle */
extern int animacao, pare, writemode;

/* Funcao recursiva do quickhull. Recebe um segmento apontado pelos inteiros
   a e b, uma pilha s com os pontos a analisar e calcula o fecho convexo
   local, devolvendo-o em uma pilha. */
_pilha quickhull_r (int a, int b, _pilha s)
{
  _pilha s1, s2, r1, r2;
  unsigned long int max, t;
  int pmax;
  if (pare)
    return (s);
  if ((s->prox)->prox == NULL) /* se s = {a, b}, basta liga-los */
  {
    if (animacao)
    {
      setcolor (LIGHTMAGENTA);
      line (pontos[a][X], pontos[a][Y], pontos[b][X], pontos[b][Y]);
    }
    return (s);
  }
  else /* senao */
  {
    /* achamos o ponto mais 'alto' (com maior area) */
    s1   = s->prox;
    pmax = s->i;
    max  = labs (area2 (pontos[a], pontos[b], pontos[s->i]));
    while (s1 != NULL)
    {
      if ((t = labs(area2 (pontos[a], pontos[b], pontos[s1->i]))) > max)
      {
	pmax = s1->i;
	max = t;
      }
      s1 = s1->prox;
    }

    /* Agora quebramos o problema em duas partes: entre [a,pmax] e [pmax,b] */
    s1=s2=NULL;
    while (s != NULL)
    {
      if (right (pontos[a], pontos[pmax], pontos[s->i]))
	s1 = empilha (s->i, s1);
      else if (right (pontos[pmax], pontos[b], pontos[s->i]))
	s2 = empilha (s->i, s2);
      s = desempilha (s);
    }

    /* resolvemos recursivamente para s1: entre a e pmax */
    s1 = empilha (a,    s1);
    s1 = empilha (pmax, s1);
    r1 = quickhull_r (a, pmax, s1);

    /* e para s2: entre pmax e b */
    s2 = empilha (pmax, s2);
    s2 = empilha (b,    s2);
    r2 = quickhull_r (pmax, b, s2);

    /* e agora unimos os resultados */
    while (r1 != NULL)
    {
      r2 = empilha (r1->i, r2);
      r1 = desempilha (r1);
    }
    return (r2);

  }
}

/* Funcao quickhull: recebe um inteiro np o numero de pontos e calcula o
   fecho convexo utilizando o vetor 'pontos'. Devolve a resposta em uma
   pilha. */
_pilha quickhull (int np)
{
  _pilha p, s1, s2, s3, s4, r1, r2, r3, r4;

  int i, maxx, maxy, minx, miny;

  p=s1=s2=s3=s4=NULL;

  /* guarda todos os pontos em uma pilha */
  for (i=0; i<np; i++)
    p = empilha (i, p);

  /* encontra os pontos extremos, para formar o quadrilatero */
  maxx = minx = maxy = miny = 0;
  for (i=1; i<np; i++)
  {
    if (pontos[i][X] > pontos[maxx][X])
      maxx = i;
    else if (pontos[i][X] < pontos[minx][X])
      minx = i;
    if (pontos[i][Y] > pontos[maxy][Y])
      maxy = i;
    else if (pontos[i][Y] < pontos[miny][Y])
      miny = i;
  }

  if (animacao)
  {
    /* desenha o quadrilatero */
    setwritemode (writemode);
    setcolor (YELLOW);
    line (pontos[miny][X], pontos[miny][Y], pontos[maxx][X], pontos[maxx][Y]);
    line (pontos[maxx][X], pontos[maxx][Y], pontos[maxy][X], pontos[maxy][Y]);
    line (pontos[maxy][X], pontos[maxy][Y], pontos[minx][X], pontos[minx][Y]);
    line (pontos[minx][X], pontos[minx][Y], pontos[miny][X], pontos[miny][Y]);
  }

  /* coloca cada ponto na sua regiao correspondente */
  while (p != NULL)
  {
    if (right (pontos[miny], pontos[maxx], pontos[p->i]))
      s1 = empilha (p->i, s1);
    else if (right (pontos[maxx], pontos[maxy], pontos[p->i]))
      s2 = empilha (p->i, s2);
    else if (right (pontos[maxy], pontos[minx], pontos[p->i]))
      s3 = empilha (p->i, s3);
    else if (right (pontos[minx], pontos[miny], pontos[p->i]))
      s4 = empilha (p->i, s4);
    p = desempilha (p);
  }

  /* agora empilha os extremos dos segmentos do quadrilatero */
  s1 = empilha (miny, s1);
  s1 = empilha (maxx, s1);
  s2 = empilha (maxx, s2);
  s2 = empilha (maxy, s2);
  s3 = empilha (maxy, s3);
  s3 = empilha (minx, s3);
  s4 = empilha (minx, s4);
  s4 = empilha (miny, s4);

  /* aplica o algoritmo recursivo para cada uma das regioes */
  r1 = quickhull_r (miny, maxx, s1);
  r2 = quickhull_r (maxx, maxy, s2);
  r3 = quickhull_r (maxy, minx, s3);
  r4 = quickhull_r (minx, miny, s4);

  /* e entao junta a resposta das 4 regioes */
  while (r1 != NULL)
  {
    r4 = empilha (r1->i, r4);
    r1 = desempilha (r1);
  }
  while (r2 != NULL)
  {
    r4 = empilha (r2->i, r4);
    r2 = desempilha (r2);
  }
  while (r3 != NULL)
  {
    r4 = empilha (r3->i, r4);
    r3 = desempilha (r3);
  }

  if (animacao)
  {
    setwritemode (0);
    setcolor (WHITE);
  }

  return (r4);
}
/* Fim de QHULL.C */
