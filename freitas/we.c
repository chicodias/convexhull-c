/*
 * we.c  manipulacao de estrutura winged-edge
 */
#include <stdlib.h>
#include "cg.h"
#include "mindist.h"
#include "erro.h"
#include "vetor.h"
#include "fgrafico.h"
#include "sort.h"
#include "we.h"

extern animacao, writemode, pare;
extern _point pontos[];

/* max de 40 no DOS */
#define MAX MAX_PONTOS
#define MAX2 MAX*MAX

typedef struct {
  int a1, a2;
} _we_f;

typedef struct {
  _point p;
  int a1, a2;
} _we_v;

typedef struct {
  char uso;
  int fccw, fcw;
  int pccw, pcw, nccw, ncw;
} _we_a;

struct {
  _we_f f[MAX2];
  _we_v v[MAX];
  _we_a a[MAX][MAX];
  int num_f;
} we;

void inicializa_we (_point *pontos, unsigned int num)
{
  unsigned int i, j;
  if (num >= MAX)
    runerror (FALTA_MEMORIA);

  for (i=0; i<MAX; i++) {
    if (i<num) attrib (&(we.v[i].p), pontos[i]);

    we.v[i].a1 = we.v[i].a2 = -1;
    for (j=0; j<MAX; j++)
      we.a[i][j].uso = FALSE;
  }
  we.num_f = 1;
  for (i=0; i<MAX2; i++)
    we.f[i].a1 = we.f[i].a2 = -1;
}

void libera_we (void)
{
  we.num_f = 0;
}

void insere_vertice (int p, int a1, int a2)
{
  we.v[p].a1 = a1;
  we.v[p].a2 = a2;
}

int insere_face (int a1, int a2)
{
  if (we.num_f >= MAX2-1)
    runerror (FALTA_MEMORIA);
  we.f[we.num_f].a1 = a1;
  we.f[we.num_f++].a2 = a2;
  return we.num_f-1;
}

void insere_2aresta (int v1, int v2)
{
  if (we.a[v1][v2].uso) return;

  we.a[v1][v2].uso = 1;
  we.a[v2][v1].uso = 2;

  we.f[0].a1 = v1;
  we.f[0].a2 = v2;

  insere_vertice (v1, v1, v2);
  insere_vertice (v2, v1, v2);

  we.a[v1][v2].pcw = we.a[v1][v2].nccw = v1;
  we.a[v1][v2].pccw = we.a[v1][v2].ncw = v2;
  we.a[v1][v2].fcw = we.a[v1][v2].fccw = 0;

  if (animacao) {
    setcolor (YELLOW);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v2].p[X], we.v[v2].p[Y]);
    setcolor (WHITE);
  }
}

void insere_3aresta (int v1, int v2, int v3)
{
  int f;
  if (we.a[v1][v2].uso || we.a[v2][v3].uso || we.a[v1][v3].uso) return;

  we.a[v1][v2].uso = 1;
  we.a[v2][v1].uso = 2;

  we.a[v1][v3].uso = 1;
  we.a[v3][v1].uso = 2;

  we.a[v2][v3].uso = 1;
  we.a[v3][v2].uso = 2;

  we.f[0].a1 = v1;
  we.f[0].a2 = v2;

  insere_vertice (v1, v1, v2);
  insere_vertice (v2, v1, v2);
  insere_vertice (v3, v1, v3);
  f = insere_face (v1, v2);

  we.a[v1][v2].pcw = we.a[v1][v2].nccw = v3;
  we.a[v1][v2].pccw = we.a[v1][v2].ncw = v3;
  we.a[v1][v2].fcw = f;
  we.a[v1][v2].fccw = 0;

  we.a[v1][v3].pcw = we.a[v1][v3].nccw = v2;
  we.a[v1][v3].pccw = we.a[v1][v3].ncw = v2;
  we.a[v1][v2].fcw = 0;
  we.a[v1][v2].fccw = f;

  we.a[v2][v3].pcw = we.a[v2][v3].nccw = v1;
  we.a[v2][v3].pccw = we.a[v2][v3].ncw = v1;
  we.a[v2][v3].fcw = f;
  we.a[v2][v3].fccw = 0;

  if (animacao) {
    setcolor (YELLOW);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v2].p[X], we.v[v2].p[Y]);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v3].p[X], we.v[v3].p[Y]);
    line (we.v[v2].p[X], we.v[v2].p[Y], we.v[v3].p[X], we.v[v3].p[Y]);
    setcolor (WHITE);
  }
}

