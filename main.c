#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main()
{
    int sentido,pnt=0;
    double posx,posy;
    char inic;
    LISTA * l;

    l = lista_criar();

    scanf("%d", &pnt);

    for(int i = 0, i < pnt, ++i)
    {
        scanf("%f %f", &posx, &posy)
        lista_inserir(l,posx,posy);
    }
    
    scanf("%c", &inic);
    switch (inic)
    {
    case "L":
        /* code */
        break;
    
    case "R":
        /* code */
        break;
    
    case "D":
        /* code */
        break;
    
    case "U":
        /* code */
        break;
    
    default:
        break;
    }

    lista_apagar(&l);
    return 0;
}