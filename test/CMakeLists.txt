# ======================================================================
#
# Testing
#
# ======================================================================

include(CetTest)
cet_enable_asserts()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(ROOT COMPONENTS Core Hist MathCore Physics RIO REQUIRED)

cet_transitive_paths(FHICL_DIR BINARY IN_TREE)
cet_test_env_prepend(FHICL_FILE_PATH ${TRANSITIVE_PATHS_WITH_FHICL_DIR})

cet_transitive_paths(LIBRARY_DIR BINARY IN_TREE)
cet_test_env_prepend(CET_PLUGIN_PATH ${TRANSITIVE_PATHS_WITH_LIBRARY_DIR})

cet_transitive_paths(GDML_DIR BINARY IN_TREE)
cet_test_env_prepend(FW_SEARCH_PATH ${TRANSITIVE_PATHS_WITH_GDML_DIR})

add_subdirectory(Services)
add_subdirectory(Algorithms)
