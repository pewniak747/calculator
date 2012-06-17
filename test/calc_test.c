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
  CU_ASSERT_EQ(result, 1.0/2);

  rpn_resolve("4/3", &result, &error);
  CU_ASSERT_EQ(result, 4.0/3);

  rpn_resolve("0/3", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("2/3/4", &result, &error);
  CU_ASSERT_EQ(result, 2.0/12);
}

void testExponentation(void) {
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

  rpn_resolve("(2+3)-(3*3)", &result, &error);
  CU_ASSERT_EQ(result, -4);

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
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("7*-2", &result, &error);
  CU_ASSERT_EQ(result, -14);

  rpn_resolve("-(2+2)", &result, &error);
  CU_ASSERT_EQ(result, -4);

  rpn_resolve("-(2+2*3)-1", &result, &error);
  CU_ASSERT_EQ(result, -9);
}

void testSqrt(void) {
  rpn_resolve("sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, 1.414213562);

  rpn_resolve("1-sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, -0.414213562);

  rpn_resolve("sqrt(2)*sqrt(2)", &result, &error);
  CU_ASSERT_EQ(result, 2);
}

void testSine(void) {
  rpn_resolve("sin(0)", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("sin(30)", &result, &error);
  CU_ASSERT_EQ(result, 0.5);

  rpn_resolve("sin(45)", &result, &error);
  CU_ASSERT_EQ(result, 0.70710678);

  rpn_resolve("sin(60)", &result, &error);
  CU_ASSERT_EQ(result, 0.8660254);

  rpn_resolve("sin(90)", &result, &error);
  CU_ASSERT_EQ(result, 1);
}

void testCosine(void) {
  rpn_resolve("cos(0)", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("cos(30)", &result, &error);
  CU_ASSERT_EQ(result, 0.8660254);


  rpn_resolve("cos(45)", &result, &error);
  CU_ASSERT_EQ(result, 0.70710678);

  rpn_resolve("cos(60)", &result, &error);
  CU_ASSERT_EQ(result, 0.5);

  rpn_resolve("cos(90)", &result, &error);
  CU_ASSERT_EQ(result, 0);
}

void testTangens(void) {
  rpn_resolve("tan(0)", &result, &error);
  CU_ASSERT_EQ(result, 0);

  rpn_resolve("tan(30)", &result, &error);
  CU_ASSERT_EQ(result, 0.5773502);


  rpn_resolve("tan(45)", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("tan(60)", &result, &error);
  CU_ASSERT_EQ(result, 1.732050);
}

void testExponential(void) {
  rpn_resolve("exp(0)", &result, &error);
  CU_ASSERT_EQ(result, 1);

  rpn_resolve("exp(1)", &result, &error);
  CU_ASSERT_EQ(result, 2.7182818);


  rpn_resolve("exp(2)", &result, &error);
  CU_ASSERT_EQ(result, 7.389056);

  rpn_resolve("exp(-1)", &result, &error);
  CU_ASSERT_EQ(result, 0.3678794);
}

void testErrors(void) {
  rpn_resolve("3/0", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);

  rpn_resolve("(2/3)/0", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);

  rpn_resolve("sqrt(-1)", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);
}

void testGibberish(void){
  rpn_resolve("124.12412.41241.", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);

  rpn_resolve("(1241254-/3)", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);

  rpn_resolve("dskla21ru*", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);

  rpn_resolve("+asf", &result, &error);
  CU_ASSERT_NOT_EQUAL(error, 0);
}

void testParseNum(void) {
  CU_ASSERT_EQ(2, rpn_parsenum("2"));
  CU_ASSERT_EQ(2, rpn_parsenum("2.0"));
  CU_ASSERT_EQ(2.5, rpn_parsenum("2.5"));
  CU_ASSERT_EQ(2.12345, rpn_parsenum("2.12345"));
  CU_ASSERT_EQ(123, rpn_parsenum("123"));
  CU_ASSERT_EQ(123.321, rpn_parsenum("123.321"));
  CU_ASSERT_EQ(-1, rpn_parsenum("-1"));
  CU_ASSERT_EQ(-0.5, rpn_parsenum("-0.5"));
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
  CU_add_test(resolveSuite, "sine", testSine);
  CU_add_test(resolveSuite, "cosine", testCosine);
  CU_add_test(resolveSuite, "tangens", testTangens);
  CU_add_test(resolveSuite, "exponential function", testExponential);
  CU_add_test(resolveSuite, "unary minus", testUnaryMinus);
  CU_add_test(resolveSuite, "errors", testErrors);
  //CU_add_test(resolveSuite, "gibberish", testGibberish);

  CU_add_test(helpersSuite, "parsing number", testParseNum);

  // run tests
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
