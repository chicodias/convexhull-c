/* 
 * File:   lista.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
 */

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// recebe um ponto (x,y), aloca no heap, inicializa e retorna o endereço de um NO
NO * criaNo(double x, double y)
{
    NO * p = (NO *) malloc(sizeof(NO));

    p->x = x;
    p->y = y;
    p->prox = NULL;

    return p;
}


// Criação da lista com alocação dinâmica e inicialização do ponteiro inicio
// Retorna o ponteiro da lista
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

//Limpeza do elemento lista e desalocação seu ponteiro
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

// inverte a ordem dos elementos de uma lista L
void lista_inverter(NO ** L)
{
    NO * anterior = NULL,
    * atual = *L,
    * proximo = NULL;
    
    while (atual != NULL) {
        proximo = atual->prox;
        atual->prox = anterior;
        anterior = atual;
        atual = proximo;
    }
    *L = anterior;
}

// recebe uma lista de pontos e imprime-a na saída padrão
void lista_imprimir(LISTA *l){
    if (l == NULL)
        return;
    for (NO *p = l->inicio; p != NULL; p = p->prox)
        printf("%.2lf %.2lf %p \n",p->x, p->y, &p->prox);
    
    return;
} 

// Função que encontra o menor y e retorna o endereço de seu NO
NO * findLowestY(LISTA * L)
{
    NO * p = L->inicio, * q = p;
    
    while (p != NULL)
    {
        if (q->y > p->y)
            q = p;
        else if (q->y == p->y)
            if(q->x < p->x)
                q = p;
        p = p->prox;
    }
    
    return q;
}

// Função que encontra o menor x e retorna o endereço de seu NO
NO * findLowestX(LISTA * L)
{
    NO * p = L->inicio, * q = p;
    
    while (p != NULL)
    {
        if (q->x > p->x)
            q = p;
        else if (q->x == p->x)
            if(q->y < p->y)
                q = p;
        p = p->prox;
    }

    return q;
}

// Função que encontra o maior y e retorna o endereço de seu NO
NO * findHighestY(LISTA * L)
{
    NO * p = L ->inicio, * q = p;

    while (p != NULL)
    {
        if (q->y < p->y)
            q = p;
        else if (q->y == p->y)
            if (q->x < p->x)
                q = p;
        p = p->prox;
    }
        return q;
}

// Função que encontra o maior x
NO * findHighestX(LISTA * L)
{
    NO * p = L ->inicio, * q = p;

    while (p != NULL)
    {
        if (q->x < p->x)
            q = p;
        else if (q->x == p->x)
            if (q->y < p->y)
                q = p;
        p = p->prox;
    }
        return q;
}

// recebe uma lista com o fecho e imprime na saida padrao
// os pontos pertencentes de acordo com as variáveis:
// inic: ponto que a impressao é iniciada (L, R, D ou U)
// sentido: 0 - anti horario; 1 - horario
void imprime_fecho(LISTA * l, char inic, int sentido)
{
    NO * first;

   if (sentido)
        lista_inverter(&(l->inicio));

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
    
    for (int i = 0; i < l->n; i++)
    {
        printf("%.2lf %.2lf\n", first->x, first->y);
        first = first->prox;
        if(first == NULL)
            first = l->inicio;
    } 

}

// retorna o ponto que a impressao será iniciada (L, R, D ou U)
NO * find(LISTA * l, char coord, int high)
{
    NO * p = l->inicio, * q = p->prox, * found = p;

    switch (coord)
    {
    case 'x':
        switch (high)
        {
        case 1:
            /* encontra o maior x */
            found = findHighestX(l);
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
            found = findHighestY(l);
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