void insere_aresta_fora (int v1, int v2, int pcw, int ncw)
{
  int t;
  if (we.a[v1][v2].uso) return;

  we.a[v1][v2].uso = 1;
  we.a[v2][v1].uso = 2;

  we.f[0].a1 = v1;
  we.f[0].a2 = v2;

  if (we.a[v2][pcw].uso == 1)
    we.a[v1][v2].nccw = we.a[v2][pcw].ncw;
  else if (we.a[v2][pcw].uso == 2)
    we.a[v1][v2].nccw = we.a[pcw][v2].nccw;
  else /* Erro! */
    exit(1);

  if (we.a[v2][t = we.a[v1][v2].nccw].uso == 1)
    we.a[v2][t].pccw = v1;
  else if (we.a[v2][t].uso == 2)
    we.a[t][v2].pcw = v1;
  else /* Erro !! */
    exit (1);

  if (we.a[v1][ncw].uso == 1)
    we.a[v1][v2].pccw = we.a[v1][ncw].pccw;
  else if (we.a[v1][ncw].uso == 2)
    we.a[v1][v2].pccw = we.a[ncw][v1].pcw;
  else /* Erro! */
    exit(1);

  if (we.a[v1][t = we.a[v1][v2].pccw].uso == 1)
    we.a[v1][t].ncw = v2;
  else if (we.a[v1][t].uso == 2)
    we.a[t][v1].nccw = v2;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].pcw = pcw;

  if (we.a[v2][pcw].uso == 1)
    we.a[v2][pcw].ncw = v1;
  else if (we.a[v2][pcw].uso == 2)
    we.a[pcw][v2].nccw = v1;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].ncw = ncw;

  if (we.a[v1][ncw].uso == 1)
    we.a[v1][ncw].pccw = v2;
  else if (we.a[v1][ncw].uso == 2)
    we.a[ncw][v1].pcw = v2;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].fcw = we.a[v1][v2].fccw = 0;

  if (animacao) {
    setcolor (YELLOW);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v2].p[X], we.v[v2].p[Y]);
    setcolor (WHITE);
  }
}

void exclui_aresta (int v1, int v2)
{
  int t;
  if (!we.a[v1][v2].uso) return;

  if (we.a[v1][v2].uso == 2) {
    t = v1;
    v1 = v2;
    v2 = t;
  }

  if (!we.a[v1][v2].fcw)
    we.f[we.a[v1][v2].fccw].a1 = we.f[we.a[v1][v2].fccw].a2 = -1;
  else if (!we.a[v1][v2].fccw)
    we.f[we.a[v1][v2].fcw].a1 = we.f[we.a[v1][v2].fcw].a2 = -1;
  else /* Erro !! */
    exit (1);

  if (we.a[v1][t = we.a[v1][v2].ncw].uso == 1)
    we.a[v1][t].pccw = we.a[v1][v2].pccw;
  else if (we.a[v1][t].uso == 2)
    we.a[t][v1].pcw = we.a[v1][v2].pccw;
  else /* Erro !! */
    exit (1);

  if (we.a[v1][t = we.a[v1][v2].pccw].uso == 1)
    we.a[v1][t].ncw = we.a[v1][v2].ncw;
  else if (we.a[v1][t].uso == 2)
    we.a[t][v1].nccw = we.a[v1][v2].ncw;
  else /* Erro !! */
    exit (1);

  if (we.a[v2][t = we.a[v1][v2].pcw].uso == 1)
    we.a[v2][t].ncw = we.a[v1][v2].nccw;
  else if (we.a[v2][t].uso == 2)
    we.a[t][v2].nccw = we.a[v1][v2].nccw;
  else /* Erro !! */
    exit (1);

  if (we.a[v2][t = we.a[v1][v2].nccw].uso == 1)
    we.a[v2][t].pccw = we.a[v1][v2].pcw;
  else if (we.a[v2][t].uso == 2)
    we.a[t][v2].pcw = we.a[v1][v2].pcw;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].uso = 0;
  we.a[v2][v1].uso = 0;

  if (animacao) {
    setcolor (YELLOW);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v2].p[X], we.v[v2].p[Y]);
    setcolor (WHITE);
  }

}

