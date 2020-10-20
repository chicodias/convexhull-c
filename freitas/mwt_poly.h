#ifndef MWT_POLY
#define MWT_POLY

typedef struct {
  double matriz[MAX_PONTOS+3][MAX_PONTOS+3];
  int escolha_k[MAX_PONTOS+3][MAX_PONTOS+3];
  int n;
} estrutura;

estrutura *procura_otimo(_poly poly);
int is_diagonal (int i, int f, _poly p);
int is_internal (_point p, _poly poly);
void mostra_diagonais (estrutura *e, int i, int j);
double mwt_poligono (_poly poly);

#endif
