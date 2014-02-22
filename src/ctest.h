/*
  Copyright 2014 James Hunt <james@jameshunt.us>

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

typedef void (*ctest_f)(void);


void test(const char *s);

void ct_unfail(void);
void ct_add_test(const char *name, ctest_f runner);
int  ct_run_tests(int argc, char **argv);

/**
  Define a Test Case

  This macro provides the function prototype header for a test
  case, and should be used as such:

  <code>
  TEST(mytest) {
    // ... assertions and testing here ...
  }
  </code>

  Tests will be run automatically when DONE_TESTING is seen.
 */
#define TEST(x)  void ct_test_ ## x(void); \
                 ct_add_test(#x, ct_test_ ## x); \
                 void ct_test_ ## x(void)

/**
  Run the Tests

  This macro invokes all defined test cases.

  <code>
  DONE_TESTING;
  </code>

  Tests will be run in the order they were defined.
 */
#define DONE_TESTING int main(int c, char **l) { return ct_run_tests(c,l); }

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
