#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calclist.h"

struct calclist * calclist_new(char * input, double output) {
  struct calclist * new = malloc(sizeof(struct calclist));
  strcpy(new->input, input);
  new->result = output;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

calc_context * calc_context_new(GtkBuilder * builder, struct calclist ** list) {
  calc_context * context =  malloc(sizeof(calc_context));
  context->builder = builder;
  context->list = list;
  return context;
}


void calclist_insert(char * input, double output, struct calclist ** list) {
  struct calclist * new = calclist_new(input, output);
  if((*list) == NULL) {
    *list = new;
  }
  else {
    new->prev = *list;
    new->next = (*list)->next;
    (*list)->next = new;
    *list = new;
  }
}

void calclist_rewind(struct calclist ** list) {
  if(!(*list)) return;
  while((*list)->prev != NULL) {
    printf("rewinding from %f ", (*list)->result);
    *list = (*list)->prev;
    printf("to %f \n", (*list)->result);
  }
}

void calclist_prev(struct calclist ** list) {
  if((*list) == NULL || (*list)->prev == NULL) return;
  *list = (*list)->prev;
}

void calclist_next(struct calclist ** list) {
  if((*list) == NULL || (*list)->next == NULL) return;
  *list = (*list)->next;
}

void calclist_free(struct calclist ** list) {
  if(*list == NULL) return;
  struct calclist * tmp;
  calclist_rewind(list);
  printf("rewind to %f\n", (*list)->result);
  while(*list != NULL) {
    tmp = *list;
    printf("freeing %f\n", tmp->result);
    *list = (*list)->next;
    free(tmp);
  }
}
