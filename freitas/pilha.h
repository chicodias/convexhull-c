/*
 * Pilha.h
 */
#ifndef PILHA_H
#define PILHA_H

/* estrutura de uma pilha de inteiros (serao numeros dos pontos) */
typedef struct _celula *_pilha;
struct _celula
{
  int i;
  _pilha prox;
};

_pilha xpush       (_pilha, _pilha);
_pilha desempilha  (_pilha);
_pilha pega_celula (void);
_pilha empilha     (int, _pilha);

#endif /* PILHA_H */
