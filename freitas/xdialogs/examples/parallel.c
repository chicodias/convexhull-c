#include <stdio.h>
#include <xdialogs.h>

char edit_1[32], edit_2[32];

int about_proc(int, WINDOW *, DIALOG *, int);

DIALOG dialog1[] = {
  { window_proc, 0,   0,   320, 200, 0, 15, 0,       0, 0,  "First window" },
  { text_proc,   10,  10,  300, 30,  0, 15, 0,       0, 0,  "Edit this:" },
  { edit_proc,   10,  50,  300, 30,  0, 15, D_AUTOH, 0, 31, edit_1 },
  { about_proc,  10,  160, 100, 30,  0, 15, D_EXIT,  0, 0,  "About" },
  { button_proc, 120, 160, 100, 30,  0, 15, D_EXIT,  0, 0,  "Close" },
  { NULL }
};

DIALOG dialog2[] = {
  { window_proc, 330, 0,   320, 200, 0, 15, 0,       0, 0,  "Second window" },
  { text_proc,   10,  10,  300, 30,  0, 15, 0,       0, 0,  "Now, this:" },
  { edit_proc,   10,  50,  300, 30,  0, 15, D_AUTOH, 0, 31, edit_2 },
  { about_proc,  10,  160, 100, 30,  0, 15, D_EXIT,  0, 0,  "About" },
  { button_proc, 120, 160, 100, 30,  0, 15, D_EXIT,  0, 0,  "Close" },
  { NULL }
};

DIALOG about_dlg[] = {
  { window_proc,   0,  0,  200, 100, 0, 15, 0, 0, 0, "About" },
  { text_proc,     10, 10, 180, 30,  0, 15, 0, 0, 0, "By Ricardo R. Massaro" },
  { button_proc,   10, 50, 100, 30,  0, 15, D_EXIT, 0, 0, "OK" },
  { NULL },
};

int about_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  int ret;

  ret = button_proc(msg, w, dlg, c);

  if (ret & D_CLOSE) {
    centre_dialog(about_dlg);
    do_dialog_window(w->parent, about_dlg, 0);
    return D_O_K;
  }
  return ret;
}

int main(void)
{
  WINDOW *w1, *w2;
  int running;

  if (init_graph(NULL)) {
    fprintf(stderr, "Impossivel abrir o display\n");
    exit(1);
  }

  w1 = create_window(NULL, 0, 0, 0, 0, NULL, dialog1);
  w2 = create_window(NULL, 0, 0, 0, 0, NULL, dialog2);

  init_dialog(w1, 0, -1);
  init_dialog(w2, 0, -1);

  do {
    running = 2;
    if (! do_dialog_step(w1)) {
      end_dialog(w1);
      /* In a normal program, we could just close the window now.
       * Here we won't do that, because we still want to use the `w1'
       * pointer to make things easier (i.e., to save a variable). Surely
       * it's not the better way of doing this, but it's just an example!
       */
      running--;
    }
    if (! do_dialog_step(w2)) {
      end_dialog(w2);
      win_hidewindow(w2);
      running--;
    }
  } while (running);

  return 0;
}
