# ======================================================================
#  larexamples main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/larexamples/ups/setup_for_development <-d|-p>
#  buildtool [-I /install/path] -bt[i] -jN
# ======================================================================

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

find_package(cetmodules REQUIRED)
project(larexamples VERSION 10.00.15 LANGUAGES CXX)

# cetbuildtools contains our cmake modules

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs
)

cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(larcore REQUIRED EXPORT)
find_package(larcorealg REQUIRED EXPORT)
find_package(larcoreobj REQUIRED)
find_package(lardata REQUIRED EXPORT)
find_package(lardataobj REQUIRED EXPORT)
find_package(larsim REQUIRED)

find_package(nusimdata REQUIRED EXPORT)

find_package(art_root_io REQUIRED)
find_package(art REQUIRED EXPORT)
find_package(messagefacility REQUIRED)
find_package(canvas_root_io REQUIRED)
find_package(canvas REQUIRED EXPORT)
find_package(fhiclcpp REQUIRED EXPORT)
find_package(cetlib REQUIRED)
find_package(cetlib_except)

find_package(ROOT COMPONENTS Core EG Hist MathMore Physics RIO Tree REQUIRED EXPORT)

# macros for dictionary and simple_plugin
include(BuildDictionary)
include(ArtMake)
include(BuildPlugins)

cet_cmake_module_directories(Modules BINARY)

# source
add_subdirectory(larexamples)
add_subdirectory(test)

# ups - table and config files

# packaging utility


cet_cmake_config()
