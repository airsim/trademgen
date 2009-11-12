## python sub-directory
include $(top_srcdir)/Makefile.common

# Hack until the proper ax_flood.m4 be added to the config sub-directory
LATUS_DIR=/remote/projteams/projects/ori-data/deliveries/latus-99.99.99
FLOOD_INC_DIR=$(LATUS_DIR)/include

MAINTAINERCLEANFILES = Makefile.in

# Library
lib_LTLIBRARIES = libpyforecast.la

libpyforecast_la_SOURCES = pyforecast.cpp
libpyforecast_la_CXXFLAGS = ${PYTHON_CFLAGS} ${BOOST_CFLAGS} -I$(FLOOD_INC_DIR)
libpyforecast_la_LIBADD = $(top_builddir)/forecast/core/libforecast.la
libpyforecast_la_LDFLAGS = ${PYTHON_LIBS} ${PYTHON_ADD_LIBS} \
	${BOOST_PYTHON_LIB} -version-info $(GENERIC_LIBRARY_VERSION)

EXTRA_DIST = pyforecast.py
