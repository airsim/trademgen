#
# Tests for TraDemGen
#

# Subdirs
add_subdirectory (${PROJECT_NAME})
add_subdirectory (sequential_generation)

if (Boost_FOUND)

  add_custom_target (check
    DEPENDS checkTrademgenTest checkRandomDistributionTest)

endif()