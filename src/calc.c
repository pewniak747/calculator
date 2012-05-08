#include<stdlib.h>
#include<gtk/gtk.h>
#include "rpn.h"

static void quit(GtkWidget * widget, gpointer data) {
  gtk_main_quit();
}

static void calculate(GtkWidget * widget, gpointer builder) {
  int error=0;
  double result;
  char * input = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "query_edit")));
  char * output[20];
  rpn_resolve(&input, &result, &error);
  sprintf(output, "%.6f", result);
  if(error > 0)
    *output = "ERROR!";
  g_print("%s\n", output);
  gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "result_label")), output);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  GtkBuilder *builder;
  GtkWidget  *window;
  GError     *error = NULL;


  builder = gtk_builder_new();
  if( ! gtk_builder_add_from_file( builder, "src/calc.glade", &error ) )
  {
      g_warning( "%s", error->message );
      g_free( error );
      return( 1 );
  }

  /* Get main window pointer from UI */
  window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );


  g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(GTK_WIDGET( gtk_builder_get_object(builder, "button_equals")), "clicked", G_CALLBACK(calculate), builder);
  /*

  gtk_widget_show(query_input);
  gtk_widget_show(result_input);
  gtk_widget_show(calculate_button);
  gtk_widget_show(main_layout);
  */
  gtk_widget_show(window);

  gtk_main();
  return 0;
}
