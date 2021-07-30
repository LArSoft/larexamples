simple_plugin(ShowerCalibrationGaloreTest "module"
  lardataobj_RecoBase
  art_Framework_Core
  art::Framework_Services_Registry
  art::Utilities canvas::canvas
  mf_MessageLogger
  fhiclcpp
  cetlib::cetlib cetlib_except 
)


cet_test(ShowerCalibrationGaloreScaleTest HANDBUILT
   DATAFILES ./test_shower_calibration_scale.fcl
   TEST_EXEC lar
   TEST_ARGS --rethrow-all --config ./test_shower_calibration_scale.fcl
   )

cet_test(ShowerCalibrationGaloreFromPIDTest HANDBUILT
   DATAFILES ./test_shower_calibration_from_pid.fcl
   TEST_EXEC lar
   TEST_ARGS --rethrow-all --config ./test_shower_calibration_from_pid.fcl
   )


# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
