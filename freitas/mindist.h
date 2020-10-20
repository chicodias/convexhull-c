/*
 * Mindist.h
 */
#ifndef MINDIST_H
#define MINDIST_H

#include "cg.h"

#define minimo(X,Y) ((X)<(Y)?(X):(Y))

double qdist             (_point, _point);
unsigned long qdistancia        (_point , _point);
unsigned long mindist_quad      (_point *, _point *, unsigned int);
unsigned long mindist_nlogn     (_point *, _point *, unsigned int);
unsigned long rec_mindist_nlogn (_point *, _point *, unsigned int,
                                 _point *, _point *);

#endif /* MINDIST_H */
