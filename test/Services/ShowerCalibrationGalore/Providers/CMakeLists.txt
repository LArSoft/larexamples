cet_make_library(LIBRARY_NAME ShowerCalibrationGaloreTests INTERFACE
  NO_INSTALL
  SOURCE ShowerCalibrationGaloreTests.h
  LIBRARIES INTERFACE
  larexamples::ShowerCalibrationGalore
  lardataobj::RecoBase
)

cet_make_library(LIBRARY_NAME CreateTestShowerCalibrationFromPID
  NO_INSTALL
  SOURCE
  CreateTestShowerCalibrationFromPID.cxx
  LIBRARIES PRIVATE
  ROOT::Core
  ROOT::RIO
  ROOT::Hist
  ROOT::MathCore
)

cet_test(CreateTestShowerCalibrationFromPID_test
  SOURCE
  CreateTestShowerCalibrationFromPID_test.cc
  LIBRARIES PRIVATE
  larexamples::CreateTestShowerCalibrationFromPID
)

cet_test(ShowerCalibrationGaloreFromPID_test
  LIBRARIES PRIVATE
  larexamples::CreateTestShowerCalibrationFromPID
  larexamples::ShowerCalibrationGaloreTests
  larexamples::ShowerCalibrationGaloreProviderTestHelpers
  larcorealg::unit_test_base
  messagefacility::MF_MessageLogger
)

cet_test(ShowerCalibrationGaloreScale_test
  LIBRARIES PRIVATE
  larexamples_Services_ShowerCalibrationGalore_Providers
  larexamples::ShowerCalibrationGaloreProviderTestHelpers
  larcorealg::unit_test_base
  messagefacility::MF_MessageLogger
)
