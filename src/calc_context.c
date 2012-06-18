/*
 * calculator context keeps pointers to builder
 * (UI widgets) and list of previous calculations
 */
#include "calclist.h"
#include "calc_context.h"

/*
 * allocate new and return pointer to new calc context
 * context attributes: GtkBuilder and calclist
 */
calc_context * calc_context_new(GtkBuilder * builder, calclist ** list) {
  calc_context * context =  malloc(sizeof(calc_context));
  context->builder = builder;
  context->list = list;
  return context;
}

/*
 * freen calc context
 */
void calc_context_free(calc_context * context) {
  calclist_free(context->list);
  free(context);
}
