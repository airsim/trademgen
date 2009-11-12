include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =

# Temporary until there is an ax_flood.m4 integrated within the config
# directory
FLOOD_DIR=/remote/projteams/projects/ori-data/deliveries/latus-99.99.99

# Library
lib_LTLIBRARIES = libforecast.la

libforecast_la_SOURCES = $(service_h_sources) $(service_cc_sources)
libforecast_la_LIBADD = \
	$(top_builddir)/forecast/basic/libbas.la \
	$(top_builddir)/forecast/bom/libbom.la \
	$(top_builddir)/forecast/factory/libfac.la \
	$(top_builddir)/forecast/dbadaptor/libdba.la \
	$(top_builddir)/forecast/command/libcmd.la \
	$(top_builddir)/forecast/service/libsvc.la
libforecast_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_PYTHON_LIB) \
	-L$(FLOOD_DIR)/lib -lFlood \
	$(SOCI_LIBS) -version-info $(GENERIC_LIBRARY_VERSION)

# Header files
pkginclude_HEADERS = $(service_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/forecast/config.h
