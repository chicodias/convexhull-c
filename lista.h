/* 
 * File:   lista.h
 * Author: Francisco Rosa Dias de Miranda e Hiago Vinicius Americo
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

// nó de uma lista circular
typedef struct pon_circ NOc;

struct pon_circ {
    double x;
    double y;
    NOc * prox;
    NOc * ant;
};

// cabeça da lista
struct lista
    {
    NO *inicio; 
    int n;
    };

typedef struct lista LISTA;

NO * criaNo(double x, double y);
LISTA *lista_criar(void); /*Criacao da lista e retorno do seu ponteiro*/
boolean lista_apagar(LISTA **l); /*Apaga todo o conteudo da lista e libera o bloco de memoria*/
void lista_imprimir(LISTA *l); /*Imprime os elementos armazenados na lista*/
void lista_inverter(NO ** L);
boolean lista_cheia(LISTA *l); /*Retorna TRUE se nao for possivel alocar mais nos*/
boolean lista_vazia(LISTA *l); /*Retorna TRUE se a lista estiver vazia*/
void lista_inserir(LISTA *l, double x, double y); /*Insere o PONTO ordenadamente por y*/
void lista_inserir_fim(LISTA *l, double x, double y); /*Insere o PONTO no fim da lista*/
void lista_remover(LISTA *l, NO * chave); //remove um ponto da lista
void imprime_fecho(LISTA * l, char inic, int sentido); //imprime uma lista circular na saida padrao

NO * findLowestY(LISTA * L);
NO * findLowestX(LISTA * L);
NO * findHighestX(LISTA * L);
NO * findHighestY(LISTA * L);

NO * find(LISTA * l, char coord, int high);


#endif /* LISTA_H */
