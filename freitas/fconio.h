/*
 * Fconio.h
 */
#ifndef FCONIO_H
#define FCONIO_H

#ifndef __GNUC__

#include <conio.h>

#else  /* __GNUC__ */

#define BGI_FUNCS  /* Pega funcoes em `xdialogs.h' ao inves de macros */

#include <xdialogs.h>

#endif  /* __GNUC__ */

/*
getch
kbhit
*/

#endif /* FCONIO_H */
