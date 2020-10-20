/*
 * Interfac.c
 */
#include "fconio.h"
#include "fctype.h"
#include "fstdio.h"
#include "fstdlib.h"
#include "cg.h"
#include "fmouse.h"
#include "fgrafico.h"
#include "interfac.h"

/* vetor de pontos */
extern _point pontos[];
/* vetor de poligonos */
extern _poly  poly[];

/* Funcao que mostra o poligono poli na tela */
void mostra_poly (_poly poli)
{
  unsigned int i;
  setcolor (LIGHTRED);
  for (i=1; i<poli.np; i++)
    line (poli.p[i-1][X], poli.p[i-1][Y], poli.p[i][X], poli.p[i][Y]);
  line (poli.p[poli.np-1][X],poli.p[poli.np-1][Y],poli.p[0][X],poli.p[0][Y]);

  setcolor (WHITE);
}

/* Funcao de interface que trata de receber as entradas do usuario. Devolve
   em np e npoly o numero de pontos e de poligonos lidos */
void selecaopontos(int *np, int *npoly)
{
  static int fir = 0;
  char k;
  int botao;
  unsigned int x, y, x2, y2;
  int pos_x, pos_y;
  int numpontos, numvelho, numpoly;
  int fim;
  char txt[50];

  if (!verifica_mouse()) /* Sem mouse nao da' ! */
  {
    closegraph();
    printf("No mouse!!!\n");
    exit(1);
  }
  k = 0;
  fim = 0;
  numvelho = numpontos = *np;
  numpoly  = *npoly;
  /* posiciona o ponteiro no centro e limita o mouse a um retangulo na tela */
  x2 = getmaxx() - 1;
  y2 = getmaxy() - 30;
  x = x2/2;
  y = y2/2;
  posiciona_mouse (x, y);
  limita_mouse (0, x2, 30, getmaxy()-1);

  setcolor (WHITE);
  setwritemode (1);
  mostra_mouse();

  while (!fim)
  {
    if (numvelho != numpontos) /* se mudou o numero de pontos */
    {
      sprintf(txt, "%3d point(s) selected", numpontos);
      setwritemode (0);
      limpa(0, 10, x2, 0, BLACK);
      setwritemode (1);
      outtextxy(10, 10, txt);
      numvelho = numpontos;
    }
    if (kbhit()) /* se tecla foi pressionada */
    {
      k=toupper(getch());
      switch (k)
      {
	case ' ': fim = 1;
		  continue; /* espaco retorna */

	case ESC: /* apaga todos os pontos */
		  numvelho = -1;
		  numpontos = 0;
		  numpoly = 0;
		  esconde_mouse();
		  clearviewport();
		  mostra_mouse();
		  continue;

	case BACKSPACE: /* apaga o ultimo ponto */
			if (numpontos > 0)
			{
			  numpontos--;
			  esconde_mouse ();
			  setcolor (YELLOW);
                          line (pontos[numpontos][X]-1, pontos[numpontos][Y],
                                pontos[numpontos][X]+1, pontos[numpontos][Y]);	
                          line (pontos[numpontos][X], pontos[numpontos][Y]-1,
				pontos[numpontos][X], pontos[numpontos][Y]+1);
			  setcolor (WHITE);
			  mostra_mouse ();
			}
			continue;

	case '-': /* apaga o ultimo poligono */
		  if (numpoly > 0)
		  {
		    numpoly--;
		    esconde_mouse ();
		    mostra_poly (poly[numpoly]);
		    mostra_mouse ();
		  }
		  continue;
      }
    }

    get_mouse_status (&botao, &pos_x, &pos_y);

    if (x != pos_x || y != pos_y)  /* se mouse andou */
    {
      x = pos_x;
      y = pos_y;
      sprintf(txt, "(%3d,%3d)", x, y);  /* imprime novas coordenadas */
      setwritemode (0);
      limpa (fir, 10, x2, 0, BLACK);
      setwritemode (1);
      fir = x2 - 100;
      outtextxy(x2 - 100, 10, txt);
    }

    if (mouse_esquerdo (botao)) /* se botao esquerdo */
    {
      while (mouse_esquerdo (botao))
	get_mouse_status (&botao, &pos_x, &pos_y);

      /* seleciona ponto quando o botao for 'solto' */
      pontos[numpontos][X]=pos_x;
      pontos[numpontos][Y]=pos_y;
      esconde_mouse();
      setcolor (YELLOW);
      line (pontos[numpontos][X]-1, pontos[numpontos][Y],
            pontos[numpontos][X]+1, pontos[numpontos][Y]);
      line (pontos[numpontos][X], pontos[numpontos][Y]-1,
            pontos[numpontos][X], pontos[numpontos][Y]+1);
      setcolor (WHITE);
      mostra_mouse();
      numpontos++;
    }

    if (mouse_direito (botao)) /* se botao direito */
    {
      setcolor (LIGHTRED);
      /* inicia a selecao de um poligono */
      poly[numpoly].p[0][X]=pos_x;
      poly[numpoly].p[0][Y]=pos_y;
      poly[numpoly].np=1;

      do
      {
	get_mouse_status (&botao, &pos_x, &pos_y);

	if (mouse_esquerdo (botao)) /* se botao esquerdo */
	{
	  while (mouse_esquerdo (botao))
	    get_mouse_status (&botao, &pos_x, &pos_y);

	  /* vamos selecionando os pontos do poligono */
	  poly[numpoly].p[poly[numpoly].np][X]=pos_x;
	  poly[numpoly].p[poly[numpoly].np][Y]=pos_y;
	  esconde_mouse();
	  line (poly[numpoly].p[poly[numpoly].np-1][X],
		poly[numpoly].p[poly[numpoly].np-1][Y],
		poly[numpoly].p[poly[numpoly].np][X],
		poly[numpoly].p[poly[numpoly].np][Y]);
	  mostra_mouse();
	  if (poly[numpoly].np < 80) /* limite maximo de pontos */
	    poly[numpoly].np++;
	}
      }
      while (mouse_direito (botao)); /* direito continua pressionado */

      esconde_mouse();
      line (poly[numpoly].p[poly[numpoly].np-1][X],
	    poly[numpoly].p[poly[numpoly].np-1][Y],
	    poly[numpoly].p[0][X],
	    poly[numpoly].p[0][Y]);

      if (poly[numpoly].np >= 3) /* poligono tem n>=3 */
	numpoly++;
      else
	mostra_poly (poly[numpoly]); /* senao apaga */

      mostra_mouse();
      setcolor (WHITE);
    }
  }
  esconde_mouse();
  setwritemode (0);

  *np    = numpontos;
  *npoly = numpoly;
}

