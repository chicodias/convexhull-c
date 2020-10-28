#include "primitivas.h"
#include "graham.h"
#include "pilha.h"
#include <stdio.h>



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
    
        
    r = L->inicio->prox;
    
    // calcula o angulo com todos os outros caras do conjunto e a linha horizontal
    for (i = 0; i < (L->n) - 1 && r != NULL; ++i)
    {
        q -> x = r -> x;
        ins_ang(angul, angulo(p, q, r), r);
        r = r->prox;
    }
    
    angs_imprimir(angul);
    // libera o vetor auxiliar criado
    free(q);

    empilha(pi, p);
    empilha(pi, angul->inicio->ponto);


    
    t = angul->inicio->prox;
    
    while (!pilha_vazia(pi))
    {
        top = topo(pi);
        p = top->ponto;
        q = top->prox->ponto;
        
        printf("%lf %lf", t->ponto->x, t->ponto->y);

        /* se o ponto X em quest�o est� a esquerda do vetor pen�ltimo-�ltimo
     elemento da pilha (�ltimo elemento da pilha � o topo) ent�o empilhamos
      e olhamos o pr�ximo;
    se o ponto X em quest�o est� a direita deste mesmo vetor, ent�o desempilhamos 
    e voltamos a analisar X. */


        if (esquerda(p,q,t->ponto))
        {
            empilha(pi,t->ponto);
            if (t->prox != NULL)
                t = t->prox;
            else
                return pi;
        }
        else
        {
            desempilha(pi);
        }
        
       
    }
    
    angs_apagar(&angul);


    return pi;
}