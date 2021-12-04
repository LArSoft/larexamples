
cet_test(CreateTestShowerCalibrationFromPID_test
  SOURCES CreateTestShowerCalibrationFromPID.cxx CreateTestShowerCalibrationFromPID_test.cc
  LIBRARIES
    ROOT::Core
    ROOT::Hist
  )

cet_test(ShowerCalibrationGaloreFromPID_test
  SOURCES CreateTestShowerCalibrationFromPID.cxx ShowerCalibrationGaloreFromPID_test.cc
  LIBRARIES
    larexamples_Services_ShowerCalibrationGalore_Providers
    larcorealg_TestUtils
    messagefacility::MF_MessageLogger
    
    fhiclcpp
    cetlib::cetlib cetlib_except
    ROOT::Core
    ROOT::Hist
  )

cet_test(ShowerCalibrationGaloreScale_test
  LIBRARIES
    larexamples_Services_ShowerCalibrationGalore_Providers
    larcorealg_TestUtils
    messagefacility::MF_MessageLogger
    
    fhiclcpp
    cetlib::cetlib cetlib_except
    ROOT::Core
    ROOT::Hist
  )


# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
