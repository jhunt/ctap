AM_CPPFLAGS = -I$(srcdir)/../src
LDADD = $(top_builddir)/src/libctap.la

if GCOV_ENABLED
AM_CFLAGS  = @GCOV_CFLAGS@
AM_LDFLAGS = @GCOV_LDFLAGS@
endif

TESTS = 00-asserts.t 01-fail.t 02-skip.t 03-todo.t 04-bail.t 05-premature.t
check_PROGRAMS = 00-asserts.t 01-fail 02-skip.t 03-todo.t 04-bail 05-premature
00_asserts_t_SOURCES = 00-asserts.c   ../src/ctap.h
01_fail_SOURCES      = 01-fail.c      ../src/ctap.h
02_skip_t_SOURCES    = 02-skip.c      ../src/ctap.h
03_todo_t_SOURCES    = 03-todo.c      ../src/ctap.h
04_bail_SOURCES      = 04-bail.c      ../src/ctap.h
05_premature_SOURCES = 05-premature.c ../src/ctap.h

###########################################

if GCOV_ENABLED
coverage-clean:
	@rm -fr coverage
	@find . -name "*.gcda" -exec rm {} \;
	@lcov --directory . --zerocounters

coverage-report:
	@mkdir -p coverage
	@lcov --compat-libtool --directory .. --capture --output-file coverage/app.info
	@genhtml -o coverage/ coverage/app.info

coverage:
	@make coverage-report

clean-local:
	@make coverage-clean

check:
	@make coverage

.PHONY: coverage-clean coverage-report coverage
endif
