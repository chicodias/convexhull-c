/* 
 * File:   pilha.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
 */

#include "pilha.h"
#include "lista.h"


// aloca uma pilha no heap e devolve seu endereço
PILHA * criapilha (void) 
{
   PILHA * pi = malloc (sizeof (PILHA)); // cabeça
   pi->prox = NULL; 
   pi->ponto = NULL;
   return pi;
}

// empilha um NO *
void empilha (PILHA * pi, NO * y) 
{ 
   PILHA * nova;
   nova = malloc (sizeof (PILHA));
   nova->ponto = y;
   nova->prox  = pi->prox;
   pi->prox = nova; 
}

// retorna um NO desempilhado
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

// retorna o topo de uma pilha sem removê-lo
PILHA * topo(PILHA * pi)
{
    return pi->prox;
}

// aloca uma LISTA a partir de uma pilha, desempilhando os elementos
// e liberando a pilha. retorna o endereço da lista.
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
   
   return M;
}
