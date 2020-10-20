/*
 * cg.c
 */
#define ARQ_CG
#include "fconio.h"
#include "fstring.h"
#include "fstdio.h"
#include "fstdlib.h"
#include "fstring.h"
#include "fmalloc.h"
#include "cg.h"
#include "dupla.h"
#include "erro.h"
#include "fio.h"
#include "fmouse.h"
#include "fgrafico.h"
#include "fio.h"
#include "graham.h"
#include "guloso.h"
#include "interfac.h"
#include "mindist.h"
#include "mwt_poly.h"
#include "pilha.h"
#include "qhull.h"
#include "triang.h"
#include "triang3.h"
#include "vetor.h"
#include "we.h"
#include "aem.h"

/* Variaveis Globais de controle */
int COR = 0, WRITEMODE = 1;
int animacao,
    delay_time,
    pare,
    indcount,
    writemode;
/* Contadores de primitivas dos algoritmos */
long count[3];

/* vetor para armazenar pontos */
_point pontos[MAX_PONTOS];
/* vetor para armazenar poligonos */
_poly poly[MAX_POLY];

double temporario;

/* Enumeracao usada para avaliar um comando */
enum
{
  INVALID = -1,
  AEMD,
  AEMG,
  ANIMATION,
  CLEAR,
  CLSCOUNT,
  COUNT,
  DELAUNAY,
  DELAY,
  EXIT,
  GRAHAM,
  GULOSO,
  LOAD,
  MEMORY,
  MINDIST,
  MINDIST2,
  MODE,
  MWT,
  QHULL,
  SAVE,
  TRIANG,
  TRIANG3,
  USECOUNT
};

/* Funcao que avalia o comando presente em 's' e retorna o resultado desta */
int avalia (char *s)
{
  int i = -1;
  char c;
  /* Substitue minisculas para maiusculas */
  while (s[++i])
    s[i] = (s[i] >= 97 && s[i] < 97+26) ? s[i]-=(97-65) : s[i];

  /* Comandos sem argumentos */
  if (strequal (s, "ANIMATION"))
    return (ANIMATION);
  if (strequal (s, "AEMD"))
    return (AEMD);
  if (strequal (s, "AEMG"))
    return (AEMG);
  else if (strequal (s, "CLEAR"))
    return (CLEAR);
  else if (strequal (s, "CLSCOUNT"))
    return (CLSCOUNT);
  else if (strequal (s, "COUNT"))
    return (COUNT);
  else if (strequal (s, "DELAUNAY"))
    return (DELAUNAY);
  else if (strequal (s, "EXIT"))
    return (EXIT);
  else if (strequal (s, "GRAHAM"))
    return (GRAHAM);
  else if (strequal (s, "GULOSO"))
    return (GULOSO);
  else if (strequal (s, "MEMORY"))
    return (MEMORY);
  else if (strequal (s, "MINDIST"))
    return (MINDIST);
  else if (strequal (s, "MINDIST2"))
    return (MINDIST2);
  else if (strequal (s, "MODE"))
    return (MODE);
  else if (strequal (s, "MWT"))
    return (MWT);
  else if (strequal (s, "QHULL"))
    return (QHULL);
  else if (strequal (s, "TRIANG"))
    return (TRIANG);
  else if (strequal (s, "TRIANG3"))
    return (TRIANG3);
  else
  {
    /* Agora comandos com 1 argumento */
    c=s[4]; /* de tamanho 4 */
    s[4]=0;
    if (strequal (s, "LOAD"))
      return (LOAD);
    if (strequal (s, "SAVE"))
      return (SAVE);
    s[4]=c;
    c=s[5]; /* de tamanho 5 */
    s[5]=0;
    if (strequal (s, "DELAY"))
      return (DELAY);
    s[5]=c;
    c=s[8]; /* de tamanho 8 */
    s[8]=0;
    if (strequal (s, "USECOUNT"))
      return (USECOUNT);
    s[8]=c;

  }

  /* se nao encontrou o comando */
  return (INVALID);
}

