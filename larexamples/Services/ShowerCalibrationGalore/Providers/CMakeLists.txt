cet_make_library(LIBRARY_NAME ShowerCalibrationGalore INTERFACE
  SOURCE ShowerCalibrationGalore.h
  LIBRARIES INTERFACE
  lardataobj::RecoBase
  larcore::ServiceProviderWrappers
)

cet_make_library(SOURCE
  ShowerCalibrationGaloreFromPID.cxx
  ShowerCalibrationGaloreScale.cxx
  LIBRARIES
  PUBLIC
  larexamples::ShowerCalibrationGalore
  fhiclcpp::types
  fhiclcpp::fhiclcpp
  ROOT::Core
  ROOT::RIO
  ROOT::Hist
  ROOT::MathMore # FIXME: Uses GSL, which is GLPv3!
  PRIVATE
  cetlib::cetlib
  cetlib_except::cetlib_except
)

cet_make_library(LIBRARY_NAME ShowerCalibrationGaloreProviderTestHelpers INTERFACE
  SOURCE
  ShowerCalibrationGaloreFromPIDTestHelpers.h
  ShowerCalibrationGaloreScaleTestHelpers.h
  LIBRARIES INTERFACE
  larexamples::Services_ShowerCalibrationGalore_Providers
  larcorealg::ProviderTestHelpers
)

install_fhicl()
install_headers()

# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
