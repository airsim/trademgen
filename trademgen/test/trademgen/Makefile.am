## test/trademgen sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = generateEvents
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

generateEvents_SOURCES = BasTypes.hpp \
						EventStream.hpp EventStream.cpp \
						RandomGeneration.hpp RandomGeneration.cpp \
						CategoricalAttribute.hpp CategoricalAttribute.cpp \
						generateEvents.cpp
generateEvents_CXXFLAGS= $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS)
generateEvents_LDADD = 
generateEvents_LDFLAGS = $(BOOST_LIBS) $(CPPUNIT_LIBS) \
	$(top_builddir)/extracppunit/libextracc-extracppunit.la \
	$(top_builddir)/trademgen/core/libtrademgen.la
