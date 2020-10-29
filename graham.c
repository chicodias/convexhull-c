#include "primitivas.h"
#include "graham.h"
#include "pilha.h"
#include <stdio.h>


/* Recebe uma lista de pontos L e retorna uma pilha com ConvHull(L) */
PILHA * graham (LISTA * L)
{
    /* Ele encontra o ponto P com menor coordenada y e ent�o 
    ordena os pontos restantes pelo �ngulo em rela��o � linha 
    horizontal que passa pelo ponto P. Agora, com ajuda de uma pilha,
     o algoritmo trabalha em tempo linear. Empilhamos os dois primeiros 
     pontos e ent�o basta percorrermos os pontos ordenados fazendo o seguinte:
 */
    PILHA * pi = criapilha(), * top;
    NO * p, * q, *r;
    ANGS * t;
    ANGULOS * angul = criarAng(L->n - 1);
    int i;

    // menor coord y
    p = L->inicio;
  
    // alocacao no primeiro no
    q = criaNo(0, p->y);
    
        
    
    
    // calcula o angulo com todos os outros caras do conjunto e a linha horizontal
    for (r = L->inicio->prox; r != NULL; r = r->prox)
    {
        q -> x = r -> x;
        ins_ang(angul, angulo(p, q, r), r);
        printf("%p\n",r);
    }
    
    // libera o vetor auxiliar criado
    free(q);

    empilha(pi, p);
    empilha(pi, angul->inicio->ponto);
    
    t = angul->inicio->prox;
    
    while (t != NULL)
    {
        top = topo(pi);
        q = top->ponto;
        p = top->prox->ponto;
    /* se r esta a esquerda do vetor PQ (topo da pilha)
      empilhamos e olhamos o proximo ponto da lista angul; */
        if (esquerda(p,q,t->ponto))
        {
            empilha(pi,t->ponto);
            t = t->prox;
        }
/*    agora, se o ponto em questao esta a direita deste mesmo vetor,
      desempilhamos q e voltamos a analisar a pilha. */
        else
            desempilha(pi);
    }
    
    // libera a lista alocada
    angs_apagar(&angul);

    return pi;
}