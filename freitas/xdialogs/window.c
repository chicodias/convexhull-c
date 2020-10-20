/* window.c
 *
 * XDialogs main file.
 * 
 * Ricardo R. Massaro (massaro@ime.usp.br)
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "xdialogs.h"
#include "internal.h"

char str_version[] = "XDialogs 1.0, by Ricardo R. Massaro";
WINDOW *active_window = NULL;
int xd_no_sent_events;
FONT *xd_default_font;
char xd_font_name[256] = "fixed";
Colormap xd_colormap;

int xd_install_colormap;
int gui_fg_color, gui_bg_color, gui_mg_color;
int xd_stddlg_3d;
int xd_light_3d, xd_shadow_3d, xd_back_3d;

Display *xd_display = NULL;
Cursor xd_arrow_cursor, xd_text_cursor;

static WINDOW root_window[1];

#define INI_PAL_SIZE  16    /* Number of colors initially in the pallete */

PALETTE xd_pal[PAL_SIZE] = {
  { 0, 0, { 0,     0,     0     } },    /* Black       */
  { 0, 0, { 0,     0,     32767 } },    /* Blue        */
  { 0, 0, { 0,     32767, 0     } },    /* Green       */
  { 0, 0, { 0,     32767, 32767 } },    /* Cyan        */
  { 0, 0, { 32767, 0,     0     } },    /* Red         */
  { 0, 0, { 32767, 0,     32767 } },    /* Magenta     */
  { 0, 0, { 32767, 32767, 0     } },    /* Yellow      */
  { 0, 0, { 50176, 50176, 50176 } },    /* Light Gray  */
  { 0, 0, { 32767, 32767, 32767 } },    /* Dark gray   */
  { 0, 0, { 0,     0,     65535 } },    /* Blue        */
  { 0, 0, { 0,     65535, 0     } },    /* Green       */
  { 0, 0, { 0,     65535, 65535 } },    /* Cyan        */
  { 0, 0, { 65535, 0,     0     } },    /* Red         */
  { 0, 0, { 65535, 0,     65535 } },    /* Magenta     */
  { 0, 0, { 65535, 65535, 0     } },    /* Yellow      */
  { 0, 0, { 65535, 65535, 65535 } },    /* White       */
};

/* Add `win' to the list of `win->parent' child windows */
static void add_window(WINDOW *win)
{
  WINDOW *w, *parent;

  parent = (win->parent != NULL) ? win->parent : root_window;

  if (parent->child == NULL) {
    parent->child = win;
    return;
  }

  for (w = parent->child; w->sibling != NULL; w = w->sibling)
    ;
  w->sibling = win;
}

/* Remove `win' from the list of `win->parent' child windows */
static void remove_window(WINDOW *win)
{
  WINDOW *parent, *w, *tmp;

  /* Remove the window from the parent list of child windows. */
  if (win != root_window) {
    parent = (win->parent != NULL) ? win->parent : root_window;

    tmp = NULL;
    for (w = parent->child; w != NULL; tmp = w, w = w->sibling)
      if (w == win)
	break;
    if (w == NULL) {
#if DEBUG
      fprintf(stderr,
	      "remove_window(): can't find child `%p' among `%p' childs\n",
	      win, parent);
#endif
      return;
    }
    if (tmp == NULL)
      parent->child = win->sibling;
    else
      tmp->sibling = win->sibling;
  }

  /* Remove all childs */
  if (win->child != NULL) {
#if DEBUG
    fprintf(stderr, "remove_window(): trying to remove non-leaf window\n");
#endif

    /* This makes `root_window' adopt `win' childs: */
    for (w = win->child; w != NULL; w = w->sibling)
      w->parent = NULL;
  }
}

/* Create the default font */
static FONT *create_default_font(void)
{
  FONT *f;

  f = (FONT *) malloc(sizeof(FONT));
  f->num_refs = 0;
  if (f == NULL)
    return NULL;
  f->font_struct = XLoadQueryFont(xd_display, xd_font_name);
  if (f->font_struct == NULL) {
    f->font_struct = XLoadQueryFont(xd_display, "fixed");
    if (f->font_struct == NULL)
      return NULL;
  }
  f->font_x = f->font_struct->min_bounds.lbearing;
  f->font_y = f->font_struct->max_bounds.ascent;
  f->num_refs = 0;
  return f;
}

