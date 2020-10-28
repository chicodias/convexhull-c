#include "pilha.h"
#include "lista.h"



PILHA * criapilha (void) 
{
   PILHA * pi = malloc (sizeof (PILHA)); // cabeça
   pi->prox = NULL; 
   pi->ponto = NULL;
   return pi;
}

void empilha (PILHA * pi, NO * y) 
{ 
   PILHA * nova;
   nova = malloc (sizeof (PILHA));
   nova->ponto = y;
   nova->prox  = pi->prox;
   pi->prox = nova; 
}

NO * desempilha (PILHA * pi) 
{
   if(topo(pi) == NULL)
      return NULL;

   PILHA * p;
   p = pi->prox;
   NO * x = p->ponto;
   pi->prox = p->prox;
   free (p);
   return x; 
}

boolean pilha_vazia(PILHA * pi)
{
   return (pi ->prox != NULL);
}


PILHA * topo(PILHA * pi)
{
    return pi->prox;
}

LISTA * pilhatolist(PILHA * pi)
{
   LISTA * M = lista_criar();
   NO * t;
   
   while (topo(pi) != NULL)
   {
      t = desempilha(pi);
      lista_inserir(M, t->x, t->y);
   }
   free(pi);
/* 
   t = M->inicio;

   while (t->prox != NULL)
      t = t->prox;

   t ->prox = M->inicio; */
   
   return M;
}
