/*
 * two-way list for keeping two values:
 * expression string and evaluated result
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calc_context.h"
#include "calclist.h"
#include "rpn.h"

/*
 * allocate and return new calclist
 */

calclist * calclist_new(char * input, double output) {
  calclist * new = malloc(sizeof(calclist));
  strcpy(new->input, input);
  new->result = output;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

/*
 * transform double to string with precision of 6
 * omitting precision when number is integer
 * parameters: number, string
 */
void format_double(double result, char * output) {
  if(fmod(result, 1) < 0.000001) {
    sprintf(output, "%.0f", result);
  }
  else {
    sprintf(output, "%.6f", result);
  }
}

/*
 * inserts new element in calclist
 * parameters: input, result, pointer to list pointer
 */
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

/*
 * rewinds calclist pointer to the beginning of the list
 * parameters: pointer to list pointer
 */
void calclist_rewind(calclist ** list) {
  if(!(*list)) return;
  while((*list)->prev != NULL) {
    printf("rewinding from %f ", (*list)->result);
    *list = (*list)->prev;
    printf("to %f \n", (*list)->result);
  }
}

/*
 * rewinds calclist pointer to previous element
 * parameters: pointer to list pointer
 */
bool calclist_prev(calclist ** list) {
  if((*list) == NULL || (*list)->prev == NULL) return false;
  *list = (*list)->prev;
  return true;
}

/*
 * rewinds calclist pointer to next element
 * parameters: pointer to list pointer
 */
bool calclist_next(calclist ** list) {
  if((*list) == NULL || (*list)->next == NULL) return false;
  *list = (*list)->next;
  return true;
}

/*
 * parses one line of input in format expression=result
 * parameters: pointer to string, pointer to result
 */
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

/*
 * writes contents of calclist to file
 * parameters: pointer to pointer to list, string
 */
void calclist_fwrite(calclist ** ilist, char *filename) {
  FILE *file = fopen(filename, "w");
  if(*ilist == NULL || file == NULL) return;
  calclist * list = *(ilist);
  calclist_rewind(&list);
  do {
    fprintf(file, "%s=%.6f\n", list->input, list->result);
  } while(calclist_next(&list));
  fclose(file);
}

/*
 * loads content of file to calclist
 * parameters: pointer to pointer to list, string
 */
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

/*
 * free calclist
 * parameters: ponter to pointer to list
 */
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
