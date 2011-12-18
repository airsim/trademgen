#
# Sequential generation tests for TraDemGen
#

##
# Specify each test suite configuration

# * RandomDistributionTest Test Suite
module_test_add_suite (sequential_generation RandomDistributionTest 
  "RandomManager.cpp;RandomDistributionTestSuite.cpp")


##
# Register all the test suites to be built and performed
module_test_build_all ()

