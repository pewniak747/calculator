#ifndef CALCLIST
#define CALCLIST
#include <stdbool.h>
#include <gtk/gtk.h>

typedef struct {
  struct calclist *prev;
  struct calclist *next;
  char input[100];
  double result;
} calclist;

calclist * calclist_new(char * input, double output);
void calclist_insert(char * input, double output, calclist ** list);
void calclist_rewind(calclist ** list);
bool calclist_prev(calclist ** list);
bool calclist_next(calclist ** list);
void calclist_free(calclist ** list);

void format_double(double result, char * output);

#endif // CALCLIST
