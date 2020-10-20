
======================================================================
INTRODUCTION
======================================================================

This is XGUI, a library to provide a simple graphical user interface
in X. It is written in plain Xlib, so it should be highly portable. It
was written for Linux, but should be compiled with no problems in any
system with X11 and a ANSI C compiler.

The library can be used for two main purposes: write programs in X
that have one or more simple output windows where you can draw and
output text, or write programs that have windows with controls like
menus, buttons, text display, text input, etc.

The drawing functions were made as close as possible to the BGI
library (from Borland). The GUI related functions were made as close
as possible to the Allegro library (by Shawn Hargreaves). Both these
libraries are for MS-DOS; BGI is used in Borland/Turbo C and Turbo
Pascal, and Allegro is free software, the sources are available from
http://www.talula.demon.co.uk/allegro/. You don't need to know any of
these libraries to use XGUI, this document gives all information you
should have to start writting programs using XGUI.

Although XGUI has little or no code from Allegro, you will find that
the data structures and functions for creating dialogs are very
similar; this was made so for making easier for the ones that have had
some experience with the Allegro GUI.

XGUI was written in July 1997 by Ricardo R. Massaro. You may freely
copy, change and redistribute it. It is provided with NO WARRANTY.


======================================================================
USING XGUI
======================================================================

In this document, I assume that you know how to write programs in C
and have some experience using X. You don't need to know anything
about programming specifically in X.

This section is divided in two parts: (a) first we will quickly see
what you have to do to create a simple window and how to get input
from it and draw on it; (b) then, we will see how to create dialog
windows.


a) CREATING SIMPLE WINDOWS

In the simplest case, a program can use XGUI to create a window and
draw on it. It is also possible to get input from the mouse and the
keyboard. In this case, you can think of the window being something
like a screen (like a DOS program, for example).

Every program using XGUI must initialize the graphics system by
calling init_graph() before using any XGUI function. This function
will open a connection to the X server and set up some variables; this
must be done BEFORE the program calls any other function from
XGUI. The function init_graph() accepts one parameter, the display
name. If you don't know what it is, don't be worried, it is always
safe to pass NULL to this so that the default name is used. The only
case that you would want to use another value would be if you wanted
to use another display (for instance, if you wanted to make your
program show in a X server running in another machine of a network).

Before the program finishes, you should call closegraph(). It will
close all windows that you haven't explicity closed and free all X
resources allocated by the program.

After calling init_graph(), you should create a window using the
create_window() function. This function accepts some arguments:

- (char *)    the title of the window
- (int)       the width of the window, in pixels (can be changed)
- (int)       the height of the window, in pixels
- (WINDOW *)  the parent window (normally it would be NULL)
- (DIALOG *)  the dialog to put in the window (NULL, unless you are
              creating a window with a dialog)

This function returns a pointer to a WINDOW structure corresponding to
the created window. If your program have only one window, you can
ignore it, as will be explained now. All functions that get input and
write output to the window have two versions: a version for any window
and a version for the active window. The version for any window has
the prefix `win_' and the rest of the name is equal to the name of the
corresponding function for the active window. For example, the
functions to draw a line are

  void win_line(WINDOW *win, int x1, int y1, int x2, int y2);
  void line(int x1, int y1, int x2, int y2);

The first, win_line(), will draw a line from (x1,y1) to (x2,y2) int
the window corresponding to the WINDOW structure pointed to by
`win'. The second, line(), will draw the same line in the "active"
window.

When you create the first window in your program, it will automatically
become the active window. After that, you can set the selected window
with the function

  void set_active_window(WINDOW *win);

Now, let's see a program that creates a window, draws a line in it,
waits for a keystroke and then terminates:

----------------------------------------------------------------------
#include <stdio.h>
#include <xgui.h>

int main(void)
{
  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }

  create_window("My Window", 300, 300, NULL, NULL);
  setcolor(12);     /* Set the color for drawing functions */
  line(10, 10, 50, 70);
  read_key();
  closegraph();
  return 0;
}
----------------------------------------------------------------------

