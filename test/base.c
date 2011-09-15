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

#include "test.h"

NEW_TEST(base)
{
	const char *string = "a string";
	void *bad_pointer  = (void*)0xdecafbad;
	void *null = NULL;

	test("assert_pass");
	assert_pass("Should always pass");
	assert_pass("So should this");

	test("assert_fail");
	assert_fail("SHOULD FAIL: assert_fail");
	ctest_unfail();

	test("assert_true");
	assert_true("1 is a true value", 1);
	assert_true("any non-zero is a true value", 42);
	assert_true("negatives are true values", -1234);
	test("assert_true failures");
	assert_true("SHOULD FAIL: 0 is true", 0);
	ctest_unfail();

	test("assert_false");
	assert_false("0 is a false value", 0);
	assert_false("NULL is a false value", (int)NULL);
	test("assert_false failures");
	assert_false("SHOULD FAIL: 1 is false", 1);
	ctest_unfail();

	test("assert_not_null");
	assert_not_null("Initialized pointer is not null", string);
	assert_not_null("Bad pointers are not null", bad_pointer);
	test("assert_not_null failures");
	assert_not_null("SHOULD FAIL: NULL is not NULL", NULL);
	ctest_unfail();

	test("assert_null");
	assert_null("NULL pointers are null", null);
	test("assert_null failures");
	assert_null("SHOULD FAIL: string is NULL", string);
	ctest_unfail();
}

NEW_SUITE(base)
{
	RUN_TEST(base);
}
