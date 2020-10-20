------------------------------------------------------------------------------
Programa CG. Por Cassio Polpo de Campos.
------------------------------------------------------------------------------
README.TXT: descreve como compilar e utilizar o programa CG
------------------------------------------------------------------------------
	
	Este pacote contem arquivos escritos em linguaguem C, para MS-DOS
ou Linux,  formando um pequeno  (esta' crescendo) sistema  com solucao de 
problemas de Geometria Computacional.

	Antes de executar o programa,  e' recomendavel que seja feita sua
compilacao.  So' assim ele estara  compativel com  o  sistema operacional
em uso. No DOS utilizar o 'Turbo C Borland' e os arquivos CG.DSK, CG.PRJ. 
Para compilar no Linux ver LINUX.TXT. Neste sistema o programa funcionara 
somente no ambiente de janelas XWindows.

	Devido `a falta de um help ou manual mais detalhado,  sigo agora 
com algumas dicas do modo de utilizacao do programa.

	Executando o programa, veremos o mouse, com sua coordenada exibida
no canto inferior direito.


SELECAO de PONTOS: basta clicar o botao esquerdo na posicao desejada.

REMOCAO de PONTOS: usando <Back-Space> remove-se os pontos de um em um. Com
		   a tecla <Esc> remove-se todos de uma so' vez.

INSERCAO de POLIGONOS: mantendo o botao direito pressionado, arrastamos o
		       mouse enquanto selecionamos seus vertices com o botao
		       esquerdo. (obs: qualquer poligono e' aceito, porem ao
		       executarmos alguns comandos, tipos de poligonos podem
		       ser rejeitados, causando solucoes incorretas).

REMOCAO de POLIGONOS: usando a tecla menos (-), removemos poligonos, um de
		      cada vez.

LINHA de COMANDO: teclando <Espaco> uma seta aparecera no canto inferior
		  esquerdo, esperando por algum comando do usuario. A seguir
		  esta' uma lista com algumas explicacoes:


	ANIMATION       seleciona animacao Sim/Nao.

	CLEAR           limpa a tela.

	COUNT           mostra contadores internos que sao atualizados
			a cada chamada de uma funcao primitiva para os
			diversos problemas.
	
	CLSCOUNT        limpa o contador atual.

	DELAY <num>     determina uma pausa entre as operacoes de um 
			algoritmo para melhor visualizacao.
			num=0: sem pausa.
			num<0: espera por tecla a cada passo.
			num>0: tempo de espera em milisegundos.

	EXIT            finaliza o programa.

	GRAHAM          faz o fecho convexo dos pontos utilizando o
			algoritmo de Graham.

	LOAD <arq>      le de um arquivo texto as informacoes previamente
			armazenadas.

	MINDIST         acha os dois pontos mais proximos em O(n*logn)

	MINDIST2        acha os dois pontos mais proximos em O(n*n)

        MODE            muda o modo de desenhar na tela (copy ou xor).
                        Ajuda para observarmos a complexidade de algoritmos

	QHULL           faz o fecho convexo dos pontos utilizando o
			algoritmo QuickHull.

	SAVE <arq>      salva os pontos e poligonos em um arquivo tipo
			texto, que podera ser lido posteriormente.

	TRIANG          faz a triangularizacao de um poligono em
			ordem de n*logn (eh um tiro!).

	TRIANG3         faz a triangularizacao de um poligono em
			ordem cubica.

	USECOUNT <num>  seleciona qual dos contadores ira' ser utilizado


	Consideracoes Finais
	--------------------
	Por enquanto e' isto. Qualquer problema encontrado, estou disposto
a ajudar, basta enviar um e-mail: cassio@ime.usp.br
	Sem mais, agradeco pelo interesse neste programa.