/* Initializes the graphics system */
int init_graph(char *disp_name)
{
  int i;

  xd_display = XOpenDisplay(disp_name);
  if (xd_display == NULL)
    return -1;

  xd_no_sent_events = 0;
  root_window->child = root_window->sibling = NULL;
  active_window = NULL;

  xd_light_3d = COLOR_WHITE;
  xd_shadow_3d = COLOR_GRAY1;
  xd_back_3d = COLOR_GRAY;
  gui_fg_color = COLOR_BLACK;
  gui_bg_color = COLOR_WHITE;
  xd_stddlg_3d = 0;

  /* Read the font */
  xd_default_font = create_default_font();
  if (xd_default_font == NULL) {
    closegraph();
    return -1;
  }

  /* Load the cursors */
  xd_arrow_cursor = XCreateFontCursor(xd_display, XC_left_ptr);
  xd_text_cursor = XCreateFontCursor(xd_display, XC_xterm);

  /* Load the colors */
  if (xd_install_colormap) {
    XColor colors[PAL_SIZE];

    xd_colormap = XCreateColormap(xd_display,
				  DefaultRootWindow(xd_display),
				  DefaultVisual(xd_display,
						DefaultScreen(xd_display)),
				  AllocAll);

    for (i = 0; i < PAL_SIZE; i++) {
      colors[i].pixel = i;
      colors[i].flags = DoRed | DoGreen | DoBlue;
      if (i < INI_PAL_SIZE) {
	colors[i].red   = xd_pal[i].val[0];
	colors[i].green = xd_pal[i].val[1];
	colors[i].blue  = xd_pal[i].val[2];
      } else {
	colors[i].red   = 0;
	colors[i].green = 0;
	colors[i].blue  = 0;
      }
      xd_pal[i].pixel = i;
    }
    XStoreColors(xd_display, xd_colormap, colors, PAL_SIZE);
  } else {
    XColor color;

    xd_colormap = DefaultColormap(xd_display, DefaultScreen(xd_display));
    color.flags = DoRed|DoGreen|DoBlue;
    for (i = 0; i < INI_PAL_SIZE; i++) {
      color.red   = xd_pal[i].val[0];
      color.green = xd_pal[i].val[1];
      color.blue  = xd_pal[i].val[2];
      XAllocColor(xd_display, xd_colormap, &color);
      xd_pal[i].val[0] = color.red;
      xd_pal[i].val[1] = color.green;
      xd_pal[i].val[2] = color.blue;
      xd_pal[i].pixel = color.pixel;
      xd_pal[i].allocated = 1;
    }
  }

  if (xd_install_colormap)
    XInstallColormap(xd_display, xd_colormap);
  else {
    /* This may improve appearance of 3D controls in systems with
     * a small number of colors.
     */
    if (xd_pal[COLOR_GRAY].pixel == xd_pal[COLOR_BLACK].pixel)
      xd_pal[COLOR_GRAY].pixel = xd_pal[COLOR_WHITE].pixel;
    if (xd_pal[COLOR_GRAY1].pixel == xd_pal[COLOR_GRAY].pixel)
      xd_pal[COLOR_GRAY1].pixel = xd_pal[COLOR_BLACK].pixel;
  }

  return 0;
}

/* Shuts down the graphics system */
void closegraph(void)
{
  Colormap cmap;

  if (xd_display == NULL)
    return;

  /* Closes all windows */
  close_window(root_window);

  /* Free the allocated colors */
  cmap = DefaultColormap(xd_display, DefaultScreen(xd_display));
  XFreeColors(xd_display, cmap, NULL, 0, AllPlanes);

  XCloseDisplay(xd_display);
}

/* Create a dialog window belonging to `parent' */
void create_dialog_window(WINDOW *parent, DIALOG *dlg)
{
  dlg->win = create_window_x(NULL, dlg->x, dlg->y, dlg->w, dlg->h,
			     0, parent, NULL);
  dlg->win->dialog = dlg;
  XSetWindowBackground(dlg->win->disp, dlg->win->window, COLOR(dlg->bg));

  /* The window will keep hidden while the parent is hidden */
  win_showwindow(dlg->win);
}

/* Creates a new window of size w, h */
WINDOW *create_window(char *title, int x, int y, int w, int h,
		      WINDOW *parent, DIALOG *dlg)
{
  WINDOW *win;
  if (xd_display == NULL)
    return NULL;

  win = create_window_x(title, x, y, w, h, 1, parent, dlg);
  if (win != NULL)
    win->is_child = 0;
  return win;
}

