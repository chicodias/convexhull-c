#include "incremental.h"
#include "primitivas.h"

LISTA incremental(char pos, int inic, LISTA * l){

/* Utilizamos as primitivas Esquerda-Direita
para saber a posição de um dado ponto em relação
a um lado do fecho convexo atual. A idéia é simplificada
se assumirmos que o polígono tem a seqüência de seus
vértices orientada em um sentido (anti-horário, 
por exemplo). Assim, basta verificar se um novo
ponto está a esquerda (se o sentido adotado for
anti-horário) de todos os lados do fecho. Se isso
for verdade, então trata-se de um ponto interno ao 
fecho, senão é um ponto externo que será colocado como
um vértice. */

// anti-horario
   LISTA anthorario(LISTA *L){
    
        NO *p, NO *q, NO *r;
        LISTA * M;
        
        
        if(lista_tamanho(l) < 3)
            M = anthorario(M->prox);
        
    
        p = L->inicio;
        q = p->prox;
        r = q -> prox;
        
        // ponto interior ao fecho
        if(esquerda(p,q,r)
        {
            p = p->prox;
            q = q->prox;
            p = r->prox;
        }
        
        // novo vertice
        else
        {
            
            p = M->inicio;
            q = p->prox;
            while(p != NULL)
            {
                if (esquerda(p,q,r))
                {
                    p = p->prox;
                    q = q->prox;
            
                }
            
                // ponto nao pertence mais ao fecho
                else
                {
                
                    p = q->prox;
                    lista_remover(M,q);
                    q = p->prox;
                }
            }    
        }
        
        }
        ultimo_emento(M)->prox = M->inicio;
        return M;
    }
}

LISTA horario(LISTA *l){

	NO *p, NO *q, NO *r;
    LISTA M;

    if(lista_tamanho(l) < 3)
    	M = anthorario(M->prox)







    return M;
   
}




