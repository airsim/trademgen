#
# Sequential generation tests for TraDemGen
#
module_set_name (sequential_generation)
module_library_add_specific (sequential_generation . "na" RandomManager.cpp)
##
# Specify each test suite configuration

# * RandomDistributionTest Test Suite
module_test_add_suite (RandomDistributionTest RandomDistributionTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

