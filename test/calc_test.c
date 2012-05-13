#include "../src/rpn.h"
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define EPS 0.000001
#define CU_ASSERT_EQ(i1, i2) CU_ASSERT(abs(i1 - i2) < EPS)

double result;
int error;
char * input[100];

void testSanity(void) {
  int i1 = 2, i2 = 1;
  CU_ASSERT_EQ(i1 + i2, 3);
}

void testSingular(void) {
  *input = "2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2);
}

void testAddition(void) {
  *input = "2+2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 4);

  *input = "1+2+3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 6);
}

void testSubtraction(void) {
  *input = "10-2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 8);

  *input = "4-3-9";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -8);
}

void testMultiplication(void) {
  *input = "1*2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2);

  *input = "4*3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 12);

  *input = "0*3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 0);

  *input = "2*3*4";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 24);
}

void testDivision(void) {
  *input = "1/2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 1/2);

  *input = "4/3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 4/3);

  *input = "0/3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 0);

  *input = "2/3/4";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2/12);
}

void testExponentation(void) {
  *input = "1^2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 1);

  *input = "4^2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 16);

  *input = "3^0";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 1);

  *input = "2^3^4";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 262144);
  
  *input = "4^(1/2)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2);
}
void testDecimalPoint(void) {
  *input = "2.0";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2);

  *input = "2.12345";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2.12345);

  *input = "2.5+3.3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 5.8);

  *input = "2.5-3.3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 0.8);

  *input = "2.5*3.3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 8.25);

  *input = "2.5/3.3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 0.757575758);
}

void testPrecedence(void) {
  *input = "2+3*4";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 14);

  *input = "2+3*4-1";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 13);

  *input = "2+3*4/2-1";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 7);

  *input = "2*4+1+3*4/2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 15);

  *input = "2*4^2/8";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 4);
}

void testBrackets(void) {
  *input = "(2)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 2);

  *input = "(2+3)*4";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 20);

  *input = "(2+3)*(4-1)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 15);

  *input = "(2+(1+3)/2)*(4-1)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, 12);
}

void testUnaryMinus(void) {
  *input = "-8";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -8);
  
  *input = "(-8)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -8);
  
  *input = "-2+3";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -1);
  
  *input = "7*-2";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -14);
  
  *input = "-(2+2)";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -4);
  
  *input = "-(2+2*3)-1";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(result, -13);
}

void testErrors(void) {
  *input = "3/0";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(error, 1);
  
  *input = "2/3/0";
  rpn_resolve(&input, &result, &error);
  CU_ASSERT_EQ(error, 1);
}

int main() {
  CU_pSuite pSuite = NULL;
  CU_initialize_registry();

  // add a suite
  pSuite = CU_add_suite("RPN resolve test", NULL, NULL);

  // add tests
  CU_add_test(pSuite, "test of sanity", testSanity);
  CU_add_test(pSuite, "test of singular number", testSingular);
  CU_add_test(pSuite, "test of addition", testAddition);
  CU_add_test(pSuite, "test of subtraction", testSubtraction);
  CU_add_test(pSuite, "test of multiplication", testMultiplication);
  CU_add_test(pSuite, "test of division", testDivision);
  CU_add_test(pSuite, "test of exponentation", testExponentation);
  CU_add_test(pSuite, "test of decimal point", testDecimalPoint);
  CU_add_test(pSuite, "test of operator precedence", testPrecedence);
  CU_add_test(pSuite, "test of brackets", testBrackets);
  //CU_add_test(pSuite, "test of unary minus", testUnaryMinus);
  CU_add_test(pSuite, "test of errors", testErrors);

  // run tests
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
