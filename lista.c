/* 
 * File:   lista.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
 */

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// aloca um no dinamicamente com as cordenadas e NO->prox = NULL
NO * criaNo(double x, double y)
{
    NO * p = (NO *) malloc(sizeof(NO));

    p->x = x;
    p->y = y;
    p->prox = NULL;

    return p;
}

/*
Criação da lista com alocação dinâmica e inicialização do ponteiro inicio
Retorna o ponteiro da lista
*/

LISTA *lista_criar(void){
    LISTA * lis;

    lis = (LISTA *)malloc(sizeof(LISTA));

    // se der problema no malloc
    if (lis == NULL)
        return NULL;

    lis->n = 0;
    lis->inicio = NULL;
    return lis;
}

// insere um elemento no começo da lista.
void lista_inserir (LISTA* l, double x, double y)
{
    /* cria novo nó */
    NO * novo = (NO *)malloc(sizeof(NO));

    novo->x = x;
    novo->y = y;
    novo->prox = l->inicio;

    l->inicio = novo;
    (l->n)++;
}


/*
Limpeza do elemento lista e desalocação seu ponteiro
*/

boolean lista_apagar(LISTA **l){
    NO *p, *q;

    // se a lista estiver vazia, retorne FALSE
    if (*l == NULL)
        return FALSE;

    // inicialização do ponteiro que percorre a lista
    p = (*l)->inicio;
    while(p != NULL)
    {
        q = p->prox;
        free(p);
        p = q;
    }
    
    free(*l);
    
    return TRUE;
}

// imprime a lista na saída padrão
void lista_imprimir(LISTA *l){
    if (l == NULL)
        return;
    for (NO *p = l->inicio; p != NULL; p = p->prox)
        printf("%.2lf %.2lf\n",p->x, p->y);
    
    return;
} 

// Função que encontra o menor y 
NO * findLowestY(LISTA * L)
{
    NO * p = L->inicio, * q = p;
    
    while (p != NULL)
    {
        if (q->y > p->y)
            q = p;
        p = p->prox;
    }
    
    return q;
}

// Função que encontra o menor x 
NO * findLowestX(LISTA * L)
{
    NO * p = L->inicio, * q = p;
    
    while (p != NULL)
    {
        if (q->x > p->x)
            q = p;
        p = p->prox;
    }
    
    return q;
}

// Função que encontra o maior y
NO * findHigherY(LISTA * L)
{
    NO * p = L ->inicio, * q = p;

    while(p != NULL)
    {
        if (q->y < p->y)
            q = p;
        p = p->prox;
    }
}

// Função que encontra o maior x
NO * findHigherX(LISTA * L)
{
    NO * p = L ->inicio, * q = p;

    while(p != NULL)
    {
        if(q->x < p->x)
            q = p;
        p = p->prox;
    }
}

// recebe uma lista com o fecho e imprime na saida padrao
// os pontos pertencentes de acordo com as variáveis:
// inic: ponto que a impressao é iniciada (L, R, D ou U)
// sentido: 0 - anti horario; 1 - horario

void imprime_fecho(LISTA * l, char inic, int sentido)
{
    NO * first;

    
    switch (inic)
    {
    case 'L':
        first = find(l, 'x', 0);
        break;

    case 'R':
        first = find(l, 'x', 1);
        break;

    case 'D':
        first = find(l, 'y', 0);
        break;

    case 'U':
        first = find(l, 'y', 1);
        
        break;

    default:
        break;
    }
    /*
    switch (sentido)
    {
        
    case 1:
        lista_imprimir(first);
        break;
    
    case 0:
        lista_imprimir(l);
        break;
    
    default:
        break;
    } 
    */  

}

// encontra a maior ou a menor coordenada de x ou y e retorna uma lista circular ordenada 
NO * find(LISTA * l, char coord, int high)
{
    printf("procurando %c ", coord);
    NO * found, * p, * q;

    found = p;
    q = p->prox;

    switch (coord)
    {
    case 'x':
        switch (high)
        {
        case 1:
            /* encontra o maior x */
            found = findHigherX(l);
            break;
        case 0:
            /* encontra o menor x */
            found = findLowestX(l);
            break;
        default:
            break;
        }
        break;
    
    case 'y':
        switch (high)
        {
        case 1:
            /* encontra o maior y */
            found = findHigherY(l);
            break;
        case 0:
            /* encontra o menor y */
            found = findLowestY(l);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    return found;
}
