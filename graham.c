#include "primitivas.h"
#include "graham.h"
#include "pilha.h"
#include "mergesort.h"
#include <stdio.h>


/* Recebe uma lista de pontos L e retorna uma lista com ConvHull(L) */
LISTA * graham (LISTA * L)
{
    /* Encontra o ponto P com menor coordenada y;
    ordena os pontos restantes pelo angulo em relacao a linha 
    horizontal que passa pelo ponto P. Empilhamos os dois primeiros 
    pontos e entao basta percorrermos os outros. */

    PILHA * pi = criapilha(), * top;
    NO * p, * q, *r;
    ANGS * t;
    ANGULOS * angul = criarAng();
    int i;

    
    // inicializacao do algoritmo
    p = findLowestY(L);
    // alocacao no primeiro no (linha horizontal)
    q = criaNo(p->x + 100, p->y);
    
    // calcula o angulo entre os outros pontos do conjunto e a linha horizontal
    for (r = L->inicio; r != NULL; r = r->prox)
        if (!pontosIguais(p,r))
            insere_inicio_ang(angul, angulo(p, q, r), r);

    // libera o ponto auxiliar criado
    free(q);

    // ordena a lista de angulos
    mergeSort(&(angul->inicio));
    
     q = angul->inicio->ponto;

    // empilha os dois primeiros pontos
    empilha(pi, p);
    empilha(pi, q);
    
    t = angul->inicio->prox;
    
    while (t != NULL)
    {
        top = topo(pi);
        if(top->prox != NULL)
        {
            q = top->ponto;
            p = top->prox->ponto;
        }

  //   se r esta a esquerda do vetor PQ (topo da pilha)
  //        empilhamos e olhamos o proximo ponto da lista angul; 
        if (esquerda(p,q,t->ponto))
        {
            empilha(pi,t->ponto);
            t = t->prox;
        }

//    agora, se o ponto em questao esta a direita deste mesmo vetor,
//                 desempilhamos q e voltamos a analisar a pilha. 
        else
            desempilha(pi);
    }
    
    // libera a lista alocada
    angs_apagar(&angul);

    return pilhatolist(pi);
}
