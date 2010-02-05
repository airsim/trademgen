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

DemandGenerationTestSuite_SOURCES = BasTypes.hpp BasConst.hpp \
	CategoricalAttribute.hpp \
	ContinuousAttribute.hpp \
	DemandCharacteristics.hpp DemandCharacteristics.cpp \
	DemandStream.hpp DemandStream.cpp \
	EventQueue.hpp EventQueue.cpp \
	EventStruct.hpp EventStruct.cpp \
	RandomGenerationContext.hpp RandomGenerationContext.cpp \
	RandomGeneration.hpp RandomGeneration.cpp \
	Request.hpp Request.cpp \
	DemandGenerationTestSuite.hpp DemandGenerationTestSuite.cpp
# test.cpp 
DemandGenerationTestSuite_CXXFLAGS= $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS)
DemandGenerationTestSuite_LDADD = 
DemandGenerationTestSuite_LDFLAGS = $(BOOST_LIBS) $(CPPUNIT_LIBS) \
	$(top_builddir)/extracppunit/libextracc-extracppunit.la \
	$(top_builddir)/trademgen/core/libtrademgen.la
