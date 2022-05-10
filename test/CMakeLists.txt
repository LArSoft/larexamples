# ======================================================================
#
# Testing
#
# ======================================================================

include(CetTest)
cet_enable_asserts()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(ROOT COMPONENTS Core Hist MathCore Physics RIO REQUIRED)


add_subdirectory( Services )
add_subdirectory( Algorithms )

