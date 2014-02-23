dnl Profide a configure option to enable gcov compiler flags.
dnl Derived from gcov.m4 in the Gobject-Introspection repository.
AC_DEFUN([ENABLE_GCOV],
[
  AC_ARG_ENABLE(gcov,
    AC_HELP_STRING([--enable-gcov],
      [compile with coverage profiling instrumentation (gcc only)]),
    enable_gcov=$enableval,
    enable_gcov=no)
  if test x$enable_gcov = xyes
  then
    if test "x$GCC" != "xyes"
    then
      AC_MSG_ERROR([gcov only works if gcc is used])
    fi

    AS_COMPILER_FLAGS(GCOV_CFLAGS, "-fprofile-arcs -ftest-coverage")
    dnl libtool 1.5.22 and lower strip -fprofile-arcs from the flags
    dnl passed to the linker, which is a bug; -fprofile-arcs implicitly
    dnl links in -lgcov, so we do it explicitly here for the same effect
    GCOV_LDFLAGS=-lgcov
    AC_SUBST(GCOV_CFLAGS)
    AC_SUBST(GCOV_LDFLAGS)
    GCOV=`echo $CC | sed s/gcc/gcov/g`
    AC_SUBST(GCOV)

    GCOV_ENABLED=yes
    AC_DEFINE_UNQUOTED(GCOV_ENABLED, 1,
      [Defined if gcov is enabled to force a rebuild due to config.h changing])

    # Force the user to turn off optimization
    AC_MSG_NOTICE([gcov enabled, adding "-g -O0" to CFLAGS])
    AS_COMPILER_FLAGS(CFLAGS, "-g -O0")
  fi
  AM_CONDITIONAL(GCOV_ENABLED, test x$enable_gcov = xyes)
])
