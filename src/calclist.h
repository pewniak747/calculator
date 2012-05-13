#ifndef CALCLIST
#define CALCLIST

struct calclist {
  struct calclist *prev;
  struct calclist *next;
  char * input;
  double result;
};

struct calclist * calclist_new(char * input, double output);
void calclist_insert(char * input, double output, struct calclist ** list);
void calclist_rewind(struct calclist ** list);
void calclist_free(struct calclist ** list);

#endif // CALCLIST
