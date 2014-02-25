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

TESTS {
	no_plan();

	ctap_assert(1, 1, "file.c", 1001, "success!");
	ctap_assert(0, 1, "file.c", 1002, "autodiag");
	ctap_assert(0, 0, "file.c", 1003, "no-autodiag");
	ctap_assert(0, 1, NULL,     1004, "no-file");
	ctap_assert(0, 1, "file.c", 1004, NULL);
	diag("this is a diag message");

	SKIP("[skip reason here]") {
		ctap_assert(1, 1, "file.c", 2001, "skipped ok");
		ctap_assert(0, 1, "file.c", 2002, "skipped fail");
		ctap_assert(0, 0, NULL,     2003, NULL);
	}

	TODO("not quite ready") {
		ctap_assert(1, 1, "file.c", 3001, "todo ok");
		ctap_assert(0, 0, "file.c", 3002, "todo fail");
		ctap_assert(0, 1, NULL,     3003, "no file");
		ctap_assert(0, 0, NULL,     3004, NULL);
	}

	done_testing();
}
