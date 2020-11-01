/* arquivo: main.c
  * autores: Francisco Miranda e Hiago Vinicius Americo
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "embrulho.h"
#include "primitivas.h"
#include "graham.h"
#include "tempo.h"

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
    LISTA * l, * m;

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
    scanf(" %c", &inic);
    scanf("%d", &sentido);
    
//lista_imprimir(l);
 
//   calculo do tempo
 //double start, end;
 //  start = seconds(); 

// calculo de conv(L)
// algoritmo do embrulho
   // m = embrulho(l);

// algoritmo de graham
   m = embrulho(l);

 //  lista_imprimir(l);
  // lista_inverter(&l);
  // printf("\n lista invertida:\n");
   //lista_imprimir(l);

  //  end = seconds() - start;


 //lista_imprimir(m);
    
    imprime_fecho(m, inic, sentido);
    printf("%.2lf\n",100*(double)(m->n /(double)l->n));

// desalocação da lista para encerrar a aplicação
    lista_apagar(&l);
    lista_apagar(&m);
    
    return 0;
}
