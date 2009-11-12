include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = forecast

forecast_SOURCES = $(batches_fct_h_sources) $(batches_fct_cc_sources)
forecast_CXXFLAGS = $(BOOST_CFLAGS) ${PYTHON_CFLAGS}
#forecast_LDADD =
forecast_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(SOCI_LIBS) \
	$(top_builddir)/forecast/core/libforecast.la
