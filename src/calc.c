#include<stdlib.h>
#include<string.h>
#include<gtk/gtk.h>
#include "rpn.h"

GtkWidget * get_widget(GtkBuilder * builder, const char * name) {
  return gtk_builder_get_object(GTK_BUILDER(builder), name);
}

static void quit(GtkWidget * widget, gpointer data) {
  gtk_main_quit();
}

static void calculate(GtkWidget * widget, gpointer builder) {
  int error=0;
  double result;
  char * input = gtk_entry_get_text(GTK_ENTRY(get_widget(builder, "query_edit")));
  if(strlen(input) == 0) return;
  char * output[20];
  rpn_resolve(&input, &result, &error);
  sprintf(output, "%.6f", result);
  if(error > 0)
    *output = "ERROR!";
  g_print("%s\n", output);
  gtk_label_set_text(GTK_LABEL(get_widget(builder, "result_label")), output);
}

static void insert_chars(GtkWidget * widget, gpointer builder) {
  char * input = gtk_entry_get_text(GTK_ENTRY(get_widget(builder, "query_edit")));
  char * output[100];
  strcpy(output, input);
  strcat(output, gtk_button_get_label(widget));
  gtk_entry_set_text(GTK_ENTRY(get_widget(builder, "query_edit")), output);
}

static void clear_input(GtkWidget * widget, gpointer builder) {
  gtk_entry_set_text(GTK_ENTRY(get_widget(builder, "query_edit")), "");
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

  window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

  // connect signals
  g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_equals")), "clicked", G_CALLBACK(calculate), builder);

  // number buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_0")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_1")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_2")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_3")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_4")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_5")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_6")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_7")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_8")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_9")), "clicked", G_CALLBACK(insert_chars), builder);

  // equation buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_add")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_subtract")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_multiply")), "clicked", G_CALLBACK(insert_chars), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_divide")), "clicked", G_CALLBACK(insert_chars), builder);

  // action buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_clear")), "clicked", G_CALLBACK(clear_input), builder);

  gtk_widget_show(window);

  gtk_main();
  return 0;
}
