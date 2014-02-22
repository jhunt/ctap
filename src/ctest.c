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

#include <stdio.h>
#include "ctest.h"

struct ctest {
	const char *name;
	ctest_f     runner;
};

int TEST_PRINT_PASS = 0;
int TEST_PRINT_FAIL = 1;

static int __SUBTESTS   = 0;
static int __TESTS      = 0;
static int __ASSERTIONS = 0;
static int __FAILURES   = 0;

static struct ctest *CTESTS = NULL;

/**********************************************************/

static inline void __test_failed(void)
{
	++__FAILURES;
}

/**********************************************************/

void test(const char *s)
{
	++__TESTS;
	printf("%s\n", s);
}

/**********************************************************/

void ct_unfail(void)
{
	--__FAILURES;
}

void ct_add_test(const char *name, ctest_f runner)
{
	struct ctest *t;

	CTESTS = realloc(CTESTS, (__SUBTESTS * sizeof(struct ctest)));
	if (!CTESTS) {
		fprintf(stderr, "Failed to register suite '%s'\n", name);
		exit(99);
	}
	t = CTESTS + (__SUBTESTS - 1);
	t->name   = name;
	t->runner = runner;
}

int ct_run_tests(int argc, char **argv)
{
	int i;

	while (*(++argv)) {
		if (strcmp(*argv, "-v") == 0) {
			TEST_PRINT_PASS = 1;
			TEST_PRINT_FAIL = 1;
		} else if (strcmp(*argv, "-q") == 0) {
			TEST_PRINT_PASS = 0;
			TEST_PRINT_FAIL = 0;
		} else {
			fprintf(stderr, "Unrecognized option '%s'\n", *argv);
			exit(1);
		}
	}

	for (i = 0; i < __SUBTESTS; i++) {
		(*(CTESTS[i].runner))();
	}

	free(CTESTS);
	CTESTS = NULL;

	printf("\n"
	       "--------------------\n"
	       "TEST RESULTS SUMMARY\n"
	       "--------------------\n");
	printf("%4i test(s)\n"
	       "%4i assertion(s)\n"
	       "\n"
	       "%4i FAILURE(S)\n",
	       __TESTS, __ASSERTIONS, __FAILURES);

	return __FAILURES;
}


/**
  Fail
 */
void assert_fail(const char *s)
{
	++__ASSERTIONS;
	__test_failed();
	if (TEST_PRINT_FAIL) { printf(" - %s: FAIL\n", s); }
}

/**
  Pass
 */
void assert_pass(const char *s)
{
	++__ASSERTIONS;
	if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
}

/**
  Check that $value is true (non-zero)
 */
void assert_true(const char *s, int value)
{
	++__ASSERTIONS;
	(value ? assert_pass(s) : assert_fail(s));
}

/**
  Check that $value is false (zero)
 */
void assert_false(const char *s, int value)
{
	assert_true(s, !value);
}

/**
  Check that $ptr is not NULL
 */
void assert_not_null(const char *s, const void *ptr)
{
	++__ASSERTIONS;
	if (ptr != NULL) {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	} else {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %p is NULL\n", s, ptr);
		}
	}
}

/**
  Check that $ptr is NULL
 */
void assert_null(const char *s, const void *ptr)
{
	++__ASSERTIONS;
	if (ptr == NULL) {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	} else {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %p is not NULL\n", s, ptr);
		}
	}
}

#define _assert_numeric_equals(s, fmt, expected, actual) do {\
	++__ASSERTIONS; \
	if (expected == actual) { \
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); } \
	} else { \
		__test_failed(); \
		if (TEST_PRINT_FAIL) { \
			printf(" - %s: FAIL:\n" \
			       "\t\texpected " fmt "\n" \
			       "\t\t but got " fmt "\n", \
			       s, expected, actual); \
		} \
	} \
} while (0)

/**
  Check that $expected == $actual

  This assertion handles large unsigned integer values,
  beyond the range of a signed long.
 */
void assert_unsigned_eq(const char *s, unsigned long int expected, unsigned long int actual)
{
	_assert_numeric_equals(s, "%lu", expected, actual);
}

/**
  Check that $expected == $actual

  This assertion is a logical counterpart to
  @assert_unsigned_eq, for large signed integers.
 */
void assert_signed_eq(const char *s, signed long int expected, signed long int actual)
{
	_assert_numeric_equals(s, "%li", expected, actual);
}

/**
  Check that two pointers are equal
 */
void assert_ptr_eq(const char *s, const void *expected, const void *actual)
{
	_assert_numeric_equals(s, "0x%lx", (unsigned long)expected, (unsigned long)actual);
}

/**
  Check that two pointers are not equal
 */
void assert_ptr_ne(const char *s, const void *unexpected, const void *actual)
{
	++__ASSERTIONS;
	if (actual != unexpected) {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	} else {
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL:\n"
			       "\t\t0x%lx == 0x%lx",
			       s, (unsigned long)unexpected, (unsigned long)actual);
		}
	}
}

/**
  Check that $expected == $actual
 */
void assert_int_eq(const char *s, int expected, int actual)
{
	++__ASSERTIONS;
	if (expected == actual) {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	} else {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i != %i\n", s, actual, expected);
		}
	}
}

/**
  Check that $expected != $actual
 */
void assert_int_ne(const char *s, int actual, int unexpected)
{
	++__ASSERTIONS;
	if (unexpected == actual) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i == %i\n", s, actual, unexpected);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

/**
  Check that $actual is > $threshold
 */
void assert_int_gt(const char *s, int actual, int threshold)
{
	++__ASSERTIONS;
	if (actual <= threshold) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i <= %i\n", s, actual, threshold);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

/**
  Check that $actual is >= $threshold
 */
void assert_int_ge(const char *s, int actual, int threshold)
{
	++__ASSERTIONS;
	if (actual < threshold) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i < %i\n", s, actual, threshold);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

/**
  Check that $actual is < $threshold
 */
void assert_int_lt(const char *s, int actual, int threshold)
{
	++__ASSERTIONS;
	if (actual >= threshold) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i >= %i\n", s, actual, threshold);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

/**
  Check that $actual <= $threshold
 */
void assert_int_le(const char *s, int actual, int threshold)
{
	++__ASSERTIONS;
	if (actual > threshold) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %i > %i\n", s, actual, threshold);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

/**
  Check that two strings are equivalent

  This assertion does a character-by-character comparison of
  two NULL-terminated character strings.  It handles NULL values
  gracefully.
 */
void assert_str_eq(const char *s, const char *actual, const char *expected)
{
	++__ASSERTIONS;
	if (expected == NULL) { expected = "(null)"; }
	if (actual == NULL)   { actual   = "(null)"; }

	if (strcmp(expected, actual) == 0) {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	} else {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL:\n\t\"%s\" !=\n\t\"%s\"\n", s, actual, expected);
		}
	}
}

/**
  Check that two string are not equivalent

  This assertion does a character-by-character comparison of
  two NULL-terminated character strings.  It handles NULL values
  gracefully.
 */
void assert_str_ne(const char *s, const char *actual, const char *unexpected)
{
	++__ASSERTIONS;
	if (unexpected == NULL) { unexpected = "(null)"; }
	if (actual == NULL)     { actual     = "(null)"; }

	if (strcmp(unexpected, actual) == 0) {
		__test_failed();
		if (TEST_PRINT_FAIL) {
			printf(" - %s: FAIL: %s == %s\n", s, actual, unexpected);
		}
	} else {
		if (TEST_PRINT_PASS) { printf(" - %s: PASS\n", s); }
	}
}

