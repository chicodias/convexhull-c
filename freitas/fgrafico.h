/*
 * FGrafico.h
 */
#ifndef FGRAFICO_H
#define FGRAFICO_H

/* defines para facilitar portabilidade */
#undef LIGHTMAGENTA
#undef GREEN
#undef YELLOW
#undef WHITE
#undef LIGHTRED
#undef LIGHTBLUE

#define GREEN        COLOR_BLUE
#define LIGHTMAGENTA COLOR_RED1
#define YELLOW       COLOR_GREEN
#define WHITE        0
#define LIGHTBLUE    COLOR_YELLOW
#define LIGHTRED     COLOR_MAGENTA
#define CYAN         COLOR_CYAN1
#define BLACK        15

#ifndef UNIX

#include <dos.h>
#include <graphics.h>

#define VGA   VGA
#define VGAHI VGAHI
#define grOk  grOk

#else  /* UNIX */

#define BGI_FUNCS   /* Pega as funcoes de `xdialogs.h' ao inves de macros */

#include <xdialogs.h>

#endif /* UNIX */

#ifndef ARQ_CG
extern int COR, WRITEMODE;
#endif

#define getmaxx()           getmaxx()
#define getmaxy()           getmaxy()
#define setwritemode(A)     WRITEMODE = (A), setwritemode ((A))

/*
#define setfillstyle(P,C)   setfillstyle ((P), (C))
#define setlinestyle(A,B,C) setlinestyle ((A), (B), (C))
*/

#define bar(XI,YI,XF,YF)    bar((XI), getmaxy()-(YI),\
                                (XF), getmaxy()-(YF))
#define outtextxy(X,Y,T)    outtextxy((X), getmaxy()-(Y), (T))
#define clearviewport()     clearviewport()
#define putpixel(X,Y,C)     putpixel((X), getmaxy()-(Y), (C))
#define line(XI,YI,XF,YF)   line((XI),getmaxy()-(YI),\
				 (XF),getmaxy()-(YF))
#define closegraph()        closegraph()
#define setcolor(C)         COR = (C), setcolor((C))

#define delay(X)            delay((X))

void limpa (int, int, int, int, int);
int inic_graf (void);
void line_xor (int x, int d);

#endif /* FGRAFICO.H */
