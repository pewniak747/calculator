#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calc_context.h"
#include "calclist.h"

calclist * calclist_new(char * input, double output) {
  calclist * new = malloc(sizeof(calclist));
  strcpy(new->input, input);
  new->result = output;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

char * format_double(double result) {
  char * output[100];
  if(fmod(result, 1) < 0.000001) {
    sprintf(output, "%.0f", result);
  }
  else {
    sprintf(output, "%.6f", result);
  }
  return output;
}

void calclist_insert(char * input, double output, calclist ** list) {
  calclist * new = calclist_new(input, output);
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

void calclist_rewind(calclist ** list) {
  if(!(*list)) return;
  while((*list)->prev != NULL) {
    printf("rewinding from %f ", (*list)->result);
    *list = (*list)->prev;
    printf("to %f \n", (*list)->result);
  }
}

void calclist_prev(calclist ** list) {
  if((*list) == NULL || (*list)->prev == NULL) return;
  *list = (*list)->prev;
}

void calclist_next(calclist ** list) {
  if((*list) == NULL || (*list)->next == NULL) return;
  *list = (*list)->next;
}

void calclist_free(calclist ** list) {
  if(*list == NULL) return;
  calclist * tmp;
  calclist_rewind(list);
  printf("rewind to %f\n", (*list)->result);
  while(*list != NULL) {
    tmp = *list;
    printf("freeing %f\n", tmp->result);
    *list = (*list)->next;
    free(tmp);
  }
}
