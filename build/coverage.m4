changecom('#')
#
# AX_TEST_COVERAGE
#
# Allow the user to optionally run the unit tests with
# coverage gathering enabled (gcov only) and produce
# fancy HTML reports with lcov/genhtml.
#
# This requires some additional tweaks to the t/Makefile.am,
# to support additional `make check` targets.
#
AC_DEFUN([AX_TEST_COVERAGE],
[
  AC_ARG_ENABLE(coverage,
    AC_HELP_STRING([--enable-coverage],
      [compile with coverage profiling instrumentation (gcc only)]),
    enable_coverage=$enableval,
    enable_coverage=no)
  if test x$enable_coverage = xyes
  then
    if test "x$GCC" != "xyes"
    then
      AC_MSG_ERROR([gcov only works if gcc is used])
    fi

    AS_COMPILER_FLAGS(GCOV_CFLAGS, "-fprofile-arcs -ftest-coverage")
    GCOV_LDFLAGS=-lgcov
    AC_SUBST(GCOV_CFLAGS)
    AC_SUBST(GCOV_LDFLAGS)
    GCOV=`echo $CC | sed s/gcc/gcov/g`
    AC_SUBST(GCOV)

    GCOV_ENABLED=yes
    AC_DEFINE_UNQUOTED(GCOV_ENABLED, 1,
      [Defined if gcov is enabled; forces rebuild due to config.h changing])

    AC_MSG_NOTICE([gcov code coverage enabled, adding "-g -O0" to CFLAGS])
    AS_COMPILER_FLAGS(CFLAGS, "-g -O0")
  fi
  AM_CONDITIONAL(GCOV_ENABLED, test x$enable_coverage = xyes)
])
