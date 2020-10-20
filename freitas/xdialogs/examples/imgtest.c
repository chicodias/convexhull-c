#include <stdio.h>
#include <xdialogs.h>

int draw_proc(int, WINDOW *, DIALOG *, int);
int btclear_proc(int, WINDOW *, DIALOG *, int);
int bthelp_proc(int, WINDOW *, DIALOG *, int);

DIALOG dlg_draw[] = {
  { CTL_WINDOW,   window_proc,  0,   0,   500, 410, 0, 15, 0,
    0, 0, "Drawing Test" },
  { CTL_USER,     draw_proc,    10,  10,  480, 350, 0, 15, 0,
    0, 0, NULL },
  { CTL_BUTTON,   button_proc,  10,  370, 60,  20,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Exit" },
  { CTL_BUTTON,   btclear_proc, 80,  370, 60,  20,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Clear" },
  { CTL_BUTTON,   bthelp_proc,  150, 370, 60,  20,  0, 15, D_AUTOH|D_EXIT,
    0, 0, "Help" },
  { CTL_NONE, NULL }
};

DIALOG dlg_help[] = {
  { CTL_WINDOW, window_proc,  0,   0,   400, 120, 0,   15, 0,
    0, 0, "Popup" },
  { CTL_TEXT,   text_proc,    10,  10,  380, 30,  12,  15, 0,
    0, 0, "Drag the left button to draw in red." },
  { CTL_TEXT,   text_proc,    10,  40,  380, 30,  9,   15, 0,
    0, 0, "Drag the right button to draw in blue." },
  { CTL_BUTTON, button_proc,  170, 80,  60,  20,  0,   15, D_AUTOH|D_EXIT,
    0, 0, "Close" },
  { CTL_NONE, NULL },
};


/* This control is based on the image_proc() control. It is used to enable
 * the user to draw lines in the image with the mouse.
 */
int draw_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  if (msg == MSG_CLICK) {
    win_read_mouse(w, NULL, NULL);

    /* Set the drawing color according to the pressed button */
    win_setcolor(w, (w->mouse_b & MOUSE_1) ? 12 : 9);

    /* Draw the initial pixel in the window */
    win_putpixel(w, w->mouse_x, w->mouse_y, win_getcolor(w));
    win_moveto(w, w->mouse_x, w->mouse_y);

    win_grab_pointer(w, 0, None);
    while (w->mouse_b & (MOUSE_1 | MOUSE_3)) {
      /* Draw the line to the memory */
      win_lineto(w, w->mouse_x, w->mouse_y);
      win_moveto(w, w->mouse_x, w->mouse_y);
      win_read_mouse(w, NULL, NULL);
    }
    win_ungrab_pointer(w);
    return D_O_K;
  }
  return image_proc(msg, w, dlg, c);
}

/* This is a button. Note the D_EXIT flag: when the button is pressed, the
 * normal button_proc would return D_CLOSE to close the dialog, we intercept
 * this, execute our action and return D_O_K to contoinue the execution.
 */
int btclear_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  int ret;

  ret = button_proc(msg, w, dlg, c);

  if (ret == D_CLOSE) {
    win_setcolor(dlg_draw[1].win, dlg_draw[1].bg);
    win_rectfill(dlg_draw[1].win, 0, 0, dlg_draw[1].w - 1, dlg_draw[1].h - 1);
    SEND_MESSAGE(MSG_DRAW, &dlg_draw[1], 0);
    return D_O_K;
  }
  return ret;
}

/* This button shows the help window when it is clicked */
int bthelp_proc(int msg, WINDOW *w, DIALOG *dlg, int c)
{
  int ret;

  ret = button_proc(msg, w, dlg, c);

  if (ret == D_CLOSE) {
    centre_dialog(dlg_help);
    do_dialog_window(w->parent, dlg_help, 1);
    return D_O_K;
  }
  return ret;
}

int main(void)
{
  strcpy(xd_font_name, "7x14");

  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }

  set_dialog_flag(dlg_help, D_3D, 1);
  set_dialog_flag(dlg_draw, D_3D, 1);
  do_dialog_window(NULL, dlg_draw, 1);

  closegraph();
  return 0;
}
