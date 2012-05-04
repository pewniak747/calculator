#include<stdlib.h>
#include<stdio.h>
#include "rpn.h"

int main(int argc, char *argv[]) {
  int error=0;
  double result;
  rpn_resolve(&argv[1], &result, &error);
  printf("%.6f\n", result);
}
