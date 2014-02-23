lib_LTLIBRARIES = libctap.la
libctap_la_SOURCES = ctap.h ctap.c
libctap_la_LDFLAGS = --version-info o:0:0

if GCOV_ENABLED
AM_CFLAGS  = @GCOV_CFLAGS@
AM_LDFLAGS = @GCOV_LDFLAGS@
endif

###########################################

if HAVE_CDOC
docs:
	@rm -fr docs
	@mkdir -p docs
	cdoc --root docs -v *.c *.h
endif
