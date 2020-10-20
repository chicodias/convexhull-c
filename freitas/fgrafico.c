/*
 * FGrafico.c
 */
#include "fstdio.h"
#include "fmalloc.h"
#include "erro.h"
#include "fgrafico.h"
#include "fmouse.h"

/* Funcao que inicia o modo grafico no DOS. Utiliza o arquivo EGAVGA.BGI */
int inic_graf (void)
{
  int gdriver = VGA, gmode = VGAHI, errorcode;
  initgraph(&gdriver, &gmode, "");
  errorcode = graphresult();
  if (errorcode != grOk) {
    printf("Error: %s\n", grapherrormsg(errorcode));
    return (0);
  }
  else return (1);
}

#ifndef UNIX

/* tabela de linhas usada para restaurar valor anterior, antes de se
   desenhar algo sobre (usado pela funcao 'line_xor' abaixo. */
char **m_linhas = NULL;

/* Funcao que desenha uma linha vertical na tela, e consegue depois, com
   outra chamada, restaurar o valor inicial. Recebe x a coluna e d indicando
   a operacao a ser executada. */
void line_xor (int x, int d)
{
  int i;
  unsigned s;

  if (m_linhas == NULL)
    if ((m_linhas = (char **) malloc (getmaxx () * sizeof (char *))) == NULL)
      runerror (FALTA_MEMORIA);

  if (x < 0 || x >= getmaxx ())
    return;

  if (d > 0) /* se d>0, desenha */
  {
    s = imagesize (x, 0, x, getmaxy ());
    if ((m_linhas [x] = (char *) malloc (s)) == NULL)
      runerror (FALTA_MEMORIA);
    getimage (x, 0, x, getmaxy (), m_linhas [x]);
    i = getcolor();
    setcolor (d);
    line (x, 0, x, getmaxy ());
    setcolor (i);
  }
  else /* senao restaura o valor anterior */
  {
    putimage (x, 0, m_linhas [x], 0);
    free (m_linhas [x]);
  }
}

#else /* UNIX */

XImage **m_linhas = NULL;

void line_xor (int x, int d)
{
  WINDOW *w;
  int i;

  w = active_window;

  if (m_linhas == NULL)
    if ((m_linhas = (XImage **) malloc (getmaxx () * sizeof (XImage *))) == NULL)
      runerror (FALTA_MEMORIA);

  if (x < 0 || x >= getmaxx ())
    return;

  if (d > 0) /* se d>0, desenha */
  {
    m_linhas[x] = XGetImage(w->disp, w->image, x, 0, 1, getmaxy (),
			    AllPlanes, ZPixmap);
    i = getcolor();
    setcolor (d);
    line (x, 0, x, getmaxy ());
    setcolor (i);
  }
  else /* senao restaura o valor anterior */
  {
    int a;

    /* Colca o gc da janela em modo "copy". Se nao fizermos isto, a
     * imagem sera "xor"ada com a ja existente
     */
    a = win_getdrawmode(w);
    if (a != DM_COPY)
      win_setdrawmode(w, DM_COPY);

    XPutImage(w->disp, w->image, w->gc, m_linhas[x],
	      0, 0, x, 0, 1, getmaxy());
    XPutImage(w->disp, w->window, w->gc, m_linhas[x],
	      0, 0, x, 0, 1, getmaxy());

    if (a != DM_COPY)
      win_setdrawmode(w, a);

    XDestroyImage(m_linhas[x]);
  }
}

#endif /* UNIX */

void limpa(int x1, int y1, int x2, int y2, int cor)
{
  int cor_v;

  cor_v = getcolor();
  esconde_mouse ();
#ifndef UNIX
  setfillstyle (1, cor);
#else
  setcolor(cor);
#endif
  bar(x1, y1, x2, y2);
  mostra_mouse ();
  setcolor(cor_v);
}

/* Fim de FGRAFICO.C */