Notice that we ignored completely the WINDOW * pointer returned by
create_window(), and then used the functions to output to the
"selected window". If you run this program, you may notice that if the
window is hidden and then displayed again, the line will
disappear. This happens because, by default, XGUI doesn't "remember"
the contents of the window, and when it must be redrawn, the contents
is lost. To make XGUI "remember" the contents of the window, you must
call win_createimage(WINDOW *w) (or the short verion,
createimage()). After that, all output drawn to the window will be
stored, and when the window must be redrawn, the setored image will be
used. Try recompiling the program after adding a line `createimage();'
after the line that creates the window, and see the difference. The
only reason why XGUI doesn't create a image by default is that the
image consumes memory, and maybe you don't need to store the contents
the window.

Now let's see an example with two windows:

----------------------------------------------------------------------
#include <stdio.h>
#include <xgui.h>

int main(void)
{
  WINDOW *w1, *w2;

  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }
  w1 = create_window("First", 300, 100, NULL, NULL);
  w2 = create_window("Second", 300, 100, NULL, NULL);
  win_createimage(w1);
  win_createimage(w2);

  win_outtextxy(w1, 10, 10, "This is the first window");
  win_outtextxy(w2, 10, 10, "This is the second window");

  /* Wait until the user has pressed a key in the both windows: */
  while (! (win_has_key(w1) && win_has_key(w2)))
    ;

  closegraph();
  return 0;
}
----------------------------------------------------------------------


b) CREATING DIALOGS

A dialog window is a window with sub-windows containing "controls". A
control is represented by a DIALOG structure (a dialog, in fact, is an
array of DIALOG structures: strange, no?). Here's the DIALOG
structure:

  typedef struct DIALOG {
    int (*proc)();                       /* Procedure */
    int x, y;                            /* Position (in the dialog window) */
    int w, h;                            /* Size (width, height) */
    unsigned long fg, bg;                /* Colors (fore-/background) */
    int flags;                           /* Flags */
    int d1, d2;                          /* Data */
    void *dp, *dp1;                      /* Data pointers */
    long d3;                             /* More data */
    int draw_x, draw_y, draw_w, draw_h;  /* Exposed rectangle */
  } DIALOG;

When XGUI has something to say to the control, like "redraw yourself",
or "the mouse has just entered your area", etc., it calls the function
pointed to by the member `proc'. The function should be like

  int control_proc(int message, WINDOW *win, DIALOG *dlg, int c);

The `message' will be one of the MSG_xxx constants, shown below;
`win' will be a pointer to the window that owns the dialog; `dlg' will
be a pointer to the DIALOG structure that represents the control, and
`c' is any message-specific data (for example, in a MSG_KEY message,
it contains the code of the key that was pressed).

As promised, here are the MSG_xxx constants:

  enum {   /* Messages */
    MSG_INIT,        /* Initializing dialog */
    MSG_END,         /* Terminating dialog */
    MSG_IDLE,        /* Sent periodically */
    MSG_DRAW,        /* The control must draw itself */
    MSG_KEY,         /* A key was pressed (key in `c') */
    MSG_CLICK,       /* The mouse button was pressed */
    MSG_WANTFOCUS,   /* Querying if the control wants the focus */
    MSG_GOTFOCUS,    /* The control has gained the input focus */
    MSG_LOSTFOCUS,   /* The control has lost the input focus */
    MSG_GOTMOUSE,    /* The mouse is going into the control */
    MSG_LOSTMOUSE,   /* The mouse is leaving the control */
    MSG_RESIZE,      /* The window size has changed */
  };

The function should return a D_xxx exit status from the list:

/* Dialog procedure return values */
#define D_O_K        0x0000    /* OK: Nothing happened */
#define D_REDRAW     0x0001    /* Must redraw the entire dialog */
#define D_CLOSE      0x0002    /* Must close the dialog */
#define D_WANTFOCUS  0x0004    /* Yes, the control wants the input focus */

It is safe to ignore all messages (returning D_O_K), but you should at
least draw the control in MSG_DRAW messages (if you want to see
anything, of course).

Every control has some flags that say things about the state of the
control. The flags are:

/* Control flags */
#define D_EXIT       0x0100    /* Should return D_CLOSE when clicked */
#define D_GOTFOCUS   0x0200    /* Control has the input focus */
#define D_GOTMOUSE   0x0400    /* Mouse is over the control's rectangle */
#define D_SELECTED   0x0800    /* Control is selected */
#define D_AUTOW      0x1000    /* Control adjusts the width */
#define D_AUTOH      0x2000    /* Control adjusts the height */
#define D_DISABLED   0x4000    /* Control is disabled */
#define D_3D         0x8000    /* Control has 3D aspect */

It is up to the control procedure to respond to the flags, changing
its appearence or behaviour accordingly.


XGUI provides these procedures for standard controls:


  window_proc

The first control in every dialog MUST be of this type. The window
position and size will be read from this control. Also, this control
clears the background of the window to the color in `bg'.


  hline_proc
  rect_proc
  fillrect_proc

These controls are decorative figures. `hline_proc' is a horizontal
line, `rect_proc' is a rectangle and `fillrect_proc' is a filled
rectangle.


  button_proc

