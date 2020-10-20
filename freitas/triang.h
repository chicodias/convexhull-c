/*
 * Triang.h
 */
#ifndef TRIANG_H
#define TRIANG_H

#include "cg.h"

#define pol(X) poligono.p[(X)]
#define folha(X) ((X)->esq == NULL && (X)->dir == NULL)

#define TEM_BICO -13
#define VAZIO -1

typedef struct _dponto
{
  int prox, ante;
} tdponto[MAX_PONTOS];

/* estrutura da arvore23
   Nos nos internos teremos como informacao o numero da aresta.
   Nos nos externos a informacao sera o numero de um ponto.
   A celula eh do mesmo tipo, mudando apenas a informacao nela contida. */
typedef struct _cel_tree23 *_tree23;
struct _cel_tree23
{
  _tree23 esq, meio, dir, pai;
  int einfo, dinfo;
};

/* estrutura de uma arvore binaria
   Nela iremos guardar as diagonais tracadas pelo algoritmo de varredura,
   para que atraves desta arvore possamos subdividir o poligono em poligonos
   monotonos eficientemente. */
typedef struct _s *ts;
struct _s
{
  int i, f;
  ts dir, esq;
};

/* estrutura onde guardaremos temporariamente as diagonais citadas acima. */
typedef struct
{
  int i, f;
} tlig;

int baixo (int);
int cima (int);
int esquerda (int, int, int);

_tree23 pega_cel_tree23 (void);
_tree23 inicio_tree23   (void);
_tree23 acha_aresta23   (int, _tree23);
_tree23 acha_vertice23  (int, _tree23);
_tree23 vertice_esq     (int, _tree23);
_tree23 vertice_dir     (int, _tree23);
_tree23 folha_esq       (_tree23);
_tree23 folha_dir       (_tree23);

tlig subst              (int, int, _tree23);
_tree23 retira_tree23a  (int, _tree23, _tree23);
_tree23 retira_tree23   (int, _tree23);
_tree23 coloca_tree23   (int, _tree23);

int  is_sentido_horario (_poly);
double percorre (ts, tdponto, int);
double monotone (_poly, tdponto, int);
double triang   (_poly);

#endif /* TRIANG_H */
