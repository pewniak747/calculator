#include <stdlib.h>
#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/rpn.h"

#define EPS 0.000001
#define CU_ASSERT_EQ(i1, i2) CU_ASSERT(fabs((i1)-(i2))<EPS)

double result;
int error;

void testSanity(void) {
  int i1 = 2, i2 = 1;
  CU_ASSERT_EQ(i1 + i2, 3);
}

void testSingular(void) {
  error = 0;
  rpn_resolve("2", &result, &error);
  CU_ASSERT_EQ(result, 2);
}

void testAddition(void) {
  error = 0;
  rpn_resolve("2+2", &result, &error);
  CU_ASSERT_EQ(result, 4);

  rpn_resolve("1+2+3", &result, &error);
  CU_ASSERT_EQ(result, 6);
}

void testSubtraction(void) {
  error = 0;
  rpn_resolve("10-2", &result, &error);
  CU_ASSERT_EQ(result, 8);

  rpn_resolve("4-3-9", &result, &error);
  CU_ASSERT_EQ(result, -8);
}

void testMultiplication(void) {
  error = 0;
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
  error = 0;
  rpn_resolve("1/2", &result, &error);
  CU_ASSERT_EQ(result, 1.0/2);

  rpn_resolve("4/3", &result, &error);
  CU_ASSERT_EQ(result, 4.0/3);

  rpn_resolve("0/3", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("2/3/4", &result, &error);
  CU_ASSERT_EQ(result, 2.0/12);
}

void testExponentation(void) {
  error = 0;
  rpn_resolve("1^2", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("4^2", &result, &error);
  CU_ASSERT_EQ(result, 16);

  rpn_resolve("3^0", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("4^3^2", &result, &error);
  CU_ASSERT_EQ(result, 262144);

  rpn_resolve("4^(1/2)", &result, &error);
  CU_ASSERT_EQ(result, 2);
}
void testDecimalPoint(void) {
  error = 0;
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
  error = 0;
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
  error = 0;
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
  error = 0;
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

void testSqrt(void) {
  error = 0;
  rpn_resolve("sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, 1.414213562);

  rpn_resolve("1-sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, 0.414213562);

  rpn_resolve("sqrt(2)*sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, 0.414213562);
}

void testErrors(void) {
  error = 0;
  rpn_resolve("3/0", &result, &error);
  CU_ASSERT_EQ(error, 1);

  rpn_resolve("2/3/0", &result, &error);
  CU_ASSERT_EQ(error, 1);
}

void testParseNum(void) {
  CU_ASSERT_EQ(2, rpn_parsenum("2"));
  CU_ASSERT_EQ(2, rpn_parsenum("2.0"));
  CU_ASSERT_EQ(2.5, rpn_parsenum("2.5"));
  CU_ASSERT_EQ(2.12345, rpn_parsenum("2.12345"));
  CU_ASSERT_EQ(123, rpn_parsenum("123"));
  CU_ASSERT_EQ(123.321, rpn_parsenum("123.321"));
}

int main() {
  CU_pSuite resolveSuite = NULL;
  CU_pSuite helpersSuite = NULL;
  CU_initialize_registry();

  // add suites
  resolveSuite = CU_add_suite("RPN resolve test", NULL, NULL);
  helpersSuite = CU_add_suite("RPN helpers test", NULL, NULL);

  // add tests
  CU_add_test(resolveSuite, "sanity", testSanity);
  CU_add_test(resolveSuite, "singular number", testSingular);
  CU_add_test(resolveSuite, "addition", testAddition);
  CU_add_test(resolveSuite, "subtraction", testSubtraction);
  CU_add_test(resolveSuite, "multiplication", testMultiplication);
  CU_add_test(resolveSuite, "division", testDivision);
  CU_add_test(resolveSuite, "exponentation", testExponentation);
  CU_add_test(resolveSuite, "decimal point", testDecimalPoint);
  CU_add_test(resolveSuite, "operator precedence", testPrecedence);
  CU_add_test(resolveSuite, "brackets", testBrackets);
  CU_add_test(resolveSuite, "square root", testSqrt);
  //CU_add_test(resolveSuite, "unary minus", testUnaryMinus);
  CU_add_test(resolveSuite, "errors", testErrors);

  CU_add_test(helpersSuite, "parsing number", testParseNum);

  // run tests
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
