AM_CPPFLAGS = -I$(srcdir)/../src
LDADD = $(top_builddir)/src/libctest.la

TESTS = 00-asserts.t
check_PROGRAMS = 00-asserts.t
00_asserts_t_SOURCES = 00-asserts.c ../src/ctest.h

###########################################

if HAVE_CDOC
docs:
	@rm -fr docs
	@mkdir -p docs
	cdoc --root docs -v *.c *.h
endif

if WANT_COVERAGE
coverage-clean:
	@rm -fr coverage
	@find . -name "*.gcda" -exec rm {} \;
	@lcov --directory . --zerocounters

coverage-report:
	@mkdir -p coverage
	@lcov --compat-libtool --directory . --capture --output-file coverage/app.info
	@genhtml -o coverage/ coverage/app.info

coverage:
	@make coverage-report

clean-local:
	@make coverage-clean

check:
	@make coverage
endif
