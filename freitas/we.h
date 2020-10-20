#ifndef WE_H
#define WE_H

void libera_we (void);
void insere_aresta_dentro (int v1, int v2, int pcw, int ncw);
void exclui_aresta (int v1, int v2);
_aresta *delaunay_nlogn (unsigned int np);
void insere_aresta_fora (int v1, int v2, int pcw, int ncw);
void insere_3aresta (int v1, int v2, int v3);
void insere_2aresta (int v1, int v2);
int insere_face (int a1, int a2);
void insere_vertice (int p, int a1, int a2);
void inicializa_we (_point *pontos, unsigned int num);
void rec_delaunay_nlogn (unsigned int ini, unsigned int fim);

#endif
