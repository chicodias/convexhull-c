/*
 * Graham.c
 */
#include "fstdlib.h"
#include "cg.h"
#include "fgrafico.h"
#include "graham.h"
#include "pilha.h"
#include "sort.h"
#include "vetor.h"

/* vetor com pontos */
extern _point pontos[];
/* variaveis de controle */
extern animacao, pare, writemode;

/* Funcao que calcula o fecho convexo utilizando o algoritmo de Graham.
   Recebe n o numero de pontos e o vetor 'pontos' e retorna a resposta
   numa pilha */
_pilha graham (unsigned int n)
{
  unsigned i;
  _pilha topo;

  if (animacao)
  {
    setwritemode (writemode);
    setcolor (WHITE);
  }

  /* encontra o menor e coloca-o na posicao 0 */
  findlowest (pontos, n);
  /* ordena os pontos de modo eficiente */
  graham_sort (pontos, n);

  /* Inicializa a pilha com pontos[n-1] e pontos[0] */
  topo = NULL;
  topo = empilha (n-1, topo);
  topo = empilha (0, topo);

  /* A pilha nunca tera menos que dois elementos */
  i = 1;

  /* enquanto nao olhamos todos os pontos */
  while (i<n && !pare)
    /* se esta para esquerda de (topo->prox, topo), entao empilhamos e
       andamos para o proximo */
    if (left (pontos[(topo->prox)->i], pontos[topo->i], pontos[i]))
    {
      topo = empilha (i, topo);
      if (animacao)
      {
        line (pontos[(topo->prox)->i][X], pontos[(topo->prox)->i][Y],
              pontos[topo->i][X], pontos[topo->i][Y]);
      }
      i++;
    }
    else /* senao desempilhamos um elemento que era 'errado' */
    {
      if (animacao)
      {
        line (pontos[(topo->prox)->i][X], pontos[(topo->prox)->i][Y],
              pontos[topo->i][X], pontos[topo->i][Y]);
      }
      topo = desempilha (topo);
    }

  /* pontos[n-1] foi empilhado duas vezes, entao desempilhamos uma. */
  if (animacao)
  {
    line (pontos[0][X], pontos[0][Y],
          pontos[topo->i][X], pontos[topo->i][Y]);
    setwritemode (0);
    setcolor (WHITE);
  }
  return (desempilha (topo));

}

/* Funcao que encontra o menor elemento (menor coordenada Y) e coloca-o
   na posicao 0. Recebe o numero n de pontos e o vetor p. */
void findlowest (_point *p, unsigned int n)
{
  unsigned i, m;
  _point aux;

  m = 0;
  /* encontra o 'menor' */
  for (i=1, m=0; i<n; i++)
    if ((p[i][Y] < p[m][Y]) ||
       ((p[i][Y] == p[m][Y]) && (p[i][X] > p[m][X])))
      m = i;

  /* agora coloca-o na posicao 0 */
  attrib (&aux, p[m]);
  attrib (&p[m], p[0]);
  attrib (&p[0], aux);
}
/* Fim de GRAHAM.C */
