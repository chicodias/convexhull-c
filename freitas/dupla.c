/*
 * Dupla.c
 */
#include "fmalloc.h"
#include "dupla.h"
#include "erro.h"

/* Funcao que retorna TRUE se a lista d tem somente um elemento */
int tam1_dupla (_dupla d)
{
  return (d->prox == d);
}

/* Funcao que retira d da lista inicio, retornando um apontador para a
   nova lista apos a exclusao */
_dupla retira_dupla (_dupla d, _dupla inicio)
{
  if (d != NULL) /* se nao vazia */
  {
    if (tam1_dupla (d)) /* se tamanho 1 entao vai ficar vazia */
      inicio = NULL;
    if (d == inicio)
      inicio = inicio->prox;

    (d->ante)->prox = d->prox;
    (d->prox)->ante = d->ante;
    free (d);
  }
  return (inicio);
}

/* Funcao de alocacao dinamica de celula para uma lista. Devolve apontador
   para a celula */
_dupla pega_cel_dupla (void)
{
  _dupla d;

  if ((d = (_dupla) malloc (sizeof (struct _cel_dupla))) == NULL)
    runerror (FALTA_MEMORIA);
  return (d);
}

/* Funcao que coloca o elemento i na lista d, retornando a nova lista apos
   a insercao. */
_dupla coloca_dupla (int i, _dupla d)
{
  _dupla aux = pega_cel_dupla ();
  aux->i = i;  /* cria celula com o valor i */

  if (d == NULL) /* se vazia */
  {
    aux->prox = aux;
    aux->ante = aux;
  }
  else /* senao */
  {
    aux->prox = d;
    aux->ante = d->ante;

    (aux->prox)->ante = aux;
    (aux->ante)->prox = aux;
  }

  return (aux);
}
/* Fim de DUPLA.C */
