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

#include "ctap.h"

tests {
	no_plan();

	subtest {
		const char *s1 = "Hello, Testing!";
		const char *s2 = "A second string";
		const char *s3 = "Hello, Testing!";

		is_string(s1,   s2,   "s1 == s2");
		is_string(s1,   NULL, "s1 == NULL");
		is_string(NULL, s3,   "NULL == s3");
		is_string(NULL, NULL, "null strings are not equiv.");

		isnt_string(s1,   s3,   "s1 != s3");
	}

	subtest {
		isnt_int(56, 56, "56 != 56");
		isnt_int(-9, -9, "-9 != -9");

		is_int( 5,  -5, " 5 ==  -5");
		is_int(-5,   5, "-5 ==   5");
		is_int(-6, -89, "-6 == -89");
		is_int( 6, -89, " 6 == -89");
		is_int(-6,  89, "-6 ==  89");
		is_int( 6,  89, " 6 ==  89");

		is_uint(43, 42, "43U ==  42U");
		isnt_uint(42, 42, "42U != 42U");
	}

	subtest {
		void *p1 = (void*)0xdead;
		void *p2 = (void*)0xbeef;
		void *p3 = (void*)0xdead;

		char s1[] = "string";
		char s2[] = "string";

		is_ptr(p1, p2,   "p1 == p2");
		is_ptr(p1, NULL, "p1 == NULL");
		is_pointer(p1, p2,   "p1 == p2");

		isnt_ptr(p1, p3, "p1 != p3");
		isnt_pointer(p1, p3, "p1 != p3");
	}

	done_testing();
}
