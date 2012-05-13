#ifndef CALCLIST
#define CALCLIST
#include <gtk/gtk.h>

struct calclist {
  struct calclist *prev;
  struct calclist *next;
  char input[100];
  double result;
};

struct calc_context {
  struct calclist ** list;
  GtkBuilder * builder;
};

struct calclist * calclist_new(char * input, double output);
struct calc_context * calc_context_new(GtkBuilder * builder, struct calclist ** list);
void calclist_insert(char * input, double output, struct calclist ** list);
void calclist_rewind(struct calclist ** list);
void calclist_prev(struct calclist ** list);
void calclist_next(struct calclist ** list);
void calclist_free(struct calclist ** list);

#endif // CALCLIST
