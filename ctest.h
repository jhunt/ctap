/*
  Copyright 2011 James Hunt <james@jameshunt.us>

  This file is part of ctest - a C Unit Testing Framework.

  ctest is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ctest is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ctest.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LIB_CTEST_H
#define _LIB_CTEST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef void (*test_suite_f)(void);

struct test_suite {
	const char    *name;
	test_suite_f   runner;
	int            active;
};

#ifdef CTEST_TEST_SUITES
void ctest_unfail(void);
#endif

void test(const char *s);

int add_test_suite(const char *name, test_suite_f runner, int active);
int activate_test(const char *name);
int run_tests(int argc, char **argv);

/**
  Declare a Test Suite

  This macro should be called from the `main` function of the
  test runner.  It declares and register a test suite with
  ctest.

  This call should be matched by a call to @NEW_SUITE to
  define the test suite.
 */
#define TEST_SUITE(x) extern void test_suite_ ## x(); add_test_suite(#x, test_suite_ ## x, 0)

/**
  Define a New Test Case

  This macro provides the function prototype header for a test
  case, and should be used as such:

  <code>
  NEW_TEST(mytest) {
    // ... assertions and testing here ...
  }
  </code>

  In order to run the defined test case from a test suite,
  see @RUN_TEST.
 */
#define NEW_TEST(x)  void test_case_  ## x(void)

/**
  Define a New Test Suite

  This macro provides the function prototype header for a test
  suite (declared using TEST_SUITE in the test runner's `main`
  function).

  Example:

  <code>
  int main(int argc, char **argv)
  {
    TEST_SUITE(some_tests);
    // ... more test suite declarations ...
  }

  NEW_SUITE(some_tests)
  {
    RUN_TEST(test1);
    RUN_TEST(test2);
    // ... etc.
  }
  </code>
 */
#define NEW_SUITE(x) void test_suite_ ## x(void)

/**
  Run a Test Case, from a Test Suite

  This macro invokes a test case function, and is intended
  to be called from with a @NEW_SUITE function body.
 */
#define RUN_TEST(x)  test_case_ ## x()

/* ASSERTIONS */

void assert_fail(const char *s);
void assert_pass(const char *s);
void assert_true(const char *s, int value);
void assert_false(const char *s, int value);
void assert_not_null(const char *s, const void *ptr);
void assert_null(const char *s, const void *ptr);

void assert_unsigned_eq(const char *s, unsigned long int expected, unsigned long int actual);
void assert_signed_eq(const char *s, signed long int expected, signed long int actual);

void assert_ptr_eq(const char *s, const void *expected, const void *actual);
void assert_ptr_ne(const char *s, const void *unexpected, const void *actual);

void assert_int_eq(const char *s, int expected, int actual);
void assert_int_ne(const char *s, int unexpected, int actual);
void assert_int_gt(const char *s, int actual, int threshold);
void assert_int_ge(const char *s, int actual, int threshold);
void assert_int_lt(const char *s, int actual, int threshold);
void assert_int_le(const char *s, int actual, int threshold);

void assert_str_eq(const char *s, const char *expected, const char *actual);
void assert_str_ne(const char *s, const char *unexpected, const char *actual);

#endif
