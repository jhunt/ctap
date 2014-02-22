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

#define CTAP_UNIT_TESTS
#include "ctap.h"

tests {
	no_plan();

	subtest {

		const char *s1 = "Hello, Testing!";
		const char *s2 = "A second string";
		const char *s3 = "Hello, Testing!";
		const char *null = NULL;

		is_string(s1, s3, "s1 == s3");
		is_string(s1, s1, "s1 == s1");
		is_string(null, NULL, "handle NULL");

#if 0
		should_fail {
			is_string("SHOULD FAIL: s1 == s2", s1, s2);
		}

		should_fail {
			is_string("SHOULD FAIL: s1 == NULL", s1, NULL);
		}

		should_fail {
			is_string("SHOULD FAIL: NULL == s3", NULL, s3);
		}
#endif

		isnt_string(s1, s2, "s1 != s2");
		isnt_string(s1, NULL, "s1 != NULL");

#if 0
		should_fail {
			isnt_string("SHOULD FAIL: s1 != s3", s1, s3);
		}

		should_fail {
			isnt_string("SHOULD FAIL: null != NULL", null, NULL);
		}

		should_fail {
			isnt_string("SHOULD FAIL: NULL != null", NULL, null);
		}
#endif
		done_testing();
	}
}

#if 0
TEST(integers)
{
	test("assert_int_ne");
	assert_int_ne("1 != 4", 1, 4);
	assert_int_ne("-42 != 42", -42, 42);
	test("assert_int_ne failures");
	assert_int_ne("SHOULD FAIL: 56 != 56", 56, 56);
	ct_unfail();

	test("assert_int_lt");
	assert_int_lt("0 < 90", 0, 90);
	test("assert_int_lt failures");
	assert_int_lt("SHOULD FAIL: 1468 < 1468", 1468, 1468);
	ct_unfail();
	assert_int_lt("SHOULD FAIL: 1468 < 14", 1468, 14);
	ct_unfail();

	test("assert_int_le");
	assert_int_le("4 <= 7", 4, 7);
	assert_int_le("7 <= 7", 7, 7);
	test("assert_int_le failures");
	assert_int_le("SHOULD FAIL: 5 <= 3", 5, 3);
	ct_unfail();

	test("assert_int_eq");
	assert_int_eq("600 == 600", 600, 600);
	test("assert_int_eq failures");
	assert_int_eq("SHOULD FAIL: 18 == 25", 18, 25);
	ct_unfail();

	test("assert_int_ge");
	assert_int_ge("7 >= 4", 7, 4);
	assert_int_ge("7 >= 7", 7, 7);
	test("assert_int_ge failures");
	assert_int_ge("SHOULD FAIL: 9 >= 104", 9, 104);
	ct_unfail();

	test("assert_int_gt");
	assert_int_gt("90 > 0", 90, 0);
	test("assert_int_gt failures");
	assert_int_gt("SHOULD FAIL: 1 > 72", 1, 72);
	ct_unfail();
	assert_int_gt("SHOULD FAIL: 2323 > 2323", 2323, 2323);
	ct_unfail();

	test("assert_signed_eq");
	assert_signed_eq("-5 == -5", -5, -5);
	assert_signed_eq(" 5 ==  5",  5,  5);
	test("assert_signed_eq failures");
	assert_signed_eq("SHOULD FAIL:  5 ==  -5",  5,  -5);
	ct_unfail();
	assert_signed_eq("SHOULD FAIL: -5 ==   5", -5,   5);
	ct_unfail();
	assert_signed_eq("SHOULD FAIL: -6 == -89", -6, -89);
	ct_unfail();
	assert_signed_eq("SHOULD FAIL:  6 == -89",  6, -89);
	ct_unfail();
	assert_signed_eq("SHOULD FAIL: -6 ==  89", -6,  89);
	ct_unfail();
	assert_signed_eq("SHOULD FAIL:  6 ==  89",  6,  89);
	ct_unfail();

	test("assert_unsigned_eq");
	assert_unsigned_eq("77 == 77", 77, 77);
	test("assert_unsigned_eq failures");
	assert_unsigned_eq("SHOULD FAIL: 43 == 42", 43, 42);
	ct_unfail();
}

TEST(pointers)
{
	void *p1 = (void*)0xdead;
	void *p2 = (void*)0xbeef;
	void *p3 = (void*)0xdead;

	char s1[] = "string";
	char s2[] = "string";

	test("assert_ptr_eq");
	assert_ptr_eq("p1 == p3", p1, p3);
	test("assert_ptr_eq failures");
	assert_ptr_eq("SHOULD FAIL: p1 == p2", p1, p2);
	assert_ptr_eq("SHOULD FAIL: p1 == NULL", p1, NULL);
	ct_unfail();

	test("assert_ptr_ne");
	assert_ptr_ne("p1 != NULL", p1, NULL);
	assert_ptr_ne("p1 != p2", p1, p2);
	assert_ptr_ne("s1 != s2 (diff mem / same string)", s1, s2);
	test("assert_ptr_ne failures");
	assert_ptr_ne("SHOULD FAIL: p1 != p3", p1, p3);
	ct_unfail();
}
#endif
