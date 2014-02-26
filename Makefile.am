SUBDIRS=src . t

ACLOCAL_AMFLAGS=-I build/autotools


test: check
docs:
	cd src; make -f Makefile $@
version:
	@echo $(VERSION)
distfile:
	@echo $(PACKAGE)-$(VERSION).tar.gz

.PHONY: version docs test
