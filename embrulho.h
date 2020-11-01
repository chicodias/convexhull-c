// file: embrulho.h
// authors:  Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
#ifndef EMBRULHO_H
#define EMBRULHO_H

#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include "primitivas.h"

// retorna uma lista circular com o fecho convexo de uma lista L de pontos.
LISTA * embrulho(LISTA * L);

#endif // embrulho.h
