#ifndef CALLBACKS
#define CALLBACKS
#include <gtk/gtk.h>

GtkWidget * get_widget(GtkBuilder * builder, const char * name);
void callback_quit(GtkWidget * widget, gpointer data);
void callback_calculate(GtkWidget * widget, gpointer builder);
void callback_insert(GtkWidget * widget, gpointer builder);
void callback_clear(GtkWidget * widget, gpointer builder);

#endif // CALLBACKS
