/* 
 * File:   primitivas.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
  (baseado no arquivo lista.h de Eliane Gniech Karasawa)
 */

#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H

#include "lista.h"

double prodEscalar (NO * p, NO * q);

double norma2 (NO * p);

double area2 (NO * p, NO * q, NO * r);

boolean esquerda (NO * p, NO * q, NO * r);

boolean direita (NO * p, NO * q, NO * r);


#endif