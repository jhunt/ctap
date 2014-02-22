AC_INIT([ctest], [1.0.1], [bugs@niftylogic.com])
AC_PREREQ([2.65])
AC_CONFIG_SRCDIR([src/ctest.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

#########################################################

AC_DEFUN_ONCE([AX_TEST_COVERAGE], [
  AC_ARG_ENABLE(coverage,
    AS_HELP_STRING([--enable-coverage],
    [enable code coverage reporting via gcov/lcov]),
    [test_coverage=yes], [test_coverage=no])

  AM_CONDITIONAL(WANT_COVERAGE, test "x$test_coverage" = "xyes")

  if test "$GCC" != "yes"; then
    AC_MSG_ERROR([GCC is required for --enable-coverage])
  fi

  AC_CHECK_PROG(LCOV, lcov, lcov)
  if test -z "$LCOV"; then
    AC_MSG_ERROR([Could not find lcov])
  fi

  AC_CHECK_PROG(GENHTML, genhtml, genhtml)
  if test -z "$GENHTML"; then
    AC_MSG_ERROR([Could not find genhtml from the lcov package])
  fi

  if test "$WANT_COVERAGE" == "yes"; then
    changequote({,})
    CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9]*//g'`
    changequote([,])

    # Add the special gcc flags
    COVERAGE_CFLAGS="--coverage -DDEBUG"
    COVERAGE_LDFLAGS="--coverage -lgcov"
  fi
]) # AX_TEST_COVERAGE

AC_DEFUN_ONCE([AX_PROG_CDOC],
  AC_CHECK_PROG(CDOC, cdoc, cdoc)
  AM_CONDITIONAL(HAVE_CDOC, test -n "$CDOC"))

#########################################################


AM_INIT_AUTOMAKE([foreign -Wall -Werror])
LT_INIT

AX_TEST_COVERAGE

AC_PROG_CC
AX_PROG_CDOC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile t/Makefile])

AC_OUTPUT
