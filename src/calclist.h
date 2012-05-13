#ifndef CALCLIST
#define CALCLIST
#include <gtk/gtk.h>

typedef struct {
  struct calclist *prev;
  struct calclist *next;
  char input[100];
  double result;
} calclist;

typedef struct {
  calclist ** list;
  GtkBuilder * builder;
} calc_context;

calclist * calclist_new(char * input, double output);
calc_context * calc_context_new(GtkBuilder * builder, calclist ** list);
void calclist_insert(char * input, double output, calclist ** list);
void calclist_rewind(calclist ** list);
void calclist_prev(calclist ** list);
void calclist_next(calclist ** list);
void calclist_free(calclist ** list);

#endif // CALCLIST