/* Funcao que reclama da falta de pontos para um determinado algoritmo */
void poucos_pontos (int n)
{
  char txt[80];
  sprintf(txt, "You should select more than %d points...", n);
  
  /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
  limpa(0, 10, getmaxx()-1, 0, BLACK);
  outtextxy(10, 10, txt);
  getch();
  clearviewport();
}

/* mostra um "Ok." */
void ok (void)
{
  char str[5];
  sprintf(str, "Ok.");
  /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
  limpa(0, 10, getmaxx()-1, 0, BLACK);
  outtextxy(10, 10, str);
}

/* Funcao que verifica se o poligono p eh simples procurando por
   interseccoes de arestas, duas a duas. */
int poly_simples (_poly *p)
{
  char str[40];
  int i, j, dt = delay_time, an = animacao;
  delay_time = animacao = 0;

  for (i=1; i < p->np; i++)
    for (j=i+1; j <= p->np; j++)
      if (cruza (p->p[i-1], p->p[i], p->p[j-1], p->p[j%p->np]))
      {
	sprintf(str, "Invalid polygon (not simple).");
	/* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
	limpa(0, 10, getmaxx()-1, 0, BLACK);
	outtextxy(10, 10, str);
	delay_time = dt;
	animacao = an;
	return (FALSE);
      }
  delay_time = dt;
  animacao = an;
  return (TRUE);
}

/* Vamos com o corpo do programa */
int main(int argc, char *argv[])
{
  int np = 0,     /* num de pontos selecionados */
      npoly = 0,  /* num de poligonos selecionados */
      i;
  char dirtests[80], buffer[80];
  FILE *fp;
  _pilha pilha;
  int sair = FALSE;  /* para sair do programa */
  char str[80];    /* buffer para conter o comando */
  _point a, b;

#ifdef UNIX
  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-install") == 0)
      xd_install_colormap = 1;
    else if (argv[i][0] == '-' && argv[i][1] == 't')
      strcpy(dirtests, &argv[i][2]);
#else
  /* apenas para nao aparecer warnings */
  if (argc != 1)
    argv[1][0] = 0;
#endif

  if (!(inic_graf())) {
    printf("I can't change to graphic mode.\n");
    exit(1);
  }

  /* Setup inicial */
  animacao = TRUE;
  delay_time= 100;
  indcount = 0;
  writemode = 1;

  while (!sair)
  {
    selecaopontos (&np, &npoly);
    sprintf (str, "               ");
    /* pega o comando */
    getstr (10, 10, str, 40);

    if (!str[0])
      continue;

    pare = FALSE; /* controle de parada dos algoritmos */

    /* avaliamos e decidimos o que fazer */
    switch (avalia (str))
    {
      case ANIMATION:  animacao = !animacao;
		       ok ();
		       break;

      case CLEAR:  clearviewport(); /* limpa tela */
		   np = 0;          /* e zera os dados */
		   npoly = 0;
		   break;

      case CLSCOUNT:  count[indcount] = 0; /* limpa contador */
		      ok ();
		      break;

      case COUNT:  /* exibe contadores */
		   sprintf(str, "Count1: %ld / Count2: %ld / Count3: %ld",
			   count[0], count[1], count[2]);
		   /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		   limpa(0, 10, getmaxx()-1, 0, BLACK);
		   outtextxy(10, 10, str);
		   break;

      case DELAUNAY:
                   if (np < 3)
		     poucos_pontos (3);
		   else
		   {
                     _aresta *conj, *a;
                     double peso = 0.0;
                     if ((conj = delaunay_nlogn (np)) == NULL)
		     {  /* dados com "problema"? */
		       sprintf(str,"Invalid set (2 points with same X)");
		       /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		       limpa(0, 10, getmaxx()-1, 0, BLACK);
		       outtextxy(10, 10, str);
		     }
                     else
                     {
                       for (a = conj; a->inicio != -1; a++)
                         peso += a->peso;
                       sprintf(str,"Triangulation weight: %f", peso);
                       /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
                       limpa(0, 10, getmaxx()-1, 0, BLACK);
                       outtextxy(10, 10, str);
                       free (conj);
                     }

		   }
		   break;

      case AEMD:
                   if (np < 3)
                     poucos_pontos (3);
                   else
                   {
                     _aresta *conj, *cj2;
                     animacao = 0;
                     if ((conj = delaunay_nlogn (np)) == NULL)
                     {  /* dados com "problema"? */
                       sprintf(str,"Invalid set (2 points with same X)");
                       /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
                       limpa(0, 10, getmaxx()-1, 0, BLACK);
                       outtextxy(10, 10, str);
                     }
                     else
                     {
		       _aresta *a;
                       animacao = 1;
                       cj2 = aem (np, conj);

		       temporario = 0.0;
		       for (a = cj2; a->inicio != -1; a++)
			 temporario += a->peso;

                       tree_to_poly (np, cj2, &poly[npoly]);
                       npoly++;
                       setwritemode (0);
                       mostra_poly (poly[npoly-1]);
                 
                       sprintf(str,"Ok.");
                       /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
                       limpa(0, 10, getmaxx()-1, 0, BLACK);
                       outtextxy(10, 10, str);
                       free (conj);
                     }

                   }
                   break;
                   
 
      case DELAY:  delay_time = atoi(&str[6]);
		   ok ();
		   break;

      case EXIT:  sair = TRUE;
		  break;

      case GRAHAM:  if (np < 3)
		      poucos_pontos (3);
		    else
		    {
		      pilha = graham (np);
		      while (pilha != NULL)
			pilha = desempilha (pilha);
		    }
		    break;

      case GULOSO:  if (np < 2)
		      poucos_pontos (2);
		    else {
                      double peso = 0.0;
		      _aresta *conj = guloso (np), *a;
                      for (a = conj; a->inicio != -1; a++)
                        peso += a->peso;
                      sprintf(str, "Triangulation weight: %f", peso);
                      limpa(0, 10, getmaxx()-1, 0, BLACK);
                      outtextxy(10, 10, str);
                      free (conj);
                    }
		    break;

      case AEMG:    if (np < 2)
                      poucos_pontos (2);
                    else {
                      _aresta *conj, *cj2, *a;
                      animacao = 0;
                      conj = guloso (np);
                      animacao = 1;
                      cj2 = aem (np, conj);

		      temporario = 0.0;
                      for (a = cj2; a->inicio != -1; a++)
                        temporario += a->peso;

                      tree_to_poly (np, cj2, &poly[npoly]);
                      npoly++;
                      setwritemode (0);
                      mostra_poly (poly[npoly-1]);

                      sprintf(str, "Ok.");
                      limpa(0, 10, getmaxx()-1, 0, BLACK);
                      outtextxy(10, 10, str);
                      free (conj);
                      free (cj2);
                    }
                    break;
                         

      case INVALID:  sprintf(str, "Invalid Command");
		     /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		     limpa(0, 10, getmaxx()-1, 0, BLACK);
		     outtextxy(10, 10, str);
		     break;

      case LOAD:  strcpy(buffer, dirtests);
                  strcat (buffer, "/");
                  strcat (buffer, &str[5]);
                  if ((i = le_arq(buffer, &np, &npoly)) == 0)
		  {
		    /* achou arquivo ? */
		    sprintf(str, "I Can't open %s.", buffer);
		    /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		    limpa(0, 10, getmaxx()-1, 0, BLACK);
		    outtextxy(10, 10, str);
		    break;
		  }
		  else if (i == -1) /* erro no arquivo de entrada ? */
		  {
		    sprintf(str, "Bad file source.");
		    /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		    limpa(0, 10, getmaxx()-1, 0, BLACK);
		    outtextxy(10, 10, str);
		    break;
		  }

		  /* mostra dados lidos */
		  clearviewport();
		  setwritemode (writemode);
		  for (i=0; i<np; i++) {
                    putpixel (pontos[i][X]-1, pontos[i][Y], YELLOW);
                    putpixel (pontos[i][X]+1, pontos[i][Y], YELLOW);
                    putpixel (pontos[i][X], pontos[i][Y], YELLOW);
                    putpixel (pontos[i][X], pontos[i][Y]+1, YELLOW);
	            putpixel (pontos[i][X], pontos[i][Y]-1, YELLOW);
                  }
		  for (i=0; i<npoly; i++)
		    mostra_poly (poly[i]);
		  setwritemode (0);

		  ok ();
		  break;

#ifndef UNIX
      case MEMORY:  sprintf(str, "Avaliable memory: %u", coreleft());
		    setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0);
		    outtextxy(10, 10, str);
		    break;
#endif /* UNIX */

      case MINDIST:  if (np < 2)
		       poucos_pontos (2);
		     else
		     {
		       mindist_nlogn(&a, &b, np);

		       /* mostra solucao em destaque */
		       setlinestyle (3, 0, 2);
		       setcolor (LIGHTMAGENTA);
		       line (a[X], a[Y], b[X], b[Y]);
		       setlinestyle (0, 0, 2);
		       setcolor (WHITE);
		     }
		     break;

      case MINDIST2:  if (np < 2)
			poucos_pontos (2);
		      else
		      {
			mindist_quad (&a, &b, np);

			/* mostra a solucao em destaque */
			setlinestyle (3, 0, 2);
			setcolor (LIGHTMAGENTA);
			line (a[X], a[Y], b[X], b[Y]);
			setlinestyle (0, 0, 2);
			setcolor (WHITE);
		      }
		      break;

      case MODE:  writemode = !writemode;
	          ok ();
	          break;

      case MWT:    if (npoly > 0)
                     if (poly_simples (poly)) {
                       sprintf(str, "Peso da triangularizacao: %f",
                               mwt_poligono (poly[npoly-1])+temporario);
                       limpa(0, 10, getmaxx()-1, 0, BLACK);
                       outtextxy(10, 10, str);
                     }
                   break;

      case QHULL:  if (np < 3)
		     poucos_pontos (3);
		   else
		   {
		     pilha = quickhull (np);
		     while (pilha != NULL)
		       pilha = desempilha (pilha);
		   }
		   break;

      case SAVE:  strcpy(buffer, dirtests);
                  strcat(buffer, "/");
                  if ((fp = fopen (strcat(buffer, &str[5]), "r")) != NULL) 
                  /* arq ja existe */
		  {
		    fclose (fp);
		    sprintf(str, "File %s already exist.", &str[5]);
		    /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		    outtextxy(10, 10, str);
		    break;
		  }

		  if (!salva_arq (buffer, np, npoly)) /* criou? */
		  {
		    sprintf(str, "I Can't create %s.", buffer);
		    /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
		    limpa(0, 10, getmaxx()-1, 0, BLACK);
		    outtextxy(10, 10, str);
		  }
		  else
		    ok ();
		  break;

      case TRIANG: if (npoly > 0)
		     if (poly_simples (poly)) {
                       double peso;
		       if ((peso = triang (poly[npoly-1])) == -1)
		       {  /* dados com "problema"? */
			 sprintf(str,"Invalid polygon (2 points with same Y)");
			 /* setfillstyle(1, 0); bar(0, 10, getmaxx()-1, 0); */
			 limpa(0, 10, getmaxx()-1, 0, BLACK);
			 outtextxy(10, 10, str);
		       }
                       else {
                         sprintf(str, "Peso da triangularizacao: %f", peso);        
                         limpa(0, 10, getmaxx()-1, 0, BLACK);
                         outtextxy(10, 10, str);
                       }
                     }

		   break;

      case TRIANG3:  if (npoly > 0)
		       if (poly_simples (poly)) {
			 sprintf(str, "Peso da triangularizacao: %f", 
                                 triang3 (poly[npoly-1]));
                         limpa(0, 10, getmaxx()-1, 0, BLACK);
                         outtextxy(10, 10, str);
                       }
		     break;

      case USECOUNT:  /* seleciona contador a ser usado */
		      indcount = (atoi (&str[9]) > 0) ?
				  atoi (&str[9])-1 : indcount;
		      ok ();
		      break;

    }
  }
  closegraph();
  return (0);
}
/* Fim do CG.C */
