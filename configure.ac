AC_INIT([ctest], [1.0.1], [bugs@niftylogic.com])
AC_PREREQ([2.65])
AC_CONFIG_SRCDIR([src/ctest.h])
AC_CONFIG_AUX_DIR([build/autotools])
AC_CONFIG_MACRO_DIR([build/autotools])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
LT_INIT

#AX_TEST_COVERAGE
ENABLE_GCOV
#AM_CONDITIONAL([GCOV_ENABLED], test "xno" == "xyes")
#AM_CONDITIONAL([HAVE_CDOC], test "xno" == "xyes")

AC_PROG_CC
AX_PROG_CDOC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile t/Makefile])

AC_OUTPUT
