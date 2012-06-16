#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calc_context.h"
#include "calclist.h"
#include "rpn.h"

calclist * calclist_new(char * input, double output) {
  calclist * new = malloc(sizeof(calclist));
  strcpy(new->input, input);
  new->result = output;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

void format_double(double result, char * output) {
  if(fmod(result, 1) < 0.000001) {
    sprintf(output, "%.0f", result);
  }
  else {
    sprintf(output, "%.6f", result);
  }
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

bool calclist_prev(calclist ** list) {
  if((*list) == NULL || (*list)->prev == NULL) return false;
  *list = (*list)->prev;
  return true;
}

bool calclist_next(calclist ** list) {
  if((*list) == NULL || (*list)->next == NULL) return false;
  *list = (*list)->next;
  return true;
}

void calclist_parseline(char ** input, double *result) {
  int i=0;
  for(i; i<strlen(*input); i++) {
    if((*input)[i] == '=') {
      (*input)[i] = (char)0;
      *result = rpn_parsenum((*input)+i+1);
      return;
    }
  }
}

void calclist_fwrite(calclist ** ilist, char *filename) {
  FILE *file = fopen(filename, "w");
  if(ilist == NULL || file == NULL) return;
  calclist * list = *(ilist);
  calclist_rewind(&list);
  do {
    fprintf(file, "%s=%.6f\n", list->input, list->result);
  } while(calclist_next(&list));
  fclose(file);
}

void calclist_fread(calclist ** ilist, char *filename) {
  calclist_free(ilist);
  FILE *file = fopen(filename, "r");
  if(file == NULL) return;
  double result;
  char *input = malloc(100*sizeof(char));
  while(fscanf(file, "%s", input) != EOF) {
    calclist_parseline(&input, &result);
    calclist_insert(input, result, ilist);
  }
  fclose(file);
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
