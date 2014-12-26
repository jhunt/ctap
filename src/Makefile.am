lib_LTLIBRARIES = libctap.la
libctap_la_SOURCES = ctap.h ctap.c
libctap_la_LDFLAGS = --version-info 0:1:0

include_HEADERS = ctap.h

if GCOV_ENABLED
AM_CFLAGS  = @GCOV_CFLAGS@
AM_LDFLAGS = @GCOV_LDFLAGS@
endif

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
