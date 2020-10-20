/*
 * MinDist.c
 */
#include "fctype.h"
#include "fmalloc.h"
#include "cg.h"
#include "erro.h"
#include "fgrafico.h"
#include "fconio.h"
#include "mindist.h"
#include "sort.h"
#include "vetor.h"
#include <math.h>

/* contador de primitivas */
extern long count[3];
/* vetor com os pontos */
extern _point pontos[MAX_PONTOS];
/* variaveis de controle */
extern int animacao, delay_time, pare, indcount, writemode;

/* Funcao que encontra o par de pontos mais proximos, por busca exaustiva.
   Recebe np o numero de pontos, utiliza o vetor 'pontos' e retorna em a,b
   o par mais proximo, cuja distancia ao quadrado eh retornada pela funcao */
unsigned long mindist_quad (_point *a, _point *b, unsigned int np)
{
  unsigned long t, min = (unsigned long) (-1);
  _point p[MAX_PONTOS];
  unsigned int i, j;

  if (animacao)
    setwritemode (writemode);

  /* copiamos o vetor de pontos para nao estragar o original */
  for (i=0; i<np; i++)
    attrib (&p[i], pontos[i]);

  /* ordenamos para uma melhora singela do algoritmo */
  mindist_sort (p, X, np);

  /* Agora para cada par de pontos, calculamos a distancia e verificamos se
     esta distancia eh minima, alterando o minimo se necessario */
  for (i=0; i<np && !pare; i++)
    for (j=i+1; j<np && !pare; j++)
    {
      if (min >= (t = qdistancia (p[i], p[j])))
      {
	attrib (a, p[i]);
	attrib (b, p[j]);
	min = t;
      }
    }

  if (animacao)
  {
    setcolor (WHITE);
    setwritemode (0);
  }

  return (min);
}

double qdist (_point a, _point b)
{
  double t;
  int ani = animacao;
  animacao = 0;
  t = sqrt ((double) qdistancia(a,b));
  animacao = ani;
  return t;
}

/* Funcao que calcula e a distancia ao quadrado entre dois pontos a e b */
unsigned long qdistancia (_point a, _point b)
{
  unsigned long t = 0;
  int i;
  count[indcount]++;
  if (animacao && 0)
  {
    setcolor (GREEN);
    line (a[X], a[Y], b[X], b[Y]);

    if (delay_time < 0)
    {
      if (toupper (getch()) == 'S') pare=TRUE;
    }
    else
      delay (delay_time);
    line (a[X], a[Y], b[X], b[Y]);
  }
  if (kbhit()) if (toupper(getch()) == 'S') pare=TRUE;

  for (i=0; i<DIM; i++)
    t += ((unsigned long)a[i]-(unsigned long)b[i])*
	 ((unsigned long)a[i]-(unsigned long)b[i]);

  return (t);
}

/* distancia minima atual */
static unsigned long mini;

/* Funcao que encontra o par mais proximo em nlogn. Recebe como entrada np
   o numero de pontos e retorna em a, b o par mais proximo. A funcao
   retorna o quadrado da distancia entre este par mais proximo */
unsigned long mindist_nlogn (_point *a, _point *b, unsigned int np)
{
  _point px[MAX_PONTOS], py[MAX_PONTOS];
  unsigned int i;
  unsigned long r;

  if (animacao)
    setwritemode (writemode);

  /* copiamos o vetor de pontos para nao estraga-lo */
  for (i=0; i<np; i++)
  {
    attrib (&px[i], pontos[i]);
    attrib (&py[i], pontos[i]);
  }

  /* ordenamos uma vez no X */
  mindist_sort (px, X, np);
  /* e uma vez no Y */
  mindist_sort (py, Y, np);

  /* maximizamos a distancia minima inicial */
  mini = (unsigned) (-1);

  /* chamamos entao a funcao recursiva que calcula a solucao */
  r = rec_mindist_nlogn (px, py, np, a, b);

  if (animacao)
  {
    setcolor (WHITE);
    setwritemode (0);
  }
  return (r);

}

/* Funcao que recebe um vetor de pontos de tamanho tam e encontra o par mais
   proximo devolvendo em a,b e retornando o quadrado da distancia entre este
   par. */
