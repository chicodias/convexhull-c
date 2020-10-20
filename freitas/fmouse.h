/*
 * FMouse.h
 */
#ifndef FMOUSE_H
#define FMOUSE_H

void esconde_mouse(void);
void mostra_mouse(void);
int  verifica_mouse(void);
void posiciona_mouse(int, int);
void limita_mouse(int, int, int, int);
void get_mouse_status (int *, int *, int *);

#ifndef UNIX

/* Macro que recebe botao e responde se este eh o esquerdo */
#define mouse_esquerdo(b) ((b)&0x0001)
/* Macro que recebe botao e responde se este eh o direito */
#define mouse_direito(b)  ((b)&0x0002)

#else

int mouse_esquerdo(int);
int mouse_direito(int);

#endif /* UNIX */

#endif /* FMOUSE_H */
