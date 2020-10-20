/* internal.h
 *
 * XDialogs internal functions and variables.
 */

#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef __GNUC__
#define INLINE inline
#else
#define INLINE
#endif

/* Double click time, in microsseconds */
#define DCLICK_TIME  300000

/* Number of colors in the palette */
#define PAL_SIZE     256

/* Events we want to receive notification about */
#define EV_MASK KeyPressMask|ButtonPressMask|ExposureMask|\
StructureNotifyMask|FocusChangeMask|EnterWindowMask|LeaveWindowMask

#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a)      (((a) >= 0) ? (a) : -(a))
#endif

#define DIST2(a,b)   ((a)*(a) + (b)*(b))

#define COLOR(i)   (((i) < 0 || (i) >= PAL_SIZE) ? xd_pal[0].pixel : \
		    xd_pal[i].pixel)

/* Pixel values for the colors in X */
typedef struct PALETTE {
  int pixel;                /* Pixel value (set in init_graph()) */
  int allocated;            /* 1 if allocated */
  unsigned int val[3];      /* Components (0-65536) */
} PALETTE;


extern Display *xd_display;
extern PALETTE xd_pal[PAL_SIZE];

#endif /* INTERNAL_H */
