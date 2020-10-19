/* 
 * File:   lista.h
 * Author: Francisco Rosa Dias de Miranda
  (baseado no arquivo lista.h de Eliane Gniech Karasawa)
 */

#ifndef LISTA_H
#define LISTA_H

#define boolean int /*Define tipo booleano*/
#define TRUE 1
#define FALSE 0
#define ERRO -32000

typedef struct pon NO; /*Tipo ITEM da lista*/

// lista ligada que armazena os pontos da entrada
struct pon {
    double x;
    double y;
    NO * prox;
};

typedef struct lista LISTA;

LISTA *lista_criar(void); /*Criacao da lista e retorno do seu ponteiro*/
boolean lista_apagar(LISTA **l); /*Apaga todo o conteudo da lista e libera o bloco de memoria*/
void lista_imprimir(LISTA *l); /*Imprime os elementos armazenados na lista*/
boolean lista_cheia(LISTA *l); /*Retorna TRUE se nao for possivel alocar mais nos*/
boolean lista_vazia(LISTA *l); /*Retorna TRUE se a lista estiver vazia*/
boolean lista_inserir(LISTA *l, double x, double y); /*Insere o PONTO no inicio da lista*/

#endif /* LISTA_H */
