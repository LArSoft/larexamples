cet_build_plugin(AtomicNumberTest art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  larexamples::Services_AtomicNumber_Services_AtomicNumberService_service
  larexamples::AtomicNumber
  fhiclcpp::fhiclcpp
  canvas::canvas
)

cet_test(AtomicNumberService_test HANDBUILT
   DATAFILES ./test_krypton_atomic_number.fcl
   TEST_EXEC lar
   TEST_ARGS --rethrow-all --config ./test_krypton_atomic_number.fcl
   )