void insere_aresta_dentro (int v1, int v2, int pcw, int ncw)
{
  int f, t;
  if (we.a[v1][v2].uso) return;

  we.a[v1][v2].uso = 1;
  we.a[v2][v1].uso = 2;

  f = insere_face (v1, v2);

  if (we.a[v2][pcw].uso == 1)
    we.a[v1][v2].nccw = we.a[v2][pcw].ncw;
  else if (we.a[v2][pcw].uso == 2)
    we.a[v1][v2].nccw = we.a[pcw][v2].nccw;
  else /* Erro! */
    exit(1);

  if (we.a[v2][t = we.a[v1][v2].nccw].uso == 1)
    we.a[v2][t].pccw = v1;
  else if (we.a[v2][t].uso == 2)
    we.a[t][v2].pcw = v1;
  else /* Erro !! */
    exit (1);

  if (we.a[v1][ncw].uso == 1)
    we.a[v1][v2].pccw = we.a[v1][ncw].pccw;
  else if (we.a[v1][ncw].uso == 2)
    we.a[v1][v2].pccw = we.a[ncw][v1].pcw;
  else /* Erro! */
    exit(1);

  if (we.a[v1][t = we.a[v1][v2].pccw].uso == 1)
    we.a[v1][t].ncw = v2;
  else if (we.a[v1][t].uso == 2)
    we.a[t][v1].nccw = v2;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].pcw = pcw;

  if (we.a[v2][pcw].uso == 1)
    we.a[v2][pcw].ncw = v1;
  else if (we.a[v2][pcw].uso == 2)
    we.a[pcw][v2].nccw = v1;
  else /* Erro !! */
    exit (1);

  we.a[v1][v2].ncw = ncw;

  if (we.a[v1][ncw].uso == 1)
    we.a[v1][ncw].pccw = v2;
  else if (we.a[v1][ncw].uso == 2)
    we.a[ncw][v1].pcw = v2;
  else /* Erro !! */
    exit (1);

  if (we.a[v2][pcw].uso == 1) {
    if (we.a[v2][pcw].fcw == 0) {
      we.a[v1][v2].fcw = 0;
      we.a[v1][v2].fccw = f;
    }
    else {
      we.a[v1][v2].fcw = f;
      we.a[v1][v2].fccw = 0;
    }
  }
  else {
    if (we.a[pcw][v2].fccw == 0) {
      we.a[v1][v2].fcw = 0;
      we.a[v1][v2].fccw = f;
    }
    else {
      we.a[v1][v2].fcw = f;
      we.a[v1][v2].fccw = 0;
    }
  }

  if (animacao) {
    setcolor (YELLOW);
    line (we.v[v1].p[X], we.v[v1].p[Y], we.v[v2].p[X], we.v[v2].p[Y]);
    setcolor (WHITE);
  }
}

_aresta *delaunay_nlogn (unsigned int np)
{
  _aresta *resp;
  _point px[MAX_PONTOS];
  unsigned int i, j, k, tam, I, J;

  /* copiamos o vetor de pontos para nao estraga-lo */
  for (i=0; i<np; i++)
    attrib (&px[i], pontos[i]);

  /* ordenamos uma vez no X */
  mindist_sort (px, X, np);

  /*  for (i = 1; i < np; i++)
    if (px[i][X] == px[i-1][X]) return NULL;
    */
  if (animacao) setwritemode (writemode);

  inicializa_we (px, np);

  /* chamamos entao a funcao recursiva que calcula a solucao */
  rec_delaunay_nlogn (0, np);

  tam = 0;
  for (i=0; i<MAX; i++)
    for (j=0; j<MAX; j++)
      if (we.a[i][j].uso == 1) tam++;

  if ((resp = (_aresta *) malloc(sizeof (_aresta) * (tam+1))) == NULL)
    runerror(FALTA_MEMORIA);

  tam = 0;
  for (i=0; i<MAX; i++)
    for (j=0; j<MAX; j++)
      if (we.a[i][j].uso == 1) {
        I = J = MAX;
        for (k=0; k<np; k++) {
          if (pontos[k][X] == px[i][X] && pontos[k][Y] == px[i][Y]) I=k;
          if (pontos[k][X] == px[j][X] && pontos[k][Y] == px[j][Y]) J=k;
        }
        if (I == MAX || J == MAX) runerror(INDEFINIDO);
        resp[tam].inicio = I;
        resp[tam].fim = J;
        if (!is_fecho2 (I, J, np))
          resp[tam].peso = qdist (pontos[I], pontos[J]);
        else
          resp[tam].peso = 0;
        tam++;
      }

  resp[tam].inicio = resp[tam].fim = -1;

  libera_we ();
  if (animacao) {
    setwritemode(0);
    setcolor(WHITE);
  }

  return resp;
}

