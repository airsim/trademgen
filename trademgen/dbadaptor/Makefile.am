## dba sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

noinst_LTLIBRARIES= libdba.la

libdba_la_SOURCES= $(dba_h_sources) $(dba_cc_sources) 
libdba_la_CXXFLAGS = $(CXXFLAGS_OPT) $(SOCI_CFLAGS)
libdba_la_LIBADD = $(SOCI_LIBS)

#pkgincludedir = $(includedir)/@PACKAGE@/dba
#pkginclude_HEADERS = $(dba_h_sources)
