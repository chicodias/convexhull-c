#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "erro.h"

typedef struct s_conj_i _conj_i;

struct s_conj_i {
  void *dado;
  _conj_i *next;
};

typedef struct {
  int tam_elem;
  _conj_i *info;
} _conjunto;

_conj_i *cj_pertence (void *elem, _conjunto *conj)
{
  _conj_i *i;
  for (i=conj->info; i; i=i->next)
    if (memcmp (elem, i->dado, conj->tam_elem)) break;
  return i;
}

_conjunto *cj_inicializa (int tam_elem)
{
  _conjunto *c;
  if ((c = malloc (sizeof (_conjunto))) == NULL)
    runerror(FALTA_MEMORIA);
  c->tam_elem = tam_elem;
  c->info = NULL;
  return c;
}

_conj_i *cj_insere (void *elem, _conjunto *conj)
{
  _conj_i *c;
  if ((c = cj_pertence (elem, conj)) == NULL) {
    if ((c = malloc (sizeof (_conj_i))) == NULL)
      runerror(FALTA_MEMORIA);
    if ((c->dado = malloc (conj->tam_elem)) == NULL)
      runerror(FALTA_MEMORIA);
    memcpy (c->dado, elem, conj->tam_elem);
    c->next = conj->info;
    conj->info = c;
  }
  return c;
}

int cj_remove (void *elem, _conjunto *conj)
{
  _conj_i *c, *i;
  if ((c = cj_pertence (elem, conj)) == NULL)
    return 0;
  if (conj->info == c) conj->info = c->next;
  else {
    i = conj->info;
    while (i->next != c) {
      i = i->next;
      if (i == NULL) runerror(INDEFINIDO);
    }
    i->next = c->next;
  }
  free (c);

  return 1;
}
