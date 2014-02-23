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

	ctap_assert(1, "success!",    1, "file.c", 1001);
	ctap_assert(0, "autodiag",    1, "file.c", 1002);
	ctap_assert(0, "no-autodiag", 0, "file.c", 1003);
	ctap_assert(0, "no-file",     1, NULL,     1004);
	ctap_assert(0, NULL,          1, "file.c", 1004);
	diag("this is a diag message");

	skip("[skip reason here]") {
		ctap_assert(1, "skipped ok",   1, "file.c", 2001);
		ctap_assert(0, "skipped fail", 1, "file.c", 2002);
		ctap_assert(0, NULL,           0, NULL,     2003);
	}

	todo("not quite ready") {
		ctap_assert(1, "todo ok",   1, "file.c", 3001);
		ctap_assert(0, "todo fail", 0, "file.c", 3002);
		ctap_assert(0, "no file",   1, NULL,     3003);
		ctap_assert(0, NULL,        0, NULL,     3004);
	}

	done_testing();
}
