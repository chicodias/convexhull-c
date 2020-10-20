/*
 * Dupla.h
 */
#ifndef DUPLA_H
#define DUPLA_H

/* lista circular duplamente ligada de inteiros (serao os numeros dos pontos
   no vetor de pontos utilizada) */
typedef struct _cel_dupla *_dupla;
struct _cel_dupla
{
  int i;
  _dupla ante, prox;
};

int    tam1_dupla (_dupla);
_dupla retira_dupla (_dupla, _dupla);
_dupla pega_cel_dupla (void);
_dupla coloca_dupla (int, _dupla);

#endif /* DUPLA_H */
