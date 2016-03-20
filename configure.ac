AC_PREREQ([2.63])
AC_INIT([ctap], [m4_esyscmd([./version.sh])], [bugs@niftylogic.com])
AC_CONFIG_SRCDIR([include/ctap.h])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AX_TEST_COVERAGE

AC_PROG_CC
AX_PROG_CDOC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
