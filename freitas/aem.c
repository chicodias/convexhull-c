/*
 * AEM.c
 */
#include "fctype.h"
#include "fmalloc.h"
#include "cg.h"
#include "erro.h"
#include "fgrafico.h"
#include "fconio.h"
#include "mindist.h"
#include "sort.h"
#include "vetor.h"
#include "pilha.h"

#define DDEBUG(x)

/* contador de primitivas */
extern long count[3];
/* vetor com os pontos */
extern _point pontos[MAX_PONTOS];
/* variaveis de controle */
extern int animacao, delay_time, pare, indcount, writemode;

_aresta *aem (unsigned int np, _aresta *ent)
{
  char matriz[MAX_PONTOS][MAX_PONTOS];
  char visto[MAX_PONTOS];
  int i, j, k, tam;
  _pilha pilha = NULL;
  _aresta *a;
  _aresta *resp;

  if (animacao)
  {
    setwritemode (writemode);
    setcolor (LIGHTBLUE);
  }

  for (i=0; i<np; i++)
    for (j=0; j<np; j++)
      matriz[i][j] = 0;

  for (tam=0, a=ent; a->inicio != -1; a++) tam++; 

  /* ordenamos em relacao ao peso para o funcionamento correto do algoritmo */
  aem_sort (ent, tam);

  /* Agora para cada aresta da entrada, colocamos ela na solucao se ela nao
     formar circuito com as ja presentes */
  for (i=0, a=ent; a->inicio != -1 && !pare; a++) {
    if (animacao) {
      line (pontos[a->inicio][X], pontos[a->inicio][Y], 
            pontos[a->fim][X],    pontos[a->fim][Y]);
      if (delay_time < 0) {
	  if (toupper(getch()) == 'S') /* deseja parar no meio? */
	    pare=1;
      }
      else delay (delay_time);
      line (pontos[a->inicio][X], pontos[a->inicio][Y], 
            pontos[a->fim][X],    pontos[a->fim][Y]);
    }
    if (kbhit()) if (toupper(getch()) == 'S') pare=1;
    count[indcount]++;

    /* busca em profundidade */
    for (k=0; k<np; k++)
      visto[k] = FALSE;
    pilha = empilha (a->inicio, pilha);
    while (pilha != NULL) {
      j = pilha->i;
      visto[j] = 1;
      if (j == a->fim) break;
      pilha = desempilha (pilha);
      for (k=0; k<np; k++)
        if (matriz[j][k] && !visto[k])
          pilha = empilha (k, pilha);
    }
    if (pilha == NULL) {
      /* colocamos o peso negativo como um flag para dizer que esta na solucao */
      a->peso = -(a->peso);
      matriz[a->inicio][a->fim] = TRUE;
      matriz[a->fim][a->inicio] = TRUE;
      i++;
      if (animacao) {
        setcolor (GREEN);
        line (pontos[a->inicio][X], pontos[a->inicio][Y], 
              pontos[a->fim][X],    pontos[a->fim][Y]);
        setcolor (LIGHTBLUE);
      }
    }
    else
      while (pilha != NULL) pilha = desempilha (pilha);
  }

  if ((resp = (_aresta *) malloc (sizeof(_aresta) * (i+1))) == NULL)
    runerror (FALTA_MEMORIA);
  for (tam=0, a=ent; a->inicio != -1; a++)
    if (a->peso < 0) {
      resp[tam].inicio = a->inicio;
      resp[tam].fim    = a->fim;
      resp[tam].peso   = -(a->peso);
      tam++;
    } 

  resp[tam].inicio = resp[tam].fim = -1;

  if (animacao)
  {
    setcolor (WHITE);
    setwritemode (0);
  }

  return resp;
}

void tree_to_poly (int np, _aresta *ent, _poly *poly)
{
  _aresta *a;
  char matriz[MAX_PONTOS][MAX_PONTOS];
  int i, j = 0, tam, esq, inicio = -1, atual = -1, ant = -1, from = -1;

  for (i=0; i<np; i++)
    for (j=0; j<np; j++)
      matriz[i][j] = 0;

  for (a=ent; a->inicio != -1; a++) {
    matriz[a->inicio][a->fim] = 1;
    matriz[a->fim][a->inicio] = -1;
  }

  for (i=0; i<np; i++)
    for (j=0; j<np; j++)
      if (i != j && is_fecho (i, j, np) && !matriz[i][j]) {
        from   = i;
        inicio = j;
        matriz[i][j] = 1;
        matriz[j][i] = -1;

        ant   = i;
        atual = j;
      }

  if (inicio == -1)
    runerror (INDEFINIDO);

  tam = 0;
  do {
    esq = -1;
    for (i=0; i<np; i++) {
      DDEBUG (printf ("\ni=%d atual=%d inicio=%d ant=%d esq=%d matriz[atual][i]=%d", 
              i, atual, inicio, ant, esq, matriz[atual][i]);)
      if (i != ant && matriz[atual][i] != 0) {
        if (esq == -1) esq = i;
        else {
          if (right (pontos[ant], pontos[atual], pontos[esq]) &&
              left  (pontos[ant], pontos[atual], pontos[i])) esq = i;
          else {
            if ( (right (pontos[ant], pontos[atual], pontos[esq]) &&
                  right (pontos[ant], pontos[atual], pontos[i])) 
                 ||
                 (left  (pontos[ant], pontos[atual], pontos[esq]) &&
                  left  (pontos[ant], pontos[atual], pontos[i]))  )
              if (left (pontos[atual], pontos[esq], pontos[i])) esq = i;
          }
        }
      }
    }

    if (esq == -1) esq = ant;

    DDEBUG (printf ("\nLigando %d a %d", atual, esq);)
    attrib (&(poly->p[tam]), pontos[esq]);
    tam++;
    ant   = atual;
    atual = esq;
  } while (atual != inicio || ant != from);

  DDEBUG (for (i=0; i<np; i++)
    printf ("\ni=%d X=%d Y=%d", i, pontos[i][X], pontos[i][Y]);)

  poly->np = tam;
}
