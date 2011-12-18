#
# TraDemGen Tests
#

##
# Specify each test suite configuration

# * TraDemGen Test Suite
module_test_add_suite (trademgen TrademgenTest DemandGenerationTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

