
#include <stdio.h>
#include <xdialogs.h>

int main(void)
{
  WINDOW *w1, *w2;

  if (init_graph(NULL)) {
    fprintf(stderr, "Can't open display\n");
    exit(1);
  }
  w1 = create_window("First", 0, 0, 300, 100, NULL, NULL);
  w2 = create_window("Second", 0, 100, 300, 100, NULL, NULL);
  win_createimage(w1);
  win_createimage(w2);
  win_showwindow(w1);
  win_showwindow(w2);

  win_outtextxy(w1, 10, 10, "This is the first window");
  win_outtextxy(w1, 10, 30, "Press any key in this window");
  win_outtextxy(w2, 10, 10, "This is the second window");
  win_outtextxy(w2, 10, 30, "Press any key in this window");

  /* Wait until the user has pressed a key in the both windows: */
  while (! (win_has_key(w1) && win_has_key(w2)))
    ;

  closegraph();
  return 0;
}
