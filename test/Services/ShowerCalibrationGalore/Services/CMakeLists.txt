cet_build_plugin(ShowerCalibrationGaloreTest art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  larexamples::ShowerCalibrationGalore
  larexamples::ShowerCalibrationGaloreService
  larexamples::ShowerCalibrationGaloreTests
  art::Framework_Principal
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
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
