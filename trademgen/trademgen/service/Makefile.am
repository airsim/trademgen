## command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

# Hack until the proper ax_flood.m4 be added to the config sub-directory
LATUS_DIR=/remote/projteams/projects/ori-data/deliveries/latus-99.99.99
FLOOD_INC_DIR=$(LATUS_DIR)/include

noinst_LTLIBRARIES= libsvc.la

libsvc_la_SOURCES= $(svc_h_sources) $(svc_cc_sources) 
libsvc_la_CXXFLAGS = $(SOCI_CFLAGS) $(BOOST_CFLAGS) \
		${PYTHON_CFLAGS} -I$(FLOOD_INC_DIR)

#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(svc_h_sources)