/* Create a window */
WINDOW *create_window_x(char *title, int x, int y, int w, int h,
			int managed, WINDOW *parent, DIALOG *dlg)
{
  WINDOW *win;
  char *argv[] = { "xdialogs", NULL };
  XSizeHints hints;
  unsigned long fg, bg;
  char *name = NULL;

  if (xd_display == NULL)
    return NULL;

  if (dlg != NULL && dlg->d4 != 0) {
    printf("Can't create window because the dialog is being used\n");
    return NULL;    /* This dialog is being used by another window */
  }

  if ((win = malloc(sizeof(WINDOW))) == NULL)
    return NULL;
  memset(win, 0, sizeof(WINDOW));

  name = title;
  hints.flags = PSize | PPosition;
  if (dlg != NULL) {
    win->color = dlg[0].fg;
    win->bkcolor = dlg[0].bg;
    win->x = hints.x = dlg[0].x;
    win->y = hints.y = dlg[0].y;
    win->w = hints.width = dlg[0].w;
    win->h = hints.height = dlg[0].h;
    if (dlg->dp != NULL)
      name = dlg->dp;
  } else {
    win->color = COLOR_BLACK;
    win->bkcolor = COLOR_WHITE;
    win->x = hints.x = x;
    win->y = hints.y = y;
    win->w = hints.width = w;
    win->h = hints.height = h;
  }
  fg = COLOR(win->color);
  bg = COLOR(win->bkcolor);

  win->dialog = dlg;
  win->parent = parent;
  win->child = win->sibling = NULL;
  win->pointer_grabbed = 0;
  win->is_child = 1;
  win->selected = 1;

  win->disp = xd_display;
  win->depth = DefaultDepth(xd_display, DefaultScreen(xd_display));

  /* Create the window and set its properties */
  win->window = XCreateSimpleWindow(xd_display,
				    ((managed || parent == NULL) ?
				     DefaultRootWindow(xd_display) :
				     parent->window),
				    hints.x, hints.y,
				    hints.width, hints.height, 1, fg, bg);
  if (managed)
    XSetStandardProperties(xd_display, win->window, name, name,
			   None, argv, 1, &hints);
  XSelectInput(xd_display, win->window, EV_MASK);
  if (! managed) {
    XSetWindowAttributes wa;

    wa.override_redirect = True;
    XChangeWindowAttributes(xd_display, win->window,
			    CWOverrideRedirect, &wa);
  }

  /* Create the gc that we will  use to draw */
  win->gc = XCreateGC(xd_display, win->window, 0ul, (XGCValues *) NULL);
  XSetBackground(xd_display, win->gc, bg);
  XSetForeground(xd_display, win->gc, fg);
  win->image = None;
  win->font = xd_default_font;
  XSetFont(xd_display, win->gc, win->font->font_struct->fid);

  /* Clear the keyboar buffer */
  win->pkb_i = win->pkb_f = 0;

  /* Create the dialog window controls */
  if (dlg != NULL) {
    int i;

    dlg->win = win;
    dlg->d4 = 1;     /* Mark this dialog as `with window' */
    for (i = 1; dlg[i].proc != NULL; i++)
      create_dialog_window(win, dlg + i);
  }

  XSetWindowColormap(xd_display, win->window, xd_colormap);

  add_window(win);
  if (parent != NULL && managed)
    XSetTransientForHint(xd_display, win->window, parent->window);
  XDefineCursor(xd_display, win->window, xd_arrow_cursor);

  if (active_window == NULL)
    active_window = win;

  return win;
}

/* Show a window */
void win_showwindow(WINDOW *win)
{
  XMapRaised(win->disp, win->window);
  XFlush(win->disp);
}

/* Hide a window */
void win_hidewindow(WINDOW *win)
{
  XUnmapWindow(win->disp, win->window);
  XFlush(win->disp);
}

/* Set the position of a window */
void win_movewindow(WINDOW *win, int x, int y)
{
  win->x = x;
  win->y = y;
  XMoveWindow(win->disp, win->window, x, y);
}

/* Set the size of a window */
void win_resizewindow(WINDOW *win, int w, int h)
{
  win->w = w;
  win->h = h;
  XResizeWindow(win->disp, win->window, w, h);
}

