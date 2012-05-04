#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

/* function types
 * 0 - value
 * 1 - addition
 * 2 - substraction
 * 3 - multiplication
 * 4 - division
 */

struct rpn_node {
  int type;
  double value;
  struct rpn_node *next;
} rpn_node;

int rpn_opcode(char op) {
  switch(op) {
    case '+': return 1;
    case '-': return 2;
    case '*': return 3;
    case '/': return 4;
  }
}

void rpn_push(struct rpn_node ** rpn_stack, struct rpn_node * new_node) {
  new_node->next = *rpn_stack;
  *rpn_stack = new_node;
}

void rpn_pop(struct rpn_node ** rpn_stack) {
  if(*rpn_stack != NULL) {
    struct rpn_node *stack_ptr = *rpn_stack;
    *rpn_stack = stack_ptr->next;
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
  for(i=0; i<input_size; i++) {
    token = (*input)[i];
    //printf("%d\n", (int)(token)-48);
    if(token == '+' || token == '-' || token == '*' || token == '/') {
      //printf("operator!\n");
      if(memory) {
        result[*result_size] = rpn_create(0, memory_value);
        *result_size += 1;
        memory = false;
      }
      if(op > 0) {
        result[*result_size] = rpn_create(op, 0);
        *result_size += 1;
      }
      op = rpn_opcode(token);
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

      if(*result_size == 0 || memory) {
      }
    }
  }
  if(memory) {
    result[*result_size] = rpn_create(0, memory_value);
    *result_size += 1;
    memory = false;
  }
  result[*result_size] = rpn_create(op, 0);
  *result_size += 1;
  
  for(i=0; i<*result_size; i++) {
    //printf("%.6f\n", result[i]->value);
  }
}

void rpn_getargs(struct rpn_node ** rpn_stack, int size, double result[]) {
  while(size>0) {
    result[size-1] = (*rpn_stack)->value;
    rpn_pop(rpn_stack);
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

double rpn_resolve(struct rpn_node *rpn_expression[], int size) {
  struct rpn_node *rpn_stack = 0;
  int i=0;
  for(i=0; i<size; i++) {
    switch(rpn_expression[i]->type) {
      case 0 :
        rpn_push(&rpn_stack, rpn_expression[i]);
        break;
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
  }
  return rpn_stack->value;
}

int main(int argc, char *argv[]) {
  int rpn_size=10;
  struct rpn_node *rpn_expression[rpn_size];
  rpn_parse(&argv[1], rpn_expression, &rpn_size);
  double result = rpn_resolve(rpn_expression, rpn_size);
  printf("%.6f\n", result);
}