This control represents a button. If the flag D_EXIT is set (in the
field `flag' of the DIALOG structure), then the function will return
D_CLOSE when the button is pressed, indicating to the XGUI dialog
manager that the dialog must be closed. You can intercept this,
execute your code and return D_O_K (this keeps the dialog alive); this
is the standard way of having a button execute your code. The field
`dp' should point to the string containing the name of the button to
be displayed.


  text_proc       Static text

This control shows a text. The field `d1' controls the alignment of
the text: 0 for left-aligned, 1 for right align and 2 for centered
text. The field `dp' should point to the string to be displayed.


  edit_proc       Editable text
  password_proc   Editable password (edits, but doesn't show)

These controls are used for editing text. The field `d1' is changed
during editing, it stores the position of the insertion mark; the
field `d2' stores the maximum number of characters to be accepted; and
`dp' must point to the edit buffer (the string being edited). If the
flag D_EXIT is set, then the control procedure will return D_CLOSE to
indicate that the dialog should be closed.


  menu_proc

This control represents a menu. When it is not clicked, the menu just
shows its items. When it is clicked, it opens a window to show the
sub-items of the item that was clicked. The field `dp' must opint to
an array of MENU structures, as described below.

  typedef struct MENU {
    char *text;                 /* Item text */
    int (*proc)();              /* Called procedure when item is clicked */
    struct MENU *child;         /* Sub-items */
    ind id;                     /* Info passed to proc() */
  } MENU;

When a menu item is selected, the `proc' function is called with two
arguments: the WINDOW * that owns the menu and the `id' field in the
MENU structure. Is is safe to ignore the arguments and have the
function like

  int proc(void);

or

  int proc(WINDOW *win);

The `id' field is useful to have many items share the same function,
the function will know which item was selected by the `id'. This
function should return one of the D_xxx exit status defined
above. This value will be returned from the menu_proc() function. If
no item is selected, menu_proc() will D_O_K.


You can use these controls or create your own controls re-using these
pre-defined functions. In practice, most dialog use some standard and
some created controls.

Once you have set up a DIALOG array to build your dialog, all you have
to do is to call

  do_dialog_window(WINDOW *parent, DIALOG *dialog);

This function will create a new window for the dialog, execute the
dialog, close the window and then return the index of the DIALOG
structure in the `dialog' array that caused the termination of the
dialog. To have better control over the window, you might want to use
the standard create_window() to create the window with the dialog and
then call

  do_dialog(WINDOW *win, int control_index);

to execute the dialog in the window `win'. `control_index' is the
index of the control that will start with the input focus, you can
pass -1 to let the manager pick one. This function might be useful if
you want to use the dialog many times wigthout closing its window or
if you have two dialog windows.

If a dialog window is created with a parent window, it will be a
"popup" window. This means that some window managers would display it
without title (fvwm is one of them), prevent its parent from appearing
in the front of it and/or REALLY use its position (fvwm has an option
for doing "interactive" window placement, which means that the user
must select the position of the window regardless of what the program
says). On the other hand, some other window managers (olwm, for
example) will always display a title and use the specified position.
So, what you'll get depends on the window manager you are using.

Here are some functions you may use BEFORE creating the dialog window
to manage dialogs:


  set_dialog_flag(DIALOG *dlg, int flag, int set);

This function will set or remove the flag `flag' of all controls of
the dialog `dlg'. If `set' is zero, the flag will be removed,
otherwise it will be set.


  centre_dialog(DIALOG *dlg);

This function sets `dlg->x' and `dlg->y' so that the window will be
centered in the screen.


Here is a simple example of a dialog:

----------------------------------------------------------------------
#include <stdio.h>
#include <xgui.h>

char edit_text[32] = "Edit me!";

DIALOG dialog[] = {
  { window_proc, 0,  0,   200, 150, 0, 15, 0,      0, 0,  "Window Title" },
  { text_proc,   10, 10,  180, 20,  0, 15, 0,      0, 0,  "Displayed text" },
  { edit_proc,   10, 30,  180, 20,  0, 15, 0,      0, 31, edit_text },
  { button_proc, 70, 110, 60,  30,  0, 15, D_EXIT, 0, 0,  "Close" },
  { NULL }
};

int main(void)
{
  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }

  do_dialog_window(NULL, dialog);

  closegraph();
  return 0;
}
----------------------------------------------------------------------


======================================================================
THE FUNCTIONS
======================================================================

