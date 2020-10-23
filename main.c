/* arquivo: main.c
  * autores: Francisco Miranda e Hiago Vinicius Americo
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "embrulho.h"

/* 
  @main: lê os pontos em cordenadas x,y da entrada padrão, armazena 
  em uma lista ligada simples e calcula o poligono convexo
*/

int main()
{
/*
   sentido: anti-horario ou horario
   pnt: numero de pontos
   posx, posy: coordenadas (x,y) de um ponto
   inic: ponto que a impressao é iniciada (L, R, D ou U)
   * l: endereço do primeiro nó da lista ligada que armazena as coordenadas dos pontos    
*/

    int sentido,pnt=0;
    double posx,posy;
    char inic;
    LISTA * l;

// inicializacao da lista ligada
    l = lista_criar();

// leitura do numero de pontos da stdin
    scanf("%d", &pnt);
    
// inserção dos pontos na lista
    for(int i = 0; i < pnt; ++i)
    {
        scanf("%lf %lf", &posx, &posy);
        lista_inserir(l,posx,posy);
    }

// ponto que a impressao é iniciada
    scanf("%c", &inic);
    scanf("%d", &sentido);
    
// chamada da função
    lista_imprimir(l);

    LISTA * m = embrulho(inic,sentido,l);

    lista_imprimir(m);
    
    //printf("%lf", angulo(l->inicio,l->inicio->prox, l->inicio->prox->prox));

// desalocação da lista para encerrar a aplicação
    lista_apagar(&l);
    lista_apagar(&m);
    return 0;
}
