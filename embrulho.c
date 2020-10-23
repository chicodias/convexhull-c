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
    NO * p, * q, * r, * s;
    // lista retornada
    LISTA * M;
    // vetor com os angulos entre um ponto e os n-2 outros
    double * angs;
    // posicao na lista do vertice com menor angulo
    int posic;

    // inicializacao da lista com elementos do poligono
    M = lista_criar();

    
    // primeiro elemento da lista: 
    p = findlowest(L);
    lista_inserir(M, p->x, p->y);

    // inicializacao do algoritmo

    // alocacao no primeiro no
    q = criaNo(0, p->y);
    
    // alocacao da lista com os angulos
    ANGULOS * angul = criarAng(L->n-1);
        
    int i = 0;
    r = p->prox;
    
    // calcula o angulo com todos os outros caras do vetor
    while (i < (L->n)-2)
    {
        q -> x = r -> x;
        ins_ang(angul, angulo(p, q, r), r);
        r = r->prox;
        ++i;
        
    }


    // encontrando o ponto com menor angulo
    //posic = minAng(angs, (L->n)-2);
    //s = r;
printf("\n esta na posicao: %d \n", posic );  
    // inserindo o menor angulo na lista M
    //for (int j = 0; j < posic; j++)
    //{
    //   s = s->prox;
    //   printf("\n end = %p\n",&s);
    //}
    //printf("\n saiu do for \n");
    //printf("\n x = %lf y = %lf\n", s->x, s->y);
    //lista_inserir(M, s->x, s->y);
    //printf("inseriu 22\n");
    

    // agora fazemos isso iterativamente até que o proximo vertice
    // do fecho convexo seja novamente o ponto p.
    //while (f != p)
    //{
        /* code */
    //}
    

    // como a complexidade do algoritmo no pior caso é quadrática, temos que agora em diante 
    // repetir o processo acima até que cheguemos no NÓ p novamente, para então retornar M.

    angs_imprimir(angul);
    // limpando a casa
    free(q);
    free(angs);

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
    // de produto escalar entre dois vetores
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

// imprime a lista na saída padrão
void angs_imprimir(ANGULOS *l){
    if (l == NULL)
        return;
    for (ANGS *p = l->inicio; p != NULL; p = p->prox)
        printf("alpha = %.2lf para (%.2lf,%.2lf)\n",p->ang, p->ponto->x, p->ponto->y);
    
    return;
}


// essas duas funcs tao deprecated, mas vou deixar aqui


// encontra o minimo em um vetor e retorna sua posicao
int minAng(double * vet, int n)
{
    int pos = 0;
    double min = vet[0];

    for (int i = 1; i < n; i++)
    {
        if(vet[i] < min)
        {
            min = vet[i];
            pos = i;
        }
    }
    
    return pos;
}

// encontra o maximo em um vetor e retorna sua posicao
int maxAng(double * vet, int n)
{
    int pos = 0;
    double max = vet[0];

    for (int i = 1; i < n; i++)
    {
        if(vet[i] > max)
        {
            max = vet[i];
            pos = i;
        }
    }
    
    return pos;
}
