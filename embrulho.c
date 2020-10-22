/* 
 * File:   embrulho.c
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
*/

#include "embrulho.h"
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
    NO * p, * q, *r;
    // lista retornada
    LISTA * M;
    // vetor com os angulos entre um ponto e os n-1 outros
    double * angs;

    // inicializacao da lista com elementos do poligono
    M = lista_criar();

    // primeiro elemento da lista: 
    p = findlowest(L);
    lista_inserir(M, p->x, p->y);

    // inicializacao do algoritmo
    // alocacao no primeiro no
    q = (NO *) malloc(sizeof(NO));
    q->x = 0;
    q->y = 0;

    // alocacao do vetor com os angulos
    angs = (double *) malloc(((L->n)-1) * sizeof(double));

    int i = 0;
    r = p->prox;
    
    // calcula o angulo com todos os outros caras do vetor
    while (i < (L->n)-1)
    {
        angs[i] = angulo(q, p, r);
        r = r->prox;
        ++i;
    }
            
    
    // tbd: funcao calcular o minimo do vetor que retorna a posiçao i do elemento no vetor
    // percorrer a lista, e colocar L[i] como proximo nó da lista M

    // como a complexidade do algoritmo no pior caso é quadrática, temos que agora em diante 
    // repetir o processo acima até que cheguemos no NÓ p novamente, para então retornar M.

     
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
// confirmar se eh isso msm
double angulo(NO * p, NO * q, NO * r)
{
    double ang;

    // cada componente do vetor (x,y) em uma posicao
    // x na pos 0 e y na pos 1.
    double PQ[2], PR[2];

    PQ[0] = q->x - p->x;
    PQ[1] = q->y - p->y;

    PR[0] = r->x - p->x;
    PR[1] = r->y - p->y;

    // cos do angulo entre PQ e PR a partir da def
    // de produto escalar entre dois vetores

    ang = (PQ[0]*PR[0] + PQ[1]*PR[1]) /
          sqrt(PQ[0] * PQ[0] + PQ[1] * PQ[1]) * sqrt(PR[0] * PR[0] + PR[1] * PR[1]);
    

    return acos(ang); //retorna o angulo em radianos
}

// encontra o minimo em um vetor e retorna sua posicao
int min(double * vet, int n)
{
    int pos = 0;

    return pos + 1;
}
