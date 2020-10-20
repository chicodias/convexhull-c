/*
 * guloso.c
 */
#include "fstdlib.h"
#include "cg.h"
#include "fgrafico.h"
#include "fmalloc.h"
#include "guloso.h"
#include "sort.h"
#include "vetor.h"
#include "erro.h"
#include "mindist.h"

/* vetor com pontos */
extern _point pontos[];
/* variaveis de controle */
extern animacao, pare, writemode, delay_time;

#define M MAX_PONTOS

_aresta *guloso (unsigned int n)
{
  int i, j, k, p, s, tam;
  int t1 = -1, t2 = -1; /* so para nao dar warning */
  char *ok, *mat[M];
  _aresta_p *a;
  _aresta *resp;

  if (animacao)
  {
    setwritemode (1);
    setcolor (WHITE);
  }

  if ((a = (_aresta_p *) malloc (M*M*sizeof (_aresta_p))) == NULL)
    runerror (FALTA_MEMORIA);
  if ((ok = (char *) malloc (M*M*sizeof (char))) == NULL)
    runerror (FALTA_MEMORIA);

  /* matriz de incidencia */
  for (i = 0; i<M; i++) {
    if ((mat[i] = (char *) malloc (M*sizeof (char))) == NULL)
      runerror (FALTA_MEMORIA);
    for (j = 0; j<M; j++)
      mat[i][j] = 0;
  }

  for (k = 0, i = 0; i<n; i++)
    for (j = i+1; j<n; j++, k++) {
      if (k >= M*M) break;
      ok[k] = 0;
      attrib (&(a[k].inicio), pontos[i]);
      attrib (&(a[k].fim),    pontos[j]);
    }

  j = delay_time;
  if (j > 8) delay_time = 8;
  guloso_sort (a, k);
  delay_time = j;

  if (animacao)
  {
    setwritemode (0);
    setcolor (WHITE);
  }

  /* loop nas arestas */
  for (tam = 0, p = 0; p<k && !pare; p++) {

    /* procurando por interseccao */
    for (i = 0; i < p; i++)
      if (ok[i] && intersec (a[i].inicio, a[i].fim, a[p].inicio, a[p].fim))
        break;

    if (i < p) /* ha interseccao com alguma ja inclusa */
      continue;

    /* incluimos a aresta */
    ok[p] = 1;
    tam++;

    /* para fazer uma matriz de incidencia */
    for (s = 0, j = 0; j<n; j++) {
      if (pontos[j][X] == a[p].inicio[X] && pontos[j][Y] == a[p].inicio[Y])
      { t1 = j; s += 100; }
      if (pontos[j][X] == a[p].fim[X] && pontos[j][Y] == a[p].fim[Y])
      { t2 = j; s += 1; }
    }
    if (s != 101) runerror(INDEFINIDO);
    mat[t1][t2] = 1;
    mat[t2][t1] = -1;
    /* fim matriz */

    if (animacao) {
      line (a[p].inicio[X], a[p].inicio[Y], a[p].fim[X], a[p].fim[Y]);
      delay (delay_time);
    }
  }

  if (animacao)
  {
    setwritemode (0);
    setcolor (WHITE);
  }

  if ((resp = (_aresta *) malloc ((tam+1) * sizeof(_aresta))) == NULL)
    runerror(FALTA_MEMORIA);

  for (tam=0, i=0; i<n; i++)
    for (j=0; j<n; j++)
      if (mat[i][j] == 1) {
	resp[tam].inicio = i;
	resp[tam].fim = j;
        if (!is_fecho2 (i, j, n))
          resp[tam].peso = qdist(pontos[i], pontos[j]);
        else 
          resp[tam].peso = 0;
	tam++;
      }
  resp[tam].inicio = resp[tam].fim = -1;

  for (i=0; i<M; i++)
    free (mat[i]);
  free (ok);
  free (a);
  return resp;
}
/* Fim de GULOSO.C */

void refina_sol (char *mat[M])
{
  unsigned i, j, k, x[20], t, d, acabou = 0, tempo = 0, esq = -1, dir = -1;
  unsigned long area;

  d = delay_time;
  delay_time = 0;

  while (!acabou && !pare && ++tempo < 10000) {
    acabou = 1;
    for (i = 0; i < M; i++)
      for (j = 0; j < M; j++)
        if (mat[i][j]) {
          for (t = 0, k = 0; k < M; k++)
            if (mat[i][k] && mat[j][k]) x[t++] = k; /* aqui tem um proble */
          if (t == 2) {
            esq = x[0];
            dir = x[1];
          }
          if (t > 2) {
            for (area = (unsigned long) -1, k = 0; k < t; k++)
              if (left (pontos[i], pontos[j], pontos[x[k]]) &&
                  labs(area2 (pontos[i], pontos[j], pontos[x[k]])) < area) {
                esq = x[k];
                area = labs (area2 (pontos[i], pontos[j], pontos[esq]));
              }
            for (area = (unsigned long) -1, k = 0; k < t; k++)
              if (right (pontos[i], pontos[j], pontos[x[k]]) &&
                  labs(area2 (pontos[i], pontos[j], pontos[x[k]])) < area) {
                dir = x[k];
                area = labs (area2 (pontos[i], pontos[j], pontos[dir]));
              }
          }
          if (t >= 2)
            if (qdistancia (pontos[esq], pontos[dir]) <
                qdistancia (pontos[i],    pontos[j]) &&
                intersec (pontos[i], pontos[j], pontos[esq], pontos[dir])) {
              mat[i][j] = mat[j][i] = 0;
              setcolor (WHITE);
              line (pontos[i][X], pontos[i][Y], pontos[j][X], pontos[j][Y]);
              setcolor (LIGHTMAGENTA);
              line (pontos[i][X], pontos[i][Y], pontos[j][X], pontos[j][Y]);
              delay (500);
              mat[esq][dir] = 1;
              mat[dir][esq] = -1;
              setcolor (LIGHTMAGENTA);
              line (pontos[esq][X], pontos[esq][Y],
                    pontos[dir][X], pontos[dir][Y]);
              setcolor (LIGHTMAGENTA);
              line (pontos[i][X], pontos[i][Y], pontos[j][X], pontos[j][Y]);
              delay (500);
              setcolor (LIGHTMAGENTA);
              line (pontos[esq][X], pontos[esq][Y],
                    pontos[dir][X], pontos[dir][Y]);
              setcolor (WHITE);
              line (pontos[esq][X], pontos[esq][Y],
                    pontos[dir][X], pontos[dir][Y]);
              acabou = 0;
            }
        }
  }
  delay_time = d;
}
