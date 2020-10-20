/*
 * Fio.c (Funcoes de io)
 */
#include "fstring.h"
#include "fstdio.h"
#include "fstdlib.h"
#include "cg.h"
#include "fio.h"

#define DDEBUG(x)

/* dados (pontos e poligonos) */
extern _poly  poly[];
extern _point pontos[];

/* Funcao que salva em arquivo os pontos e poligonos da memoria. Passamos
   como parametro np o numero de pontos e npoly o numero de poligonos.
   Retorna TRUE em caso de sucesso, FALSE caso contrario.
   A estrutura do arquivo pode ser encontrada em FORM_ARQ.TXT */
int salva_arq (char *nome, int np, int npoly)
{
  FILE *fp;
  int i, j, k;
  DDEBUG(printf("salva_arq nome:%s\n", nome);)
  if ((fp = fopen (nome, "w")) == NULL) /* criou? */
    return (FALSE);

  /* salva pontos */
  fprintf (fp, "--------------------------------------\n");
  fprintf (fp, "CG Program. By Cassio Polpo de Campos.\n");
  fprintf (fp, "--------------------------------------\n\n");
  fprintf (fp, "number of points=%d\n", np);
  for (i=0; i<np; i++)
  {
    fprintf (fp, "\\p %d", pontos[i][0]);
    for (k=1; k<DIM; k++)
      fprintf (fp, " %d", pontos[i][k]);
    fprintf (fp, "\n");
  }

  /* salva poligonos */
  fprintf (fp, "\nnumber of polygons=%d\n", npoly);
  for (i=0; i<npoly; i++)
  {
    fprintf (fp, "\npolygon=%d: number of points=%d\n",
                 i+1, poly[i].np);
    for (j=0; j<poly[i].np; j++)
    {
      fprintf (fp, "P %d", poly[i].p[j][0]);
      for (k=1; k<DIM; k++)
        fprintf (fp, " %d", poly[i].p[j][k]);
      fprintf (fp, "\n");
    }
  }

  fclose (fp);
  return (TRUE);
}

/* Funcao que le de um arquivos pontos e poligonos para serem usados no
   programa. Devolve em np o numero de pontos, em npoly o numero de poligonos
   e a funcao retorna 1 caso sucesso, 0 caso nao achou arquivo, -1 caso o
   arquivo nao eh compativel. */
int le_arq (char *nome, int *np, int *npoly)
{
  int h, i, j, k;
  FILE *fp;
  char buffer[80];
  DDEBUG(printf("le_arq nome:%s", nome);)
  if ((fp = fopen (nome, "r")) == NULL)
    return (0);

  /* Procura pelo numero de pontos */
  *np = -1;
  while (!feof (fp))
  {
    buffer[0] = 0;
    fgets (buffer, 78, fp);
    buffer[16] = 0;
    if (strequal (buffer, "number of points"))
    {
      *np = atoi (&buffer[17]);
      break;
    }
  }

  if (*np == -1) { 
    fseek (fp, 0, SEEK_SET);
    *np = MAX_PONTOS;
  }

  /* Agora procura pelo pontos */
  i = 0;
  while (!feof (fp) && i < *np)
  {
    buffer[0] = 0;
    fgets (buffer, 78, fp);
    if (buffer[0] == '\\' && buffer[1] == 'p' && buffer[2] == ' ') 
    {
      j = 3;
      k = 0;
      while (buffer[j] >= ' ')
      {
        while (buffer[j] == ' ') j++;
        pontos[i][k++] = atoi (&buffer[j]);
        while (buffer[j] > ' ') j++;
      }
      i++;
    }
    else if (buffer[0] == 'p' && buffer[1] == ' ')
    {
      j = 2;
      k = 0;
      while (buffer[j] >= ' ')
      {
        while (buffer[j] == ' ') j++;
        pontos[i][k++] = atoi (&buffer[j]);
        while (buffer[j] > ' ') j++;
      }
      i++;
    }
  }

  if (*np == MAX_PONTOS) *np = i;
  if (i != *np) /* se dados incompativeis */
    return (-1);

  /* Procura pelo numero de poligonos */
  *npoly = -1;
  while (!feof (fp))
  {
    buffer[0] = 0;
    fgets (buffer, 78, fp);
    buffer[18] = 0;
    if (strequal (buffer, "number of polygons"))
    {
      *npoly = atoi (&buffer[19]);
      break;
    }
  }

  /* Agora pelos poligonos */
  i = 0;
  while (!feof (fp) && i < *npoly)
  {
    while (!feof (fp))
    {
      buffer[0] = 0;
      fgets (buffer, 78, fp);
      buffer[7] = 0;
      if (strequal (buffer, "polygon"))
        break;
    }
    if (!feof (fp)) /* se achou poligono */
    {
      j = 0;
      while (buffer[j] != ':') j++;
      j++;
      while (buffer[j] == ' ') j++;
      buffer[j+16] = 0;
      if (strequal (&buffer[j], "number of points"))
        poly[i].np = atoi (&buffer[j+17]);   /* acha seu num de pontos */
      else
        return (-1);

      h = 0;
      while (!feof (fp) && h < poly[i].np) /* agora acha os proprios pontos */
      {
        buffer[0] = 0;
        fgets (buffer, 78, fp);
        if (buffer[0] == 'P' && buffer[1] == ' ')
        {
          j = 2;
          k = 0;
          while (buffer[j] >= ' ')
          {
            while (buffer[j] == ' ') j++;
            poly[i].p[h][k++] = atoi (&buffer[j]);
            while (buffer[j] > ' ') j++;
          }
          h++;
        }
      }
      if (h != poly[i].np) /* se poligono incompativel */
        return (-1);
      i++;
    }
  }
  
  if (*npoly == -1) *npoly = 0;
  if (i != *npoly) /* se numero de poligonos incompativeis */
    return (-1);
  return (1);
}
/* Fim de FIO.C */
