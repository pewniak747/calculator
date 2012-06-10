#ifndef RPN_FUNCTIONS
#define RPN_FUNCTIONS

#include <stdbool.h>

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

void rpn_push(struct rpn_node ** rpn_stack, struct rpn_node * new_node);
void rpn_pop(struct rpn_node ** rpn_stack, bool fr);
struct rpn_node* rpn_create(int type, double value);

int rpn_opcode(char op);
bool rpn_isop(char op);
bool rpn_isnum(char *num);
double rpn_parsenum(char *num);
int rpn_precedence(int op);
bool rpn_left_assoc(int op);
void rpn_getargs(struct rpn_node ** rpn_stack, int size, double result[]);

void rpn_addition(struct rpn_node ** rpn_stack);
void rpn_substraction(struct rpn_node ** rpn_stack);
void rpn_multiplication(struct rpn_node ** rpn_stack);
void rpn_division(struct rpn_node ** rpn_stack);

void rpn_tokenize(char *input, char **output, int * size, int * error);
void rpn_parse(char **input, struct rpn_node * result[], int * result_size, int * error);
void rpn_resolve(char *input, double * result, int * error);

#endif // RPN_FUNCTIONS
