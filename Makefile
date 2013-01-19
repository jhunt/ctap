#
#  Copyright 2011 James Hunt <james@jameshunt.us>
#
#  This file is part of ctest - a C Unit Testing Framework.
#
#  ctest is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ctest is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with ctest.  If not, see <http://www.gnu.org/licenses/>.
#

#################################################################

CFLAGS  := -fPIC -g -Wall -lc -Wunused -I.
COVER   := -fprofile-arcs -ftest-coverage

LCOV    := lcov --directory . --base-directory .
GENHTML := genhtml --prefix $(shell dirname `pwd`)
CDOC    := cdoc

VERSION := 1.0.1
SOVERS  := 1.0
SONAME  := libctest.so
LIBDIR  := /usr/lib
SOVNAME := $(SONAME).$(SOVERS)
DISTDIR := libctest$(SOVERS)-$(VERSION)

test_o := test/assertions.o test/base.o

#################################################################

default: libctest.so

install: default
	install -d -o root -g root -m 0755 $(DESTDIR)$(LIBDIR)
	install -o root -g root -m 0644 $(SONAME).$(VERSION) $(DESTDIR)$(LIBDIR)/$(SOVNAME)
	install -d -o root -g root -m 0755 $(DESTDIR)/usr/include
	install -o root -g root -m 0644 ctest.h $(DESTDIR)/usr/include

.PHONY: clean test
clean:
	rm -f *.o test/*.o lcov.info lib*.so* test/run
	find . -name '*.gc??' | xargs rm -f
	rm -f libctest*.tar.gz libctest*.deb
	rm -rf doc/api/* doc/coverage/*

test: test/run
	@LD_LIBRARY_PATH=. test/run
	@LD_LIBRARY_PATH=. test/run -v
	@LD_LIBRARY_PATH=. test/run -q
	@LD_LIBRARY_PATH=. test/run -q base
	@LD_LIBRARY_PATH=. test/run -q unknown-test

coverage:
	$(LCOV) --capture -o $@.tmp
	$(LCOV) --remove $@.tmp > lcov.info
	rm -f $@.tmp
	rm -rf doc/coverage
	mkdir -p doc
	$(GENHTML) -o doc/coverage lcov.info

apidocs:
	mkdir -p doc/api
	rm -rf doc/api/*
	$(CDOC) --root doc/api *.c *.h -v

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp *.c *.h $(DISTDIR)
	cp Makefile $(DISTDIR)
	tar -czvf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)


#################################################################

test/run: test/run.o ctest-int.o $(test_o)
	$(CC) $(LDFLAGS) $(COVER) -o $@ $+

ctest-int.o: ctest.c ctest.h
	$(CC) $(CFLAGS)  $(COVER) -DCTEST_TEST_SUITES -c -o $@ $<

libctest.so: ctest.o
	$(CC) -shared -Wl,-soname,$(SOVNAME) -o $@.$(VERSION) $+
	ln -sf $@.$(VERSION) $@