#ifndef UNIX

/* Funcao que le uma cadeia de caracteres de tamanho tam do teclado
   devolvendo-a em str, e mostrando-a na tela na posicao x,y */
void getstr (int x, int y, char *str, int tam)
{
  char c = 0, txt[80];
  int i = 0, j;
  sprintf(txt, "=>%s", str);
  setfillstyle(1, 0); bar(x, y, getmaxx()-1, y-10);
  outtextxy(x, y, txt);
  while (c != 13 && tam)
  {
    c = getch();
    switch (c)
    {
      case BACKSPACE: if (i>0)  /* BackSpace apaga */
		      {
			i--;
			for (j=i; j<tam-1; j++)
			  str[j] = str[j+1];
		      }
		      break;

      case ENTER: str[i] = '\0';  /* Enter termina */
		  break;

      default: str[i] = c; /* senao escrevemos o caracter */
	       if (i < tam) i++;
    }

    sprintf(txt, "=>%s", str);
    setfillstyle(1, 0); bar(x, y, getmaxx()-1, y-10);
    outtextxy(x, y, txt);
  }
}

#else

/* Caixa de dialogo para ler comandos: */
DIALOG dlg_getstr[] = {
  { CTL_WINDOW, window_proc, 0,   0,  300, 130,  0, 15,
    0, 0, 0, "Command" },
  { CTL_TEXT,   text_proc,   10,  10, 280, 20,   0, 15,
    D_AUTOH, 0, 0, "Enter command:" },
  { CTL_EDIT,   edit_proc,   10,  40, 280, 40,   0, 15,
    D_AUTOH|D_EXIT, 0, 0, NULL },
  { CTL_BUTTON, button_proc, 70,  90, 70,  30,   0, 15,
    D_AUTOH|D_EXIT, 0, 0, "OK" },
  { CTL_BUTTON, button_proc, 160, 90, 70,  30,   0, 15,
    D_AUTOH|D_EXIT, 0, 0, "Cancel" },
  { CTL_NONE, NULL }
};

void getstr (int x, int y, char *str, int tam)
{
  str[0] = '\0';
  dlg_getstr[2].dp = str;
  dlg_getstr[2].d2 = tam - 1;  /* Numero maximo de caracteres */
  dlg_getstr[2].d1 = 0;        /* Posicao inicial do cursor */

  centre_dialog(dlg_getstr, active_window);
  set_dialog_flag(dlg_getstr, D_3D, 1);
  set_dialog_flag(dlg_getstr, D_GOTFOCUS, 0);
  dlg_getstr[2].flags |= D_GOTFOCUS;

  if (do_dialog_window(active_window, dlg_getstr, 1) == 4)   /* Cancel */
    str[0] = '\0';
}

#endif /* UNIX */

/* Fim de INTERFAC.C */
