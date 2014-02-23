changecom('#')
#
# AX_PROG_CDOC
#
# author:  James Hunt <james@niftylogic.com>
# created: 2014-02-23
#
# Look for the cdoc utility for generating API documentation
# from C comment blocks.
#
AC_DEFUN_ONCE([AX_PROG_CDOC],
[
  AC_CHECK_PROG(CDOC, cdoc, cdoc)
  AM_CONDITIONAL(HAVE_CDOC, test "x$CDOC" != "")
])
