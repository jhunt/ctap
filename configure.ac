AC_INIT([libctap], [1.1.2], [bugs@niftylogic.com])
AC_PREREQ([2.65])
AC_CONFIG_SRCDIR([src/ctap.h])
AC_CONFIG_AUX_DIR([build/autotools])
AC_CONFIG_MACRO_DIR([build/autotools])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

dnl AM_PROG_AR is needed for some linker stuff
AM_PROG_AR

LT_INIT

AX_TEST_COVERAGE

AC_PROG_CC
AX_PROG_CDOC

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile t/Makefile])

AC_OUTPUT