unsigned long rec_mindist_nlogn (_point *px, _point *py, unsigned int tam,
				 _point *a, _point *b)
{
  unsigned int i, j, k, l;
  unsigned long d, d2;
  unsigned long s1, s2, s3;
  _point *p1, *p2, f, g;
  if (pare)
    return (0);
  if (tam < 4) /* se temos menos que 4 pontos */
  {
    attrib (a, px[0]);
    attrib (b, px[0]);
    if (tam==1) /* se temos apenas um, retornamos um valor grande */
      return ((unsigned long) (-1));

    /* calculamos a distancia entre (a,b) e (a,c) */
    d = qdistancia (px[0], px[1]);
    d2 = qdistancia (px[0], px[tam-1]);

    if (d > d2)  /* seguramos a menor */
    {
      attrib (b, px[tam-1]);
      d = d2;
      mini = (d < mini) ? d : mini;
    }
    else
      attrib (b, px[1]);
    if (tam==2)  /* se tam=2, entao retornamos a distancia */
      return (d);

    /* se tam>2, calculamos a distancia entra (b,c) */
    d2 = qdistancia (px[1], px[tam-1]);

    if (d > d2)  /* e continuamos segurando a menor */
    {
      d = d2;
      attrib (a, px[1]);
      attrib (b, px[tam-1]);
      mini = (d < mini) ? d : mini;
    }
    return (d);
  }

  /* alocamos memoria para duas metades */
  if ((p1 = (_point *) malloc (sizeof (_point)*((int)tam/2))) == NULL)
    runerror (FALTA_MEMORIA);
  if ((p2 = (_point *) malloc (sizeof (_point)*(tam-((int)tam/2)))) == NULL)
    runerror (FALTA_MEMORIA);

  /* e entao dividimos o problema em duas metades */
  for (i=0, j=0, k=0; i<tam; i++)
    if (py[i][X] <= px[((int)tam/2)-1][X])
    {
      for (l=0; l<DIM; l++)
	p1[j][l] = py[i][l];
      j++;
    }
    else
    {
      for (l=0; l<DIM; l++)
	p2[k][l] = py[i][l];
      k++;
    }

  if (animacao)
  {
    /* mostramos o local da divisao */
    line_xor (px[((int)tam/2)-1][X], 3);
    delay (((int)delay_time/2));
  }

  /* calculamos recursivamente para as duas metades */
  d = rec_mindist_nlogn (px, p1, ((int)tam/2), a, b);
  d2 = rec_mindist_nlogn (px+((int)tam/2), p2, tam-((int)tam/2), &f, &g);

  if (d > d2) /* seguramos a menor */
  {
    attrib (a, f);
    attrib (b, g);
    d = d2;
    mini = (d < mini) ? d : mini;
  }

  free (p1);
  free (p2);

  /* Ja fizemos a divisao, agora vamos fazer a 'conquista' */

  /* Alocamos memoria */
  if ((p1 = (_point *) malloc (sizeof (_point)*tam)) == NULL)
    runerror (FALTA_MEMORIA);
  s1 = px[((int)tam/2)-1][X]-mini;
  s2 = px[((int)tam/2)-1][X]+mini;
  s3 = px[((int)tam/2)-1][X];

  if (animacao)
  {
    line_xor ((int) s1, 1);
    line_xor ((int) s2, 1);
    if (delay_time >= 0) delay (((int)delay_time/2));
    else                 if (toupper (getch()) == 'S') pare=TRUE;
  }

  /* so precisamos olhar para alguns pontos na conquista (no maximo 7) */
  for (i=0, j=0; i<tam; i++)
    if ((s3-py[i][X])*(s3-py[i][X]) <= mini)
    {
      for (l=0; l<DIM; l++)
	p1[j][l] = py[i][l];
      j++;
    }

  /* entao olhamos para eles em tempo linear */
  for (i=0; i<j; i++)
    for (k=i+1; k<i+1+7 && k<j; k++)
    {
      d2 = qdistancia (p1[i], p1[k]);
      if (d2 < d)
      {
	attrib (a, p1[i]);
	attrib (b, p1[k]);
	d = d2;
	mini = (d < mini) ? d : mini;
      }
    }

  free (p1);

  if (animacao)
  {
    line_xor ((int) s1, -1);
    line_xor ((int) s2, -1);
    line_xor (px[((int)tam/2)-1][X], -3);
    if (delay_time >= 0) delay (((int)delay_time/2));
    else                 if (toupper (getch()) == 'S') pare=TRUE;
  }

  return (d);
}
/* Fim de MINDIST.C */
