#ifndef CALC_CONTEXT
#define CALC_CONTEXT

#include <gtk/gtk.h>
#include "calclist.h"

typedef struct {
  calclist ** list;
  GtkBuilder * builder;
} calc_context;

calc_context * calc_context_new(GtkBuilder * builder, calclist ** list);

#endif // CALC_CONTEXT
