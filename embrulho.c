/* 
 * File:   embrulho.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
*/

#include "embrulho.h"
#include "primitivas.h"
#include <stdlib.h>
#include <stdio.h>
// pra usar essa lib tem que compilar com a tag -lm
#include <math.h>


/* Algoritmo do embrulho.
Inicialmente, determine o ponto P do conjunto com menor coordenada y.
Em seguida encontre o ponto Q, dentre os que outros pontos, com menor ângulo
em relação a P e a linha horizontal. Esse é um ponto que certamente é vértice 
do fecho convexo, e mais, a aresta PQ pertence ao fecho convexo. A partir daí,
basta encontrar o próximo ponto com menor ângulo em relação a aresta PQ, e assim
por diante. A execução termina quando o ponto P inicial é encontrado novamente.
*/

// retorna uma lista circular com o poligono convexo a partir dos pontos em L
LISTA * embrulho(char pos, int inic, LISTA * L)
{
    // nos que usaremos nas iteracoes
    NO * p, * q, * r;
    // lista retornada (M = Conv(L))
    LISTA * M; 
    // lista com os angulos
    ANGULOS * angul;
    // posicao na lista do vertice com menor angulo
    int posic;
    
    int i;
    
    // inicializacao da lista com elementos do poligono
    M = lista_criar();

    
    // primeiro elemento da lista: 
    lista_inserir(M, L->inicio->x, L->inicio->y);

    // nao precisa remover os caras, so percorrer a lista.
    //lista_remover(L,L->inicio);
    // inicializacao do algoritmo
    p = M->inicio;
    // alocacao no primeiro no
    q = criaNo(0, p->y);
    
    angul = criarAng(L->n - 2);
    
    r = L->inicio->prox;
    
    // calcula o angulo com todos os outros caras do conjunto e a linha horizontal
    for (i = 0; i < (L->n) - 2 && r != NULL; ++i)
    {
        q -> x = r -> x;
        ins_ang(angul, angulo(p, q, r), r);
        r = r->prox;
    }
    
    // inserindo o menor angulo na lista M
    //lista_inserir(M, angul->inicio->ponto->x, angul->inicio->ponto->y);
    
    // libera o vertice criado
    free(q);

    // agora fazemos isso iterativamente até que o proximo vertice
    // do fecho convexo seja novamente o ponto p.
    q = angul->inicio->ponto;  
    
    while(q != L->inicio)
    {
        lista_inserir_fim(M, q->x, q->y);
        //lista_imprimir(M);

        angs_apagar(&angul);
        r = q->prox;

        if (r == NULL)
            break;
        
        angul = criarAng(L->n - 2);
    
    // calcula o angulo com todos os outros caras do conjunto
        for (i = 0; i < (L->n) - 2; ++i)
        {
            ins_ang_dec(angul, angulo(q, p, r), r);
            r = r->prox;

            if (r == NULL)
                r = L->inicio;    
        }

        //printf("\n p=  %lf, %lf, q =  %lf, %lf \n", p->x, p->y,q->x, q->y);

        p = q;
        q = angul->inicio->ponto;
        
        
        
        //angs_imprimir(angul);
                   
    }
        
    q->prox = M->inicio; 
    printf("ultimo: %lf %lf primeiro: %lf %lf \n", q->x, q->y, q->prox->x, q->prox->y);   

    return M; 
}


/* Como temos uma lista ordenada pelos valores de y,
   basta pegar o primeiro elemento da lista.
*/
NO * findlowest(LISTA * L)
{
    return L->inicio;
}

// calcula o angulo entre o vetor PQ e o vetor PR
double angulo(NO * p, NO * q, NO * r)
{
    // angulo que retornaremos
    double ang;
    
    // vetores PQ e PR
    NO * PQ = criaNo(q->x - p->x, q->y - p->y);
    NO * PR = criaNo(r->x - p->x, r->y - p->y);

    // cos do angulo entre PQ e PR a partir da def
    // de produto escalar
    ang = prodEscalar(PQ, PR) / (sqrt(norma2(PQ)) * sqrt(norma2(PR)));

    free(PQ);
    free(PR);

    return acos(ang); // retorna o angulo no intervalo [0,2pi]
}

ANGULOS * criarAng(int n)
{
    return (ANGULOS *) malloc (n * sizeof(ANGULOS));
}

// insere um elemento na lista ordenadamente de acordo com o angulo
void ins_ang (ANGULOS * l, double y, NO * x)
{
    /* cria novo nó */
    ANGS* novo = (ANGS *)malloc(sizeof(ANGS));

    novo->ang = y;
    novo->ponto = x;
    novo->prox = NULL;

    /* ponteiro para elemento anterior */   
    ANGS* ant = NULL;     
    
    /* ponteiro para percorrer a lista*/   
    ANGS* p = l->inicio;          
    
    
    /* procura posição de inserção */   
    while (p != NULL && p->ang < y) 
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

    }


// insere um elemento na lista ordenadamente de acordo com o angulo
void ins_ang_dec (ANGULOS * l, double y, NO * x)
{
    /* cria novo nó */
    ANGS* novo = (ANGS *)malloc(sizeof(ANGS));

    novo->ang = y;
    novo->ponto = x;
    novo->prox = NULL;

    /* ponteiro para elemento anterior */   
    ANGS* ant = NULL;     
    
    /* ponteiro para percorrer a lista*/   
    ANGS* p = l->inicio;          
    
    
    /* procura posição de inserção */   
    while (p != NULL && p->ang > y) 
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

    }


// imprime a lista na saída padrão
void angs_imprimir(ANGULOS *l){
    if (l == NULL)
        return;
    for (ANGS *p = l->inicio; p != NULL; p = p->prox)
        printf("alpha = %.2lf para (%.2lf,%.2lf)\n",p->ang, p->ponto->x, p->ponto->y);
    
    return;
}



// desaloca a lista de angulos
void angs_apagar(ANGULOS **l){
    ANGS *p, *q;

    // se a lista estiver vazia, retorne 
    if (*l == NULL)
        return;

    // inicialização do ponteiro que percorre a lista
    p = (*l)->inicio;
    while(p != NULL)
    {
        q = p->prox;
        free(p);
        p = q;
    }
    
    free(*l);
    
}
