# Projeto 1 - Fecho Convexo
## SCC0223 - ESTRUTURAS DE DADOS I

Abaixo, uma relação das bibliotecas desenvolvidas na implementação do projeto, com uma breve descrição:

* main.c - programa principal, responsável por ler a entrada, chamar às funções e imprimir a saída. Os parâmetros de entrada e saída foram gerados conforme especificação do projeto, para serem enviados na plataforma *run.codes*.

* embrulho.h - contém as funções utilizadas para cálculo do fecho convexo através do Algoritmo de Embrulho;
* graham.h - contém as funções utilizadas para cálculo do fecho convexo através do Algoritmo de Graham;
* lista.h - contém as funções utilizadas para operações com o TAD *Lista Encadeada Dinâmica*;
* pilha.h - contém as funções utilizadas para operações com o TAD *Pilha*;
* primitivas.h - contém as operações primitivas com vetores para o cálculo do fecho convexo;
* mergesort.h - biblioteca do algoritmo de ordenação MergeSort para listas encadeadas;
* tempo.h - biblioteca para calcular o tempo tomado pelas operações.

O programa pode ser compilado através da diretiva `make all` e rodado através do comando `make run`. Para limpar os arquivos gerados pela compilação, utilize o comando `make clean`, conforme padrões vigentes da GNU Coding Standards.