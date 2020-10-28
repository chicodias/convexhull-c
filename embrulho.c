/* 
 * File:   embrulho.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
*/

#include "embrulho.h"
#include "primitivas.h"
#include "lista.h"


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
    //printf("ultimo: %lf %lf primeiro: %lf %lf \n", q->x, q->y, q->prox->x, q->prox->y);   

    return M; 
}
