## test/trademgen sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = DemandGenerationTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

DemandGenerationTestSuite_SOURCES = BasTypes.hpp \
	EventStream.hpp EventStream.cpp RandomGeneration.hpp RandomGeneration.cpp \
	CategoricalAttribute.hpp CategoricalAttribute.cpp \
	DemandGenerationTestSuite.hpp DemandGenerationTestSuite.cpp
DemandGenerationTestSuite_CXXFLAGS= $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS)
DemandGenerationTestSuite_LDADD = 
DemandGenerationTestSuite_LDFLAGS = $(BOOST_LIBS) $(CPPUNIT_LIBS) \
	$(top_builddir)/extracppunit/libextracc-extracppunit.la \
	$(top_builddir)/trademgen/core/libtrademgen.la
