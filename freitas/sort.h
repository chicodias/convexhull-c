/*
 * Sort.h
 */

#ifndef SORT_H
#define SORT_H

#include "cg.h"

#define qsort(A,B,C,D) qsort((A),(B),(C),(D))

int  mindist_comp (const void *, const void *);
void mindist_sort (_point *, int, unsigned int);

int  graham_comp (const void *, const void *);
void graham_sort (_point *, unsigned int);

int  triang_comp (const void *, const void *);
void triang_sort (int *, int, unsigned int);

int  guloso_comp (const void *, const void *);
void guloso_sort (_aresta_p *, unsigned int);

int  aem_comp    (const void *, const void *);
void aem_sort    (_aresta *, unsigned int);

#endif /* SORT_H */
