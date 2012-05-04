#include<stdlib.h>
#include<gtk/gtk.h>
#include "rpn.h"

static void quit(GtkWidget * widget, gpointer data) {
  gtk_main_quit();
}

int main(int argc, char *argv[]) {
  GtkWidget *window;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);

  int error=0;
  double result;
  rpn_resolve(&argv[1], &result, &error);
  if(error == 0)
    g_print("%.6f\n", result);
  else
    g_print("ERROR!\n");

  gtk_widget_show(window);

  gtk_main();
  return 0;
}
