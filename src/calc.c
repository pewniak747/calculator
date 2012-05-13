#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "rpn.h"

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  GtkBuilder *builder;
  GtkWidget  *window;
  GError     *error = NULL;

  builder = gtk_builder_new();
  if(!gtk_builder_add_from_file(builder, "src/calc.glade", &error)) {
      g_warning("%s", error->message);
      g_free(error);
      return(1);
  }

  window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

  // connect signals
  g_signal_connect(window, "destroy", G_CALLBACK(callback_quit), NULL);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_equals")), "clicked", G_CALLBACK(callback_calculate), builder);

  // number buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_0")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_1")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_2")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_3")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_4")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_5")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_6")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_7")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_8")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "number_9")), "clicked", G_CALLBACK(callback_insert), builder);

  // equation buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_add")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_subtract")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_multiply")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_divide")), "clicked", G_CALLBACK(callback_insert), builder);

  // action buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_clear")), "clicked", G_CALLBACK(callback_clear), builder);

  gtk_widget_show(window);

  gtk_main();
  return 0;
}
