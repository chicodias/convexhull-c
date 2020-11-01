#ifndef MERGE_H
#define MERGE_H

#include "primitivas.h"
#include "lista.h"

int length(ANGS* current);

void merge(ANGS** start1, ANGS** end1,  
          ANGS** start2, ANGS** end2);

void mergeSort(ANGS** head);

void push(ANGS** head_ref,  
          int new_data);


#endif
