#include <string.h>
#include "calc_context.h"
#include "calclist.h"
#include "callbacks.h"
#include "rpn.h"

GtkWidget * get_widget(GtkBuilder * builder, const char * name) {
  return gtk_builder_get_object(GTK_BUILDER(builder), name);
}

void callback_quit(GtkWidget * widget, gpointer data) {
  gtk_main_quit();
}

void callback_calculate(GtkWidget * widget, gpointer cx) {
  int error=0;
  double result;
  calc_context * context = cx;
  char * input = gtk_entry_get_text(GTK_ENTRY(get_widget(context->builder, "query_edit")));
  if(strlen(input) == 0) return;
  rpn_resolve(input, &result, &error);
  char *output[100];
  if(error == 0) {
    format_double(result, output);
    g_print("SUCCESS! result: %.6f, output: %s\n", result, output);
    calclist_insert(input, result, context->list);
  }
  else {
    g_print("ERROR! code: %d\n", error);
    strcpy(output, "ERROR!");
  }
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
  calc_context * context = cx;
  calclist ** list = context->list;
  calclist_prev(list);
  if(*list == NULL) return;
  gtk_entry_set_text(GTK_ENTRY(get_widget(context->builder, "query_edit")), (*list)->input);
  char * output[100];
  format_double((*list)->result, output);
  gtk_label_set_text(GTK_LABEL(get_widget(context->builder, "result_label")), output);
}

void callback_next(GtkWidget * widget, gpointer cx) {
  calc_context * context = cx;
  calclist ** list = context->list;
  calclist_next(list);
  if(*list == NULL) return;
  gtk_entry_set_text(GTK_ENTRY(get_widget(context->builder, "query_edit")), (*list)->input);
  char * output[100];
  format_double((*list)->result, output);
  gtk_label_set_text(GTK_LABEL(get_widget(context->builder, "result_label")), output);
}

void callback_control_buttons(GtkWidget * widget, gpointer cx) {
  calc_context * context = cx;
  calclist ** list = context->list;
  if((*list)->prev == NULL) gtk_widget_set_sensitive(GTK_WIDGET(get_widget(context->builder, "button_previous")), FALSE);
  else gtk_widget_set_sensitive(GTK_WIDGET(get_widget(context->builder, "button_previous")), TRUE);
  if((*list)->next == NULL) gtk_widget_set_sensitive(GTK_WIDGET(get_widget(context->builder, "button_next")), FALSE);
  else gtk_widget_set_sensitive(GTK_WIDGET(get_widget(context->builder, "button_next")), TRUE);
}

void callback_clear(GtkWidget * widget, gpointer builder) {
  gtk_entry_set_text(GTK_ENTRY(get_widget(builder, "query_edit")), "");
  gtk_label_set_text(GTK_LABEL(get_widget(builder, "result_label")), "0");
}

