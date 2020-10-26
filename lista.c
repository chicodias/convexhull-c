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

// indica se uma dada lista está vazia retornando TRUE
boolean lista_vazia(LISTA *l){
    if (l == NULL)
        return ERRO;
    else if (l->inicio == NULL)
        return TRUE;
    
    return FALSE;
}


// insere um elemento na lista ordenadamente de acordo com a coordenada y.
void lista_inserir (LISTA* l, double x, double y)
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
    
    
    /* procura posição de inserção */   
    while (p != NULL && p->y < y) 
    {      
        ant = p;      
        p = p->prox;
    }   
    
    /* insere elemento */   
    if (ant == NULL) 
    {   /* insere elemento no início */      
        novo->prox = l->inicio;      
        l->inicio = novo;   
    }
    
    else 
    {   /* insere elemento no meio da lista */      
        novo->prox = ant->prox;      
        ant->prox = novo;   
    }

    (l->n)++;
    }

// insere um elemento na lista ordenadamente de acordo com a coordenada x.
void lista_inserir_x (LISTA* l, double x, double y)
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
    
    
    /* procura posição de inserção */   
    while (p != NULL && p->x < x) 
    {      
        ant = p;      
        p = p->prox;
    }   
    
    /* insere elemento */   
    if (ant == NULL) 
    {   /* insere elemento no início */      
        novo->prox = l->inicio;      
        l->inicio = novo;   
    }
    
    else 
    {   /* insere elemento no meio da lista */      
        novo->prox = ant->prox;      
        ant->prox = novo;   
    }

    (l->n)++;
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