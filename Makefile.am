SUBDIRS=src . t

ACLOCAL_AMFLAGS=-I build/autotools


test: check
docs:
	cd src; make -f Makefile $@

.PHONY: docs test
