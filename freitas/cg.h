/*
 * cg.h
 */
#ifndef CG_H
#define CG_H

#define FALSE      0
#define TRUE       !FALSE
#define INFINITO   ((unsigned) -1)

#define MAX_I      10000
#define MAX_PONTOS 200
#define MAX_POLY   10
#define DIM        2
#define X          0
#define Y          1

/* ponto eh um vetor de tamanho dimensoes */
typedef int _point[DIM];

/* poligono eh uma estrutura com o numero de pontos e um vetor */
typedef struct
{
  unsigned int np;
  _point p[MAX_PONTOS];
} _poly;

typedef struct
{
  _point inicio, fim;
} _aresta_p;

typedef struct
{
  int inicio, fim;
  double peso;
} _aresta;

int avalia (char *);
void poucos_pontos (int);
void ok (void);
int poly_simples (_poly *);

int main (int, char *[]);

#endif /* CG_H */