void rec_delaunay_nlogn (unsigned int ini, unsigned int fim)
{
  int esq, dir, next_esq, next_dir, temp, ql, qr, q;
  if (pare) return;
  if (fim - ini < 2) /* Erro !! */
    return;
  else if (fim - ini == 2)
    insere_2aresta (ini, ini+1);
  else if (fim - ini == 3)
    insere_3aresta (ini, ini+1, ini+2);
  else {
    /* alteracao de num iguais */
    int meio = (fim+ini)/2;
//    while (we.v[meio].p[X] == we.v[meio+1].p[X] && meio<fim-2) meio++;

    rec_delaunay_nlogn (ini, meio);
    rec_delaunay_nlogn (meio, fim);

    next_esq = (fim+ini)/2 - 1;
    next_dir = (fim+ini)/2;

    for (ql = next_esq, esq = 0; esq < MAX; esq++)
      if (we.a[next_esq][esq].uso)
        if (!left (we.v[next_esq].p, we.v[ql].p, we.v[esq].p))
          ql = esq;

    for (qr = next_dir, dir = 0; dir < MAX; dir++)
      if (we.a[next_dir][dir].uso)
        if (left_on (we.v[next_dir].p, we.v[qr].p, we.v[dir].p))
          qr = dir;

    esq = next_esq;
    if (we.a[ql][next_esq].uso == 1) {
      next_esq = we.a[ql][next_esq].nccw;
    }
    else {
      next_esq = we.a[next_esq][ql].ncw;
    }

    dir = next_dir;
    if (we.a[qr][next_dir].uso == 1) {
      next_dir = we.a[qr][next_dir].pcw;
    }
    else {
      next_dir = we.a[next_dir][qr].pccw;
    }

    do {
      temp = -1;
      while (left (we.v[dir].p, we.v[esq].p, we.v[next_esq].p)) {
        temp = next_esq;
        if (we.a[esq][next_esq].uso == 1) {
          next_esq = we.a[esq][next_esq].nccw;
        }
        else {
          next_esq = we.a[next_esq][esq].ncw;
        }
        esq = temp;
      }
      while (left (we.v[dir].p, we.v[esq].p, we.v[next_dir].p)) {
        temp = next_dir;
        if (we.a[dir][next_dir].uso == 1) {
          next_dir = we.a[dir][next_dir].pcw;
        }
        else {
          next_dir = we.a[next_dir][dir].pccw;
        }
        dir = temp;
      }
    }
    while (temp != -1 && !pare);
    if (pare) return;

    insere_aresta_fora (esq, dir, next_dir, next_esq);

    while (!pare) {
      /* determinacao do candidato ql */
      if (we.a[esq][dir].uso == 1)
        ql = we.a[esq][dir].pccw;
      else if (we.a[esq][dir].uso == 2)
        ql = we.a[dir][esq].pcw;
      else /* Erro !! */
        exit (1);

      if (we.a[esq][ql].uso == 1)
        q = we.a[esq][ql].pccw;
      else if (we.a[esq][ql].uso == 2)
        q = we.a[ql][esq].pcw;
      else /* Erro !! */
        exit (1);

      while(left (we.v[esq].p, we.v[dir].p, we.v[ql].p) &&
            incircle (we.v[esq].p, we.v[dir].p, we.v[ql].p, we.v[q].p) >0) {
        exclui_aresta (esq, ql);
        ql = q;
        if (we.a[esq][q].uso == 1)
          q = we.a[esq][q].pccw;
        else if (we.a[esq][q].uso == 2)
          q = we.a[q][esq].pcw;
        else /* Erro !! */
          exit (1);
      }

      if (left_on (we.v[dir].p, we.v[esq].p, we.v[ql].p))
        /* nao ha candidato ql */
        ql = -1;

      /* determinacao do candidato qr */
      if (we.a[dir][esq].uso == 1)
        qr = we.a[dir][esq].ncw;
      else if (we.a[dir][esq].uso == 2)
        qr = we.a[esq][dir].nccw;
      else /* Erro !! */
        exit (1);

      if (we.a[dir][qr].uso == 1)
        q = we.a[dir][qr].ncw;
      else if (we.a[dir][qr].uso == 2)
        q = we.a[qr][dir].nccw;
      else /* Erro !! */
        exit (1);

      while(left (we.v[esq].p, we.v[dir].p, we.v[qr].p) &&
            incircle (we.v[esq].p, we.v[dir].p, we.v[qr].p, we.v[q].p) >0) {
        exclui_aresta (dir, qr);
        qr = q;
        if (we.a[dir][q].uso == 1)
          q = we.a[dir][q].ncw;
        else if (we.a[dir][q].uso == 2)
          q = we.a[q][dir].nccw;
        else /* Erro !! */
          exit (1);
      }
      if (left_on (we.v[dir].p, we.v[esq].p, we.v[qr].p))
        /* nao ha candidato qr */
        qr = -1;

      if (qr == -1 && ql == -1) break;
      if (qr == -1) {
        insere_aresta_dentro (ql, dir, esq, esq);
        esq = ql;
      }
      else if (ql == -1) {
        insere_aresta_dentro (esq, qr, dir, dir);
        dir = qr;
      }
      else {
        if (incircle (we.v[esq].p, we.v[dir].p, we.v[qr].p, we.v[ql].p) >0){
          insere_aresta_dentro (ql, dir, esq, esq);
          esq = ql;
        }
        else {
          insere_aresta_dentro (esq, qr, dir, dir);
          dir = qr;
        }
      }
    }
  }
}
