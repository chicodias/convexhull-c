/*
 * Erro.c
 */
#include "fstdio.h"
#include "fstdlib.h"
#include "erro.h"
#include "fgrafico.h"

void runerror (int codigo)
{
  closegraph();
  switch (codigo)
  {
    case FALTA_MEMORIA: fprintf (stderr, "Not enough memory\n\n");
                        break;
    case INDEFINIDO:    fprintf (stderr, "Run-error\n\n");
                        break;
  }
  exit (1);
}
/* Fim de ERRO.C */
