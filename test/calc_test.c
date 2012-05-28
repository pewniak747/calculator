#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/rpn.h"

#define EPS 0.000001
#define CU_ASSERT_EQ(i1, i2) CU_ASSERT(abs(i1 - i2) < EPS)

double result;
int error;

void testSanity(void) {
  int i1 = 2, i2 = 1;
  CU_ASSERT_EQ(i1 + i2, 3);
}

void testSingular(void) {
  rpn_resolve("2", &result, &error);
  CU_ASSERT_EQ(result, 2);
}

void testAddition(void) {
  rpn_resolve("2+2", &result, &error);
  CU_ASSERT_EQ(result, 4);

  rpn_resolve("1+2+3", &result, &error);
  CU_ASSERT_EQ(result, 6);
}

void testSubtraction(void) {
  rpn_resolve("10-2", &result, &error);
  CU_ASSERT_EQ(result, 8);

  rpn_resolve("4-3-9", &result, &error);
  CU_ASSERT_EQ(result, -8);
}

void testMultiplication(void) {
  rpn_resolve("1*2", &result, &error);
  CU_ASSERT_EQ(result, 2);

  rpn_resolve("4*3", &result, &error);
  CU_ASSERT_EQ(result, 12);

  rpn_resolve("0*3", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("2*3*4", &result, &error);
  CU_ASSERT_EQ(result, 24);
}

void testDivision(void) {
  rpn_resolve("1/2", &result, &error);
  CU_ASSERT_EQ(result, 1/2);

  rpn_resolve("4/3", &result, &error);
  CU_ASSERT_EQ(result, 4/3);

  rpn_resolve("0/3", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("2/3/4", &result, &error);
  CU_ASSERT_EQ(result, 2/12);
}

void testExponentation(void) {
  rpn_resolve("1^2", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("4^2", &result, &error);
  CU_ASSERT_EQ(result, 16);

  rpn_resolve("3^0", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("2^3^4", &result, &error);
  CU_ASSERT_EQ(result, 262144);

  rpn_resolve("4^(1/2)", &result, &error);
  CU_ASSERT_EQ(result, 2);
}
void testDecimalPoint(void) {
  rpn_resolve("2.0", &result, &error);
  CU_ASSERT_EQ(result, 2);

  rpn_resolve("2.12345", &result, &error);
  CU_ASSERT_EQ(result, 2.12345);

  rpn_resolve("2.5+3.3", &result, &error);
  CU_ASSERT_EQ(result, 5.8);

  rpn_resolve("2.5*3", &result, &error);
  CU_ASSERT_EQ(result, 7.5);

  rpn_resolve("2.5-3.3", &result, &error);
  CU_ASSERT_EQ(result, -0.8);

  rpn_resolve("2.5*3.3", &result, &error);
  CU_ASSERT_EQ(result, 8.25);

  rpn_resolve("2.5/3.3", &result, &error);
  CU_ASSERT_EQ(result, 0.757575758);
}

void testPrecedence(void) {
  rpn_resolve("2+3*4", &result, &error);
  CU_ASSERT_EQ(result, 14);

  rpn_resolve("2+3*4-1", &result, &error);
  CU_ASSERT_EQ(result, 13);

  rpn_resolve("2+3*4/2-1", &result, &error);
  CU_ASSERT_EQ(result, 7);

  rpn_resolve("2*4+1+3*4/2", &result, &error);
  CU_ASSERT_EQ(result, 15);

  rpn_resolve("2*4^2/8", &result, &error);
  CU_ASSERT_EQ(result, 4);
}

void testBrackets(void) {
  rpn_resolve("(2)", &result, &error);
  CU_ASSERT_EQ(result, 2);

  rpn_resolve("(2+3)*4", &result, &error);
  CU_ASSERT_EQ(result, 20);

  rpn_resolve("(2+3)*(4-1)", &result, &error);
  CU_ASSERT_EQ(result, 15);

  rpn_resolve("(2+(1+3)/2)*(4-1)", &result, &error);
  CU_ASSERT_EQ(result, 12);
}

void testUnaryMinus(void) {
  rpn_resolve("-8", &result, &error);
  CU_ASSERT_EQ(result, -8);

  rpn_resolve("(-8)", &result, &error);
  CU_ASSERT_EQ(result, -8);

  rpn_resolve("-2+3", &result, &error);
  CU_ASSERT_EQ(result, -1);

  rpn_resolve("7*-2", &result, &error);
  CU_ASSERT_EQ(result, -14);

  rpn_resolve("-(2+2)", &result, &error);
  CU_ASSERT_EQ(result, -4);

  rpn_resolve("-(2+2*3)-1", &result, &error);
  CU_ASSERT_EQ(result, -13);
}

void testErrors(void) {
  rpn_resolve("3/0", &result, &error);
  CU_ASSERT_EQ(error, 1);

  rpn_resolve("2/3/0", &result, &error);
  CU_ASSERT_EQ(error, 1);
}

int main() {
  CU_pSuite pSuite = NULL;
  CU_initialize_registry();

  // add a suite
  pSuite = CU_add_suite("RPN resolve test", NULL, NULL);

  // add tests
  CU_add_test(pSuite, "sanity", testSanity);
  CU_add_test(pSuite, "singular number", testSingular);
  CU_add_test(pSuite, "addition", testAddition);
  CU_add_test(pSuite, "subtraction", testSubtraction);
  CU_add_test(pSuite, "multiplication", testMultiplication);
  CU_add_test(pSuite, "division", testDivision);
  CU_add_test(pSuite, "exponentation", testExponentation);
  CU_add_test(pSuite, "decimal point", testDecimalPoint);
  CU_add_test(pSuite, "operator precedence", testPrecedence);
  CU_add_test(pSuite, "brackets", testBrackets);
  //CU_add_test(pSuite, "unary minus", testUnaryMinus);
  CU_add_test(pSuite, "errors", testErrors);

  // run tests
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
