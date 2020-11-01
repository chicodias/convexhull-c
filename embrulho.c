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
por diante. A execução termina quando o ponto P inicial é encontrado novamente. */

// retorna uma lista com o poligono convexo a partir dos pontos em L
LISTA * embrulho(LISTA * L)
{
    // nos que usaremos nas iteracoes
    NO * p, * q, * r, *inicio;
    // lista retornada (M = Conv(L))
    LISTA * M; 
    // lista com os angulos e respectivos apontadores
    ANGULOS * angul;
    ANGS * a, * b;
    
    
    // inicializacao da lista com elementos do poligono
    M = lista_criar();

    
    // primeiro elemento da lista: 
    p = findLowestY(L);
    lista_inserir(M, p->x, p->y);
    inicio = p;

    // inicializacao do algoritmo

    // alocacao no primeiro no (linha horizontal)
    q = criaNo(p->x + 100, p->y);
    
    angul = criarAng();
    
    // calcula o angulo entre os outros pontos do conjunto e a linha horizontal
    for (r = L->inicio; r != NULL; r = r->prox)
        if (!pontosIguais(p,r))
            insere_inicio_ang(angul, angulo(p, q, r), r);  
        
    // libera o vertice criado
    free(q);  

    // pega o ponto com menor angulo e remove-o da lista
    a = minAng(angul);

    if (a != NULL)
    {
        b = a->prox;
        a->prox = b->prox;
    }
    else
    {
        b = angul->inicio;
        angul->inicio = angul->inicio->prox;
    }

    q = b->ponto;
    free(b);

    // insere p na lista com angulo -inf para o criterio de parada
    insere_inicio_ang(angul, -7, p);
  
    
    // agora fazemos isso iterativamente até que o proximo vertice
    // do fecho convexo seja novamente o ponto p.
    
    while (!pontosIguais(inicio,q))
    {
       lista_inserir(M, q->x, q->y);

    // calcula o angulo com todos os pontos na lista angul
       for(a = angul->inicio; a!= NULL; a = a->prox)
        {          
            r = a->ponto;
            if (!pontosIguais(p,r))
                a->ang = angulo(q, p, r);
        
        // algulo de -inf para pontos iguais
            else
                a->ang = -7;
        }
               
    //  escolhe o ponto com o maior angulo e remove-o da lista
        a = maxAng(angul);
        
    // se o ponto nao é o primeiro da lista angul
        if (a != NULL)
        {
            b = a->prox;
            a->prox = b->prox;
        }
    // ponto maximo no inicio da lista angul
        else
        {
            b = angul->inicio;
            angul->inicio = angul->inicio->prox;
        }
        
        p = q;
        q = b->ponto;
        free(b);
        
    }

    // libera a lista restante de angulos
    angs_apagar(&angul);

    return M; 
}
