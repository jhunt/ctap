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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

TESTS {
	no_plan();

	int rc;
	pid_t pid;

	pass("in parent");
	pid = fork();
	switch (pid) {
	case -1:
		BAIL_OUT("fork failed!");

	case 0: /* child */
		exit(4);

	default:
		waitpid(pid, &rc, 0);
		ok(WIFEXITED(rc), "child process exited normally");
		is_int(WEXITSTATUS(rc), 4, "child process exit code");
	}

	done_testing();
}
