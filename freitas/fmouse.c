/*
 * Fmouse.c
 */
#include "fgrafico.h"
#include "fmouse.h"

#ifndef UNIX

/* Funcao para esconder o mouse (no DOS) */
void esconde_mouse(void)
{
  asm {
    push si
    push di
    mov ax,2      /* esconde o ponteiro */
    int 33h
    pop di
    pop si
  }
}

/* Funcao para mostrar o mouse (no DOS) */
void mostra_mouse(void)
{
  asm {
    push si
    push di
    mov ax,1      /* mostra o ponteiro */
    int 33h
    pop di
    pop si
  }
}

/* Funcao para verificar a presenca de um dispositivo de mouse (no DOS) */
int verifica_mouse(void)
{
  unsigned int mouse_ok;
  asm {
    mov ax,0           /* verifica o mouse */
    int 33h
    mov mouse_ok,ax
  }
  return ((mouse_ok == 0xFFFF));
}

/* Funcao para posicionar o mouse na tela (em x,y) (no DOS) */
void posiciona_mouse (int x, int y)
{
  y = getmaxy()-y;
  asm {
	push si
	push di
	mov ax,4        /* posiciona o ponteiro */
	mov cx,x
	mov dx,y
	int 33h
	pop di
	pop si
  }
}

/* Funcao que limita o mouse ao quadrado delimitado por x_ini e x_fim,
   y_ini e y_fim (no DOS) */
void limita_mouse (int x_ini, int x_fim, int y_ini, int y_fim)
{
  y_ini = getmaxy()-y_ini;
  y_fim = getmaxy()-y_fim;
  asm {
	push si
	push di
	mov ax,7        /* limita intervalo horizontal */
	mov cx,x_ini
	mov dx,x_fim
	int 33h
	mov ax,8        /* limita intervalo vertical */
	xor cx,y_ini
	mov dx,y_fim
	int 33h
	pop di
	pop si
  }
}

/* Funcao que retorna em a o status dos botoes do mouse e em b,c
   as coordenadas dele na tela (no DOS) */
void get_mouse_status (int *a, int *b, int *c)
{
  unsigned int botao, pos_x, pos_y;
  asm {
	 push si
	 push di
	 mov ax,3           /* get position and button status */
	 int 33h
	 pop di
	 pop si
	 mov botao,bx       /* botao 1,2 ou 3 selecionado */
	 mov pos_x,cx           /* coordenada x do mouse */
	 mov pos_y,dx           /* coordenada y do mouse */
  }
  *a = botao;
  *b = pos_x;
  *c = getmaxy()-(int)pos_y;
}

#else  /* UNIX */

/* No X, nao e necessario esconder o mouse ao desenhar (como ja era
 * de se esperar. Assim, fazemos funcoes `esconde_mouse()' e
 * `mostra_mouse()' sao vazias para manter a compatibilidade com o
 * codigo do DOS.
 */

void esconde_mouse(void) { }
void mostra_mouse(void) { }

void posiciona_mouse(int x, int y)
{
  /* AINDA NAO IMPLEMENTADO.
   *
   * Para fazer isto, veja a funcao XWarpPointer da Xlib
   * (man XWarpPointer). Note que alguns usuarios nao gostariam
   * de ter programas que mudam a posicao do mouse contra a sua
   * vontade...
   */
}

void limita_mouse(int x1, int y1, int x2, int y2)
{
  /* NAO IMPLEMENTADO.
   *
   * E possivel fazer isto no X, mas nao acho que valha a pena.
   * Uma das vantagens de se rodar o programa num ambiente de
   * janelas e poder trabalhar com outros programas ao mesmo tempo.
   * Se o programa limitar o mouse a uma regiao, esta caracteristica
   * sera perdida.
   */
}

void get_mouse_status(int *bot, int *x, int *y)
{
  *bot = read_mouse(x, y);
  *y = getmaxy() - *y;
}

int mouse_esquerdo(int botao)
{
  return ((botao & MOUSE_1) != 0);
}

int mouse_direito(int botao)
{
  return ((botao & MOUSE_2) != 0);
}

int verifica_mouse(void)
{
  return 1;     /* Assume que no X o mouse esta presente. */
}

#endif /* UNIX */

/* Fim de FMOUSE.C */
