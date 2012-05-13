#include "calc_context.h"

calc_context * calc_context_new(GtkBuilder * builder, calclist ** list) {
  calc_context * context =  malloc(sizeof(calc_context));
  context->builder = builder;
  context->list = list;
  return context;
}
