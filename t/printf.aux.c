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

	pass("A - %d %d %d...", 1, 2, 3);
	fail("B - %d %d %d...", 1, 2, 3);

	ok(1, "C - %d %d %d...", 1, 2, 3);
	is("test", "test", "D - %d %d %d...", 1, 2, 3);
	isnt("test", "test", "E - %d %d %d...", 1, 2, 3);

	printf("THIS SHOULD NOT SHOW UP IN THE OUTPUT AT ALL\n");

	done_testing();
}