/* Closes and destroys the window `win' */
void close_window(WINDOW *win)
{
  WINDOW *ch, *tmp;

  if (win == NULL)
    return;

  /* Close the childs */
  ch = win->child;
  while (ch != NULL) {
    tmp = ch->sibling;
    close_window(ch);
    ch = tmp;
  }
  remove_window(win);

  /* Now really close the window */
  if (win != root_window) {
    if (win->image != None)
      XFreePixmap(win->disp, win->image);
    free_font(win->font);
    XFreeGC(win->disp, win->gc);
    XDestroyWindow(win->disp, win->window);
    XFlush(win->disp);
    if (win->dialog != NULL)
      win->dialog->d4 = 0;    /* Mark it's not being used anymore */
    free(win);
  }
}

#if DEBUG
int dump_win_list(WINDOW *root, int start)
{
  WINDOW *w;

  printf("%*s%p\n", start, "", root);
  for (w = root->child; w != NULL; w = w->sibling)
    dump_win_list(w, start + 2);
  return 0;
}
#endif

static WINDOW *do_search_window_handle(WINDOW *parent, Window win)
{
  WINDOW *w, *tmp;

  if (parent == NULL)
    return NULL;

  if (parent->window == win)
    return parent;

  for (w = parent->child; w != NULL; w = w->sibling)
    if ((tmp = do_search_window_handle(w, win)) != NULL)
      return tmp;
  return NULL;
}

/* Return the WINDOW structure that corresponds to w */
WINDOW *search_window_handle(Window w)
{
  return do_search_window_handle(root_window, w);
}

/* Return the screen width */
int screen_width(void)
{
  return WidthOfScreen(DefaultScreenOfDisplay(xd_display));
}

/* Return the screen height */
int screen_height(void)
{
  return HeightOfScreen(DefaultScreenOfDisplay(xd_display));
}

/* Makes `win' the active window */
void set_active_window(WINDOW *win)
{
  active_window = win;
}

/* Create a new font */
FONT *create_font(char *font_name)
{
  FONT *f;

  /* FIXME: this function should keep a list of font names (perhaps
   * add a `name' field in the FONT structure) and return the existing
   * font if it was loaded before.
   */
  f = (FONT *) malloc(sizeof(FONT));
  f->num_refs = 0;
  if (f == NULL)
    return NULL;
  f->font_struct = XLoadQueryFont(xd_display, font_name);
  if (f->font_struct == NULL) {
    free(f);
    return NULL;
  }
  f->font_x = f->font_struct->min_bounds.lbearing;
  f->font_y = f->font_struct->max_bounds.ascent;
  f->num_refs = 0;
  return f;
}

/* Free a font */
void free_font(FONT *f)
{
  if (f->num_refs-- > 0 || f == xd_default_font)
    return;
  if (f->font_struct != NULL)
    XFreeFont(xd_display, f->font_struct);
#if DEBUG
  else
    fprintf(stderr, "free_font(): empty font!\n");
#endif
  f->font_struct = NULL;
  free(f);
}

/* Store the selection in the display buffer */
void win_set_selection(WINDOW *win, char *sel, int n)
{
  Atom selection;

  /* Get or create the primary selection atom */
  selection = XInternAtom(win->disp, "PRIMARY", False);
  /* Set the selection owner to `none': I wonder if it always works? */
  XSetSelectionOwner(win->disp, selection, None, CurrentTime);
  /* Now, store the selection in the cut buffer */
  XStoreBytes(win->disp, sel, n);
}

/* Get the selection from the display buffer */
int win_get_selection(WINDOW *win, char *sel, int max)
{
  int n;
  char *p;
  
  p = XFetchBytes(win->disp, &n);
  if (p != NULL) {
    strncpy(sel, p, MIN(max, n));
    XFree(p);
  }
  return MIN(max, n);
}

#if DEBUG
/* Display information on the selection */
void selection_info(void)
{
  Window w;
  int i, n;
  char *p;

  w = XGetSelectionOwner(xd_display, 1);
  printf("Selection owner window: 0x%x\n", (unsigned) w);
  p = XFetchBytes(xd_display, &n);
  if (p != NULL)
    printf("XFetchBytes gives `%.*s'\n", n, p);
  else
    printf("XFetchBytes gives NULL\n");
  for (i = 0; i < 8; i++) {
    p = XFetchBuffer(xd_display, &n, i);
    if (p != NULL)
      printf("buffer[%d] = `%.*s'\n", i, n, p);
    else
      printf("buffer[%d] = NULL\n", i);
  }
}
#endif
