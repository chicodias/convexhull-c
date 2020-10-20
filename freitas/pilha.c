/*
 * Pilha.c
 */
#include "fmalloc.h"
#include "erro.h"
#include "pilha.h"

/* Funcao que empilha p sobre a pilha t, devolvendo p como a nova pilha */
_pilha xpush (_pilha t, _pilha p)
{
  p->prox = t;
  return (p);
}

/* Funcao que desempilha de p um elemento e retorna um apontador para a nova
   pilha apos a remocao */
_pilha desempilha (_pilha p)
{
  _pilha aux = p;
  if (p != NULL)
  {
    p = p->prox;
    free ((void *) aux);
  }
  return (p);
}

/* Funcao de alocacao dinamica de uma celula para pilha */
_pilha pega_celula (void)
{
  _pilha p;

  if ((p = (_pilha) malloc (sizeof (struct _celula))) == NULL)
    runerror (FALTA_MEMORIA);
  return (p);
}

/* Funcao que empilha i na pilha topo, retornando apontador para a nova
   pilha apos a insercao */
_pilha empilha (int i, _pilha topo)
{
  _pilha p;

  p    = pega_celula ();
  p->i = i;
  return (xpush (topo, p));
}
/* Fim de PILHA.C */
