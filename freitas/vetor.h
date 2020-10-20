/*
 * Vetor.h
 */
#ifndef VETOR_H
#define VETOR_H

#include "cg.h"

/* Algumas funcoes com vetores, de importancia indescritivel, presentes
   em O'Rourke, "Computational Geometry in C". */
long     area2     (_point, _point, _point);
int      is_fecho  (int, int, unsigned int);
int      xor       (int, int);
int      left      (_point, _point, _point);
int      left_on   (_point, _point, _point);
int      collinear (_point, _point, _point);
void     subvec    (_point, _point, _point);
int      dot       (_point, _point);
int      length2   (_point);
void     attrib    (_point *, _point);
int      cruza     (_point, _point, _point, _point);
int      intersec  (_point, _point, _point, _point);
int      incircle  (_point, _point, _point, _point);

#define right(x,y,z) !left_on((x),(y),(z))
#define is_fecho2(x,y,z) (is_fecho((x),(y),(z)) || is_fecho((y),(x),(z)))

#endif /* VETOR_H */
