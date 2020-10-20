#include <stdio.h>
#include <string.h>
#include "xdialogs.h"

#define USE_3D   1     /* 0 if you don't want "3d effects" */

int changetxt_proc(int, WINDOW *, DIALOG *, int);
int showpopup_proc(int, WINDOW *, DIALOG *, int);
int do_show_popup(WINDOW *, int);
int do_change_text(WINDOW *, int);
int do_exit(WINDOW *, int);
char *list_getter(int, int *);

char txt_edit[32] = "Edit Me!";
char *txt_show_list[2] = {
  "This is a text",
  "This is another text",
};
char txt_show[32];

MENU menu_items[] = {
  { "Change text", do_change_text, NULL, 0 },
  { "Show Popup", do_show_popup, NULL, 0 },
  { "", NULL, NULL, 0 },
  { "It's me again >", NULL, menu_items, 0 },
  { "", NULL, NULL, 0 },      /* Separator */
  { "Exit", do_exit, NULL, 0 },
  { NULL }
};

MENU menu[] = {
  { "Menu 1", NULL, menu_items, 0 },
  { "Menu 2", NULL, menu_items, 0 },
  { NULL }
};

/* The main dialog controls: */
DIALOG dlg_main[] = {
  { CTL_WINDOW, window_proc,    0,   0,   330, 360, 0, 15, 0,
    0, 0, "Control test" },
  { CTL_MENU,   menu_proc,      0,   0,   330, 25,  0, 15, D_AUTOW|D_AUTOH,
    0, 0, menu },
  { CTL_TEXT,   changetxt_proc, 10,  40,  150, 30,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Change text" },
  { CTL_BUTTON, showpopup_proc, 170, 40,  150, 30,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Show Popup" },
  { CTL_TEXT,   text_proc,      10,  80,  200, 30,  0, 15, 0,
    0, 0, txt_show },
  { CTL_EDIT,   edit_proc,      10,  120, 310, 30,  0, 15, D_AUTOH,
    0, 31, txt_edit },
  { CTL_LIST,   list_proc,      10,  160, 310, 95,  0, 15, D_AUTOH,
    0, 0, list_getter },
  { CTL_BUTTON, button_proc,    130, 270, 60,  30,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Exit" },
  { CTL_BUTTON, check_proc,     10,  310, 200, 30,  0, 15, 0,
    0, 0, "Check me!" },
  { CTL_NONE, NULL }
};
#define TEXT_CONTROL  4    /* Index of text control in the main dialog */
#define LIST_CONTROL  6    /* Index of list control in the main dialog */

/* The popup window controls: */
DIALOG dlg_popup[] = {
  { CTL_WINDOW, window_proc,  0,   0,   400, 170, 0, 15, 0,
    0, 0, "Popup" },
  { CTL_TEXT,   text_proc,    10,  10,  380, 30,  0, 15, 0,
    0, 0, "Edited text:" },
  { CTL_TEXT,   text_proc,    10,  40,  380, 30,  9, 15, 0,
    0, 0, txt_edit },
  { CTL_TEXT,   text_proc,    10,  70,  380, 30,  0, 15, 0,
    0, 0, "List selection:" },
  { CTL_TEXT,   text_proc,    10,  100, 380, 30,  9, 15, 0,
    0, 0, NULL },
  { CTL_BUTTON, button_proc,  170, 130, 60,  30,  0, 15, D_EXIT,
    0, 0, "Close" },
  { CTL_NONE,   NULL },
};
#define LIST_SELECTION  4


/* This function provides the list items */
char *list_getter(int index, int *nitens)
{
  static char *lst[] = {
    "One",   "Two",
    "Three", "Four",
    "Five",  "Six",
    "Seven", "Eight",
    "Nine",  "Ten"
  };

  if (index < 0) {    /* Requested # of items */
    *nitens = sizeof(lst) / sizeof(lst[0]);
    return NULL;
  }
  return lst[index];
}

/* This function returns D_CLOSE so that the gui manager closes the dialog */
int do_exit(WINDOW *win, int item)
{
  return D_CLOSE;
}

/* This function changes the text of the text
 * control in the main dialog window
 */
int do_change_text(WINDOW *w, int item)
{
  dlg_main[TEXT_CONTROL].d2 = ! dlg_main[TEXT_CONTROL].d2;
  strcpy(txt_show, txt_show_list[dlg_main[TEXT_CONTROL].d2]);
  SEND_MESSAGE(MSG_DRAW, &dlg_main[TEXT_CONTROL], 0);
  return D_O_K;  
}

/* This function shows the popup window */
int do_show_popup(WINDOW *w, int item)
{
  typedef char *(*get_index)(int, int *);

#if 0
  dlg_popup[LIST_SELECTION].dp =
    ((get_index)dlg_main[LIST_CONTROL].dp)(dlg_main[LIST_CONTROL].d1, NULL);
  centre_dialog(dlg_popup);
  do_dialog_window(w->parent, dlg_popup);
#else
  {
    char txt[256] = ".";
    file_select(w->parent, "Select file", txt, "*");
  }
#endif

  return D_O_K;
}

/* This is a button. Note the D_EXIT flag: when the button is pressed, the
 * normal button_proc would return D_CLOSE to close the dialog, we intercept
 * this, execute our action and return D_O_K to continue the execution.
 */
int changetxt_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  int ret;

  ret = button_proc(msg, w, dlg, c);

  if (ret == D_CLOSE)
    return do_change_text(w, 0);
  return ret;
}

/* This button shows the help window when it is clicked */
int showpopup_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  int ret;

  ret = button_proc(msg, w, dlg, c);

  if (ret == D_CLOSE)
    return do_show_popup(w, 0);
  return ret;
}

int main(void)
{
  strcpy(xd_font_name,
	 "-adobe-helvetica-bold-o-normal--18-180-75-75-p-104-iso8859-1"
         /* "7x14bold" */
	 );

  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }

  xd_stddlg_3d = USE_3D;
  strcpy(txt_show, txt_show_list[0]);
  set_dialog_flag(dlg_main, D_3D, USE_3D);
  set_dialog_flag(dlg_popup, D_3D, USE_3D);
  centre_dialog(dlg_main);
  do_dialog_window(NULL, dlg_main, 1);

  closegraph();
  return 0;
}
