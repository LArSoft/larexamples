# The short version: There's probably no reason to change this file.

# Medium version: There are a couple of reasons to change this
# CMakeLists.txt file or any of the others in the directories above
# it.

# 1) You need a major piece of functionality that's not included
#    implicitly in the list below. Typically you'd have to locate some
#    other package that also requires the same functionality and
#    compare its CMakeLists.txt file(s) with this one, then add the
#    necessary line(s).

# 2) There's a breaking change in the art/LArSoft packages. To be
#    aware of such changes, consider subscribing to the
#    larsoft@fnal.gov mailing list. Note that changes of this sort are
#    typically made by software librarians just before a particular
#    version tag, so if you've followed the directions on the
#    AnalysisExample web page (see README.md) then you'd only have to
#    make a change if you're updating your analysis package to a later
#    or development version of LArSoft.

# Long version:

# This file is used by the utility cmake <https://cmake.org/>, a
# utility for building and updating software. The cmake program is
# invoked as part of the "mrb install" procedure.

# Actually, this file is part of a chain of files:

# $MRB_SOURCE/CMakeLists.txt
# $MRB_SOURCE/${myProduct}/CMakeLists.txt
# $MRB_SOURCE/${myProduct}/${myProduct}/CMakeLists.txt
# $MRB_SOURCE/${myProduct}/${myProduct}/${myPackage}/CMakeLists.txt

# You can glance at them to get some idea of what's being invoked. You
# can try to look things up in the CMake documentation, which will
# give you some idea of the syntax. However, all CMake-based projects
# rapidly gain their own custom rules, so most of what you see in
# these files only has meaning in the context of art/LArSoft. You have
# to use detective work to determine the meaning of most of what you
# see in the CMakeLists.txt files; for example, in the "simple_plugin"
# below, you can see "cetlib_except::cetlib_except", which presumably means the
# cetlib::cetlib exception-handling library.

# The function of cmake is to create a Makefile for use by the "make"
# program; the "mrb iinstall" procedure runs "make" for you after it's
# run cmake. If you hunt around the $MRB_BUILDDIR directory, you'll
# see the work files (including Makefile(s)) created by the cmake/make
# build procedure.

# The following lines define AnalysisExample as a module (something
# that can be invoked as a procedure in a .fcl file) and the
# libraries/resources needed to compile/execute the module.

cet_build_plugin(AnalysisExample art::EDAnalyzer
  LIBRARIES PRIVATE
  larsim::Simulation
  lardata::DetectorClocksService
  larcore::Geometry_Geometry_service
  larcore::ServiceUtil
  lardataobj::RecoBase
  lardataobj::Simulation
  larcorealg::Geometry
  nusimdata::SimulationBase
  art_root_io::TFileService_service
  art::Framework_Services_Registry
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
  cetlib::cetlib
  ROOT::Hist
  ROOT::Physics
  ROOT::Tree
)

# The above will create libraries in $MRB_INSTALL; these are the ".so"
# files you saw if you followed the example on the AnalysisExample web
# page. The following lines specify additional files that are to be
# copied to $MRB_BUILDDIR and $MRB_INSTALL; for example, the
# documentation (.md) files.

# To get a sense of what gets built and installed, look at:
#   ls $MRB_INSTALL/larexamples/$LAREXAMPLES_VERSION

# Compare this with a package that you did not check out, but which is
# part of standard LArSoft; e.g.,
#   ls $LARSIM_DIR

# For a pretty-printed listing of where LArSoft binary programs are located:
#   echo $PATH | sed "s/:/\n/g" | less

# For a pretty-printed listing of which LArSoft shared libraries are searched:
#   echo $LD_LIBARY_PATH | sed "s/:/\n/g" | less

install_headers()
install_fhicl()
install_scripts()
install_source()
