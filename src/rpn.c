#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "rpn.h"

int rpn_opcode(char op) {
  int result;
  switch(op) {
    case '+': result = 1; break;
    case '-': result = 2; break;
    case '*': result = 3; break;
    case '/': result = 4; break;
    case '^': result = 5; break;
    default: result = 0; break;
  }
  return result;
}

bool rpn_isop(char token) {
  return token == '+' || token == '-' || token == '*' || token == '/' || token == '^';
}

int rpn_precedence(int op) {
  switch(op) {
    case 1: op = 1; break;
    case 2: op = 1; break;
    case 3: op = 2; break;
    case 4: op = 2; break;
    case 5: op = 3; break;
  }
  return op;
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

void rpn_parse(char *input, struct rpn_node *result[], int *result_size, int *error) {
  int i=0, input_size = strlen(input), op = 0;
  *result_size = 0;
  double memory_value;
  bool memory = false;
  int decimal = 0;
  char token;
  struct rpn_node * op_stack = 0;
  for(i=0; i<input_size; i++) {
    token = input[i];
    if(token == '.') {
      decimal = 1;
    }
    else if(rpn_isop(token)) {
      if(memory) {
        result[*result_size] = rpn_create(0, memory_value);
        *result_size += 1;
        memory = false;
        decimal = 0;
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
        if(decimal > 0) {
          memory_value += ((int)(token)-48)/pow(10, decimal);
          decimal = decimal+1;
        }
        else {
          memory_value *= 10;
          memory_value += (int)(token)-48;
        }
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

void rpn_exponentation(struct rpn_node ** rpn_stack) {
  double args[2];
  rpn_getargs(rpn_stack, 2, args);
  rpn_push(rpn_stack, rpn_create(0, pow(args[0], args[1])));
}

void rpn_tokenize(char *input, char *output[], int *size, int *error) {
  char *buffer = malloc(100*sizeof(char));
  char *nbuffer = malloc(100*sizeof(char));
  char current;
  int i;
  *size = 0;
  for(i=0; i<strlen(input); i++) {
    current = input[i];
    printf("> %c\n", current);
    if((current-48 >= 0 && current-48 <=9) || current == 46) {
      strncat(nbuffer, &current, 1);
    }
    else if(rpn_isop(current) || current == '(' || current == ')') {
      if(strlen(nbuffer) > 0) {
        output[*size] = malloc(30*sizeof(char));
        strcpy(output[*size], nbuffer);
        strcpy(nbuffer, "");
        *size = *size + 1;
      }
      output[*size] = malloc(30*sizeof(char));
      strncat(output[*size], &current, 1);
      *size = *size + 1;
    }
  }
  if(strlen(nbuffer) > 0) {
    output[*size] = malloc(30*sizeof(char));
    strcpy(output[*size], nbuffer);
    strcpy(nbuffer, "");
    *size = *size + 1;
  }
}

void rpn_resolve(char *input, double *result, int *error) {
  if(strlen(input) == 0) {
    *error = 1;
    return;
  }
  int rpn_size = 100;
  /*
  char *tokenized_input[1000];
  rpn_tokenize(input, tokenized_input, &rpn_size, error);
  printf("\nexpression: %s\ntokens: ", input);
  int i;
  for(i = 0; i<rpn_size; i++) printf("%s ", tokenized_input[i]);
  printf("\n\n");
  */
  struct rpn_node *rpn_expression[rpn_size];
  rpn_parse(input, rpn_expression, &rpn_size, error);
  if(*error > 0) return;
  struct rpn_node *rpn_stack = 0;
  double (*functions[6])(struct rpn_node ** rpn_stack) = {
    NULL,
    rpn_addition,
    rpn_substraction,
    rpn_multiplication,
    rpn_division,
    rpn_exponentation
  };
  int i=0;
  for(i=0; i<rpn_size; i++) {
    if(rpn_expression[i]->type == 0) {
      rpn_push(&rpn_stack, rpn_expression[i]);
    }
    else {
      functions[rpn_expression[i]->type](&rpn_stack);
      free(rpn_expression[i]);
    }
  }
  *result = rpn_stack->value;
  while(rpn_stack != NULL) rpn_pop(&rpn_stack, true);
}

