/*
 * Vetor.c
 */
#include "fctype.h"
#include "cg.h"
#include "fgrafico.h"
#include "fconio.h"
#include "vetor.h"
/**/
#include "mindist.h"

/* contadores de primitivas */
extern long count[3];
/* variaveis de controle */
extern int animacao, delay_time, pare, indcount;

/* pontos */
extern _point pontos[MAX_PONTOS];

/* Retorna se a aresta ij nao possui nenhum vertice a sua direita */
int is_fecho (int i, int j, unsigned int np)
{
  int k, a = animacao;
  animacao = 0;
  for (k=0; k<np; k++)
    if (right (pontos[i], pontos[j], pontos[k]))
      break;

  animacao = a;
  return (k<np)? 0: 1;
}


/* calcula o quadrado da area entre a, b, c, utilizando a regra do
   determinante e presenvando o sinal negativo, caso exista, para uso
   posterior. */
long area2 (_point a, _point b, _point c)
{
  if (animacao && 0)
  {
    int atual = COR, mode = WRITEMODE;
    setwritemode(1);
    setcolor (LIGHTBLUE);
    line (a[X], a[Y], b[X], b[Y]);
    line (a[X], a[Y], c[X], c[Y]);
    line (c[X], c[Y], b[X], b[Y]);


    if (delay_time < 0)
    {
      if (toupper(getch()) == 'S') /* deseja parar no meio? */
        pare=1;
    }
    else delay (delay_time);

    line (a[X], a[Y], b[X], b[Y]);
    line (a[X], a[Y], c[X], c[Y]);
    line (c[X], c[Y], b[X], b[Y]);
    setcolor (atual);
    setwritemode(mode);
  }
  if (kbhit()) if (toupper(getch()) == 'S') pare=1;
  count[indcount]++;

  return((long)((double)a[0] * (double)b[1] - (double)a[1] * (double)b[0] +
                (double)a[1] * (double)c[0] - (double)a[0] * (double)c[1] +
                (double)b[0] * (double)c[1] - (double)c[0] * (double)b[1]));
                /* o que eh necessario para ele nao se perder nas contas !? */
}

/* retorna o ou-exclusivo entre dois inteiros em 0,1 */
int xor (int x, int y)
{
  return (!x ^ !y);
}

/* Retorna se c esta estritamente `a esquerda do vetor ab */
int left (_point a, _point b, _point c)
{
  return (area2 (a, b, c) > 0L);
}

/* Retorna se c esta `a esquerda ou sobre o vetor ab */
int left_on (_point a, _point b, _point c)
{
  return (area2 (a, b, c) >= 0L);
}

/* Retorna se a, b, c sao colineares */
int collinear (_point a, _point b, _point c)
{
  return (area2 (a, b, c) == 0L);
}

/* Coloca (a - b) em c. */
void subvec (_point a, _point b, _point c)
{
  int i;

  for(i=0; i<DIM; i++)
    c[i] = a[i] - b[i];
}

int dot (_point a, _point b)
{
  int i;
  int sum = 0;

  for (i=0; i<DIM; i++)
    sum += sum + a[i] * b[i];

  return (sum);
}

/* usa a funcao acima para calcula o tamanho ao quadrado do vetor p */
int length2 (_point p)
{
  return (dot (p, p));
}

/* atribui ao ponto x as coordenadas do ponto y */
void attrib (_point *x, _point y)
{
  int i;
  for (i=0; i<DIM; i++)
    (*x)[i] = y[i];
}

/* Retorna se ha interseccao entre os segmentos ab e cd */
int intersec (_point a, _point b, _point c, _point d)
{
  if (collinear (a, b, c) && collinear (a, b, d)) {
    int i1 = (a[X] < b[X])? a[X]: b[X],
        f1 = (a[X] < b[X])? b[X]: a[X],
        i2 = (c[X] < d[X])? c[X]: d[X],
        f2 = (c[X] < d[X])? d[X]: c[X];
    if ((i1 == i2 && f1 == f2) ||
        (i1 < i2 && i2 < f1)   ||
        (i1 < f2 && f2 < f1)   ||
        (i2 < i1 && i1 < f2)   ||
        (i2 < f1 && f1 < f2))
      return TRUE;
    else return FALSE;
  }

  return cruza (a, b, c, d);
}

int cruza (_point a, _point b, _point c, _point d)
{
  if (collinear (a, b, c) ||
      collinear (a, b, d) ||
      collinear (c, d, a) ||
      collinear (c, d, b))
    return FALSE;

  return (xor (left (a, b, c), left (a, b, d)) &&
          xor (left (c, d, a), left (c, d, b)));
}

/* Verifica a posicao do ponto d em relacao a circunferencia definida
   pelos pontos a,b,c */
long double _incircle (_point a, _point b, _point c, _point d)
{
  int ani = animacao;
  long double temp;
  animacao = FALSE;
  if (ani) {
    /* vai ser meio complicado !! */
  }
  /* chamamos area2 quatro vezes. Entao temos que decrementar count 3 vezes
     para que ele conte o numero de chamadas a funcao incircle */
  count[indcount]-=3;

  temp = (long double) ((long double)a[X]*a[X]+(long double)a[Y]*a[Y]) * area2 (b,c,d) -
         (long double) ((long double)b[X]*b[X]+(long double)b[Y]*b[Y]) * area2 (a,c,d) +
         (long double) ((long double)c[X]*c[X]+(long double)c[Y]*c[Y]) * area2 (a,b,d) -
         (long double) ((long double)d[X]*d[X]+(long double)d[Y]*d[Y]) * area2 (a,b,c);

  animacao = ani;
  return temp;
}

int incircle (_point a, _point b, _point c, _point d)
{
  long double l = _incircle (a,b,c,d);
  if (l > 0) return 1;
  if (l < 0) return -1;
  return 0;
}
/* Fim de VETOR.C */
