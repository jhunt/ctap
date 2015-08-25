AC_INIT([libctap], [1.1.7], [bugs@niftylogic.com])
AC_PREREQ([2.63])
AC_CONFIG_SRCDIR([include/ctap.h])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# AM_PROG_AR is used for linker stuff, but only by newer Automake
# On older Automakes (<1.12 IIRC) it just complains during ./bootstrap
# so we use the ifndef trick here to expand if expandable.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

PKG_PROG_PKG_CONFIG

LT_INIT

AX_TEST_COVERAGE

AC_PROG_CC
AX_PROG_CDOC

# set pkgconfigdir, allow override
AC_ARG_WITH([pkgconfigdir],
    AS_HELP_STRING([--with-pkgconfigdir=PATH],
        [Path to the pkgconfig directory [[LIBDIR/pkgconfig]]]),
    [pkgconfigdir="$withval"],
    [pkgconfigdir='${libdir}/pkgconfig'])
AC_SUBST([pkgconfigdir])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/ctap.pc])

AC_OUTPUT
