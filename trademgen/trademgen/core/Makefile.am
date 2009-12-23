include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =

# Library
lib_LTLIBRARIES = libtrademgen.la

libtrademgen_la_SOURCES = $(service_h_sources) $(service_cc_sources)
libtrademgen_la_LIBADD = \
	$(top_builddir)/trademgen/basic/libbas.la \
	$(top_builddir)/trademgen/bom/libbom.la \
	$(top_builddir)/trademgen/factory/libfac.la \
	$(top_builddir)/trademgen/dbadaptor/libdba.la \
	$(top_builddir)/trademgen/command/libcmd.la \
	$(top_builddir)/trademgen/service/libsvc.la
libtrademgen_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_PYTHON_LIB) \
	$(SOCI_LIBS) -version-info $(GENERIC_LIBRARY_VERSION)

# Header files
pkginclude_HEADERS = $(service_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/trademgen/config.h
