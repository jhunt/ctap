#
# Makefile.am - ctap Automake File
#

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I build

############################################################

AM_CFLAGS = -I$(srcdir)/include -Wall $(DEPS_CFLAGS)
AM_CFLAGS += @GCOV_CFLAGS@

############################################################

include_HEADERS = include/ctap.h

###
#
# COMPILED_TESTS are compiled C and they can be run
# by the Autotools test harness directly (they should
# all pass without outside intervention)
#
###

COMPILED_TESTS =

COMPILED_TESTS    += t/asserts
t_asserts_SOURCES  = t/asserts.c include/ctap.h

COMPILED_TESTS += t/skip
t_skip_SOURCES  = t/skip.c include/ctap.h

COMPILED_TESTS += t/todo
t_todo_SOURCES  = t/todo.c include/ctap.h

###
#
# AUX_TESTS are compiled C, but behave in such a way
# that they need a Perl driver (in PERL_TESTS) to handle
# the output (they fail, they timeout, and otherwise
# behave badly).
#
###

AUX_TESTS      =
PERL_TESTS     =

AUX_TESTS  += t/fail.aux
PERL_TESTS += t/fail.pl
t_fail_aux_SOURCES = t/fail.aux.c

AUX_TESTS  += t/bail.aux
PERL_TESTS += t/bail.pl
t_bail_aux_SOURCES = t/bail.aux.c

AUX_TESTS  += t/premature.aux
PERL_TESTS += t/premature.pl
t_premature_aux_SOURCES = t/premature.aux.c

AUX_TESTS  += t/output.aux
PERL_TESTS += t/output.pl
t_output_aux_SOURCES = t/output.aux.c

AUX_TESTS  += t/stack.aux
PERL_TESTS += t/stack.pl
t_stack_aux_SOURCES = t/stack.aux.c

AUX_TESTS  += t/printf.aux
PERL_TESTS += t/printf.pl
t_printf_aux_SOURCES = t/printf.aux.c

AUX_TESTS  += t/fork.aux
PERL_TESTS += t/fork.pl
t_fork_aux_SOURCES = t/fork.aux.c


check_PROGRAMS = $(COMPILED_TESTS) $(AUX_TESTS)
TESTS = $(COMPILED_TESTS) $(PERL_TESTS)
EXTRA_DIST = t/extap t/untap t/expect t/skips $(PERL_TESTS)

###########################################

docs:
if HAVE_CDOC
	@rm -fr docs
	@mkdir -p docs
	cdoc --root docs -v *.c *.h
endif
.PHONY: docs

clean-local:
	@rm -fr docs

test: check
coverage:
version:
	@echo $(VERSION)
distfile:
	@echo $(PACKAGE)-$(VERSION).tar.gz

.PHONY: distfile version docs test
