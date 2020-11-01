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

void lista_imprimir_circ(NOc *l){
    if (l == NULL)
        return;
    NOc *p = l;
    printf("%.2lf %.2lf\n",p->x, p->y);
    for (p = p->prox; p != l; p = p->prox)
        printf("%.2lf %.2lf\n",p->x, p->y);
    
    return;
} 

void lista_imprimir_circ_inv(NOc *l){
    if (l == NULL)
        return;
    NOc *p = l;
    printf("%.2lf %.2lf\n",p->x, p->y);
    for (p = p->ant; p != l; p = p->ant)
        printf("%.2lf %.2lf\n",p->x, p->y);
    
    return;
} 

// indica se uma dada lista está vazia retornando TRUE
boolean lista_vazia(LISTA *l){
    if (l == NULL)
        return ERRO;
    else if (l->inicio == NULL)
        return TRUE;
    
    return FALSE;
}


int compara(NO * p, NO * q)
{
    //negativo se p-q<0
    //positivo se p-q>0
    
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
// remove um elemento da lista e retorna TRUE se conseguir.
// se não encontrar, retorna FALSE
// se a lista estiver vazia, retorna ERRO
void lista_remover(LISTA *l, NO * chave)
{
    double chavex = chave->x, chavey = chave->y;
    NO *p, *q;
    
    if (l == NULL || l->inicio == NULL)
        return;
    

    p = l->inicio;

    // remove da primeira posição
    if(p->x == chavex && p->y == chavey ){
        l->inicio=l->inicio->prox;
        free(p);
        return;
    }

    while (p->x != chavex && p->y != chavey){
        
        q = p;
        p = p->prox;
        l->n--;
    
        if (p == NULL)
            return;
    }

    q->prox = p->prox;
    free(p);
}

void lista_inserir_fim (LISTA* l, double x, double y)
{
    /* cria novo nó */
    NO* novo = (NO *)malloc(sizeof(NO));

    novo->x = x;
    novo->y = y;
    novo->prox = NULL;

    /* ponteiro para elemento anterior */   
    NO* ant = NULL;     
    
    /* ponteiro para percorrer a lista*/   
    NO* p = l->inicio;          

    while (p != NULL) 
    {      
        ant = p;      
        p = p->prox;
    } 
    ant->prox = novo;

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

/*
NOc * criaNoc(double x, double y)
{
    NOc * p = (NOc *) malloc(sizeof(NOc));

    p->x = x;
    p->y = y;
    p->prox = NULL;
    p->ant = NULL;

    return p;
}


NOc * listolisc(LISTA * l)
{
    NO * p = l->inicio;
    NOc * novo, * ant, *inic;

    ant = criaNoc(p->x, p->y);
    inic = ant;
    p = p->prox;
    while(p!=NULL)
    {
        novo = criaNoc(p->x, p->y);
        novo->ant = ant;
        ant->prox = novo;
        p = p->prox;
        ant = novo;
    }

    novo->prox = inic;
    inic->ant = novo;
printf("%.2lf,%.2lf e %.2lf,%.2lf", inic->x,inic->y, inic->ant->x, inic->ant->y);
    return inic;
}
/*
void liberaListaC(NOc * l)
{
    NOc * p = l , * q = l->prox, *tmp;

    while (q != p)
    {
        tmp = q;
        q = q->prox;
        free(tmp);

    }
    free(p);
}
*/