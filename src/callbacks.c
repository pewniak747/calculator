#include <string.h>
#include "calclist.h"
#include "callbacks.h"

GtkWidget * get_widget(GtkBuilder * builder, const char * name) {
  return gtk_builder_get_object(GTK_BUILDER(builder), name);
}

void callback_quit(GtkWidget * widget, gpointer data) {
  gtk_main_quit();
}

void callback_calculate(GtkWidget * widget, gpointer cx) {
  int error=0;
  double result;
  struct calc_context * context = cx;
  char * input = gtk_entry_get_text(GTK_ENTRY(get_widget(context->builder, "query_edit")));
  if(strlen(input) == 0) return;
  char * output[20];
  rpn_resolve(&input, &result, &error);
  sprintf(output, "%.6f", result);
  if(error > 0)
    *output = "ERROR!";
  else {
    calclist_insert(input, result, context->list);
  }
  g_print("%s\n", output);
  gtk_label_set_text(GTK_LABEL(get_widget(context->builder, "result_label")), output);
}

void callback_insert(GtkWidget * widget, gpointer builder) {
  char * input = gtk_entry_get_text(GTK_ENTRY(get_widget(builder, "query_edit")));
  char * output[100];
  strcpy(output, input);
  strcat(output, gtk_button_get_label(widget));
  gtk_entry_set_text(GTK_ENTRY(get_widget(builder, "query_edit")), output);
}

void callback_previous(GtkWidget * widget, gpointer cx) {
  struct calc_context * context = cx;
  struct calclist ** list = context->list;
  calclist_prev(list);
  if(*list == NULL) return;
  gtk_entry_set_text(GTK_ENTRY(get_widget(context->builder, "query_edit")), (*list)->input);
  char * output[100];
  sprintf(output, "%.6f", (*list)->result);
  gtk_label_set_text(GTK_LABEL(get_widget(context->builder, "result_label")), output);
}

void callback_next(GtkWidget * widget, gpointer cx) {
  struct calc_context * context = cx;
  struct calclist ** list = context->list;
  calclist_next(list);
  if(*list == NULL) return;
  gtk_entry_set_text(GTK_ENTRY(get_widget(context->builder, "query_edit")), (*list)->input);
  char * output[100];
  sprintf(output, "%.6f", (*list)->result);
  gtk_label_set_text(GTK_LABEL(get_widget(context->builder, "result_label")), output);
}

void callback_clear(GtkWidget * widget, gpointer builder) {
  gtk_entry_set_text(GTK_ENTRY(get_widget(builder, "query_edit")), "");
}

