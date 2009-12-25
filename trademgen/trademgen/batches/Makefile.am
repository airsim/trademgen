include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = trademgen

trademgen_SOURCES = $(batches_fct_h_sources) $(batches_fct_cc_sources)
trademgen_CXXFLAGS = $(BOOST_CFLAGS)
#trademgen_LDADD =
trademgen_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(top_builddir)/trademgen/core/libtrademgen.la
