## python sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

# Library
lib_LTLIBRARIES = libpytrademgen.la

libpytrademgen_la_SOURCES = pytrademgen.cpp
libpytrademgen_la_CXXFLAGS = $(PYTHON_CFLAGS) $(BOOST_CFLAGS)
libpytrademgen_la_LIBADD = $(top_builddir)/trademgen/core/libtrademgen.la
libpytrademgen_la_LDFLAGS = $(PYTHON_LIBS) $(PYTHON_ADD_LIBS) \
	$(BOOST_PYTHON_LIB) -version-info $(GENERIC_LIBRARY_VERSION)

EXTRA_DIST = pytrademgen.py
