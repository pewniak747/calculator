#include <gtk/gtk.h>
#include "rpn.h"
#include "callbacks.h"
#include "calc_context.h"
#include "calclist.h"

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  GtkBuilder *builder;
  GtkWidget  *window;
  GError     *error = NULL;

  if(argc == 2 && strcmp(argv[1], "--nogui") == 0) {
    double res;
    int err;
    while(true) {
      printf("> ");
      char *input = malloc(100*sizeof(char));
      scanf("%s", input);
      if(strcmp(input, "exit") == 0) return 0;
      rpn_resolve(input, &res, &err);
      if(err == 0) printf("%.6f\n", input, res);
      else printf("ERROR!\n");
    }
  }

  builder = gtk_builder_new();
  if(!gtk_builder_add_from_file(builder, "src/calc.glade", &error)) {
      g_warning("%s", error->message);
      g_free(error);
      return(1);
  }
  calclist * list = NULL;
  calclist_fread(&list, "calculations.txt");

  calc_context * context = calc_context_new(builder, &list);
  window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

  // connect signals
  g_signal_connect(window, "destroy", G_CALLBACK(callback_quit), context);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_quit")), "clicked", G_CALLBACK(callback_quit), context);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_equals")), "clicked", G_CALLBACK(callback_calculate), context);

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

  // function buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_sine")), "clicked", G_CALLBACK(callback_insert_function), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_cosine")), "clicked", G_CALLBACK(callback_insert_function), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_tangens")), "clicked", G_CALLBACK(callback_insert_function), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_sqrt")), "clicked", G_CALLBACK(callback_insert_function), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_exp")), "clicked", G_CALLBACK(callback_insert_function), builder);

  // equation buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_add")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_subtract")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_multiply")), "clicked", G_CALLBACK(callback_insert), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_divide")), "clicked", G_CALLBACK(callback_insert), builder);

  // control buttons
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_clear")), "clicked", G_CALLBACK(callback_clear), builder);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_previous")), "clicked", G_CALLBACK(callback_previous), context);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_next")), "clicked", G_CALLBACK(callback_next), context);

  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_next")), "clicked", G_CALLBACK(callback_control_buttons), context);
  g_signal_connect(GTK_WIDGET(get_widget(builder, "button_previous")), "clicked", G_CALLBACK(callback_control_buttons), context);

  gtk_widget_show(window);

  if(list != NULL) {
    callback_control_buttons(NULL, context);
    callback_next(NULL, context);
  }

  gtk_main();
  return 0;
}
