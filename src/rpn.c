#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include "rpn.h"

int rpn_opcode(char op) {
  switch(op) {
    case '+': return 1;
    case '-': return 2;
    case '*': return 3;
    case '/': return 4;
  }
}

int rpn_precedence(int op) {
  switch(op) {
    case 1: return 1;
    case 2: return 1;
    case 3: return 2;
    case 4: return 2;
  }
}

void rpn_push(struct rpn_node ** rpn_stack, struct rpn_node * new_node) {
  new_node->next = *rpn_stack;
  *rpn_stack = new_node;
}

void rpn_pop(struct rpn_node ** rpn_stack, bool fr) {
  if(*rpn_stack != NULL) {
    struct rpn_node *stack_ptr = *rpn_stack;
    *rpn_stack = stack_ptr->next;
    if(fr)
      free(stack_ptr);
  }
}

struct rpn_node* rpn_create(int type, double value) {
  struct rpn_node *new_node = (struct rpn_node*)malloc(sizeof(struct rpn_node));
  new_node->type = type;
  new_node->value = value;
  return new_node;
}

void rpn_parse(char *input[], struct rpn_node * result[], int * result_size) {
  int i=0, input_size = strlen(*input), op = 0;
  *result_size = 0;
  double memory_value;
  bool memory = false;
  char token;
  struct rpn_node * op_stack = 0;
  for(i=0; i<input_size; i++) {
    token = (*input)[i];
    if(token == '+' || token == '-' || token == '*' || token == '/') {
      if(memory) {
        result[*result_size] = rpn_create(0, memory_value);
        *result_size += 1;
        memory = false;
      }
      op = rpn_opcode(token);
      if(!(op_stack == NULL || rpn_precedence(op_stack->type) < rpn_precedence(op))) {
        while(op_stack != NULL && rpn_precedence(op_stack->type) > rpn_precedence(op)) {
          result[*result_size] = op_stack;
          *result_size += 1;
          rpn_pop(&op_stack, false);
        }
      }
      rpn_push(&op_stack, rpn_create(op, 0));
    }
    else {
      if(memory) {
        memory_value *= 10;
        memory_value += (int)(token)-48;
      }
      else {
        memory_value = (int)(token)-48;
        memory = true;
      }
    }
  }
  if(memory) {
    result[*result_size] = rpn_create(0, memory_value);
    *result_size += 1;
    memory = false;
  }
  while(op_stack != NULL) {
    result[*result_size] = op_stack;
    *result_size += 1;
    rpn_pop(&op_stack, false);
  }
  /* debug
  printf("finishing! rpn size: %d\n", *result_size);
  for(i=0; i<*result_size; i++) {
    printf("> %.3f - %d\n", result[i]->value, result[i]->type);
  }
  printf("finished\n");
  */
}

void rpn_getargs(struct rpn_node ** rpn_stack, int size, double result[]) {
  while(size>0) {
    result[size-1] = (*rpn_stack)->value;
    rpn_pop(rpn_stack, true);
    size--;
  }
}

void rpn_addition(struct rpn_node ** rpn_stack) {
  double args[2];
  rpn_getargs(rpn_stack, 2, args);
  rpn_push(rpn_stack, rpn_create(0, args[0]+args[1]));
}

void rpn_substraction(struct rpn_node ** rpn_stack) {
  double args[2];
  rpn_getargs(rpn_stack, 2, args);
  rpn_push(rpn_stack, rpn_create(0, args[0]-args[1]));
}

void rpn_multiplication(struct rpn_node ** rpn_stack) {
  double args[2];
  rpn_getargs(rpn_stack, 2, args);
  rpn_push(rpn_stack, rpn_create(0, args[0]*args[1]));
}

void rpn_division(struct rpn_node ** rpn_stack) {
  double args[2];
  rpn_getargs(rpn_stack, 2, args);
  rpn_push(rpn_stack, rpn_create(0, args[0]/args[1]));
}

//double rpn_resolve(struct rpn_node *rpn_expression[], int size) {
void rpn_resolve(char * input[], double * result, int * error) {
  int rpn_size = 10;
  struct rpn_node *rpn_expression[rpn_size];
  rpn_parse(input, rpn_expression, &rpn_size);
  struct rpn_node *rpn_stack = 0;
  int i=0;
  for(i=0; i<rpn_size; i++) {
    if(rpn_expression[i]->type == 0) {
      rpn_push(&rpn_stack, rpn_expression[i]);
    }
    else {
      switch(rpn_expression[i]->type) {
        case 1 :
          rpn_addition(&rpn_stack);
          break;
        case 2 :
          rpn_substraction(&rpn_stack);
          break;
        case 3 :
          rpn_multiplication(&rpn_stack);
          break;
        case 4 :
          rpn_division(&rpn_stack);
          break;
      }
      free(rpn_expression[i]);
    }
  }
  *result = rpn_stack->value;
  while(rpn_stack != NULL) rpn_pop(&rpn_stack, true);
}

