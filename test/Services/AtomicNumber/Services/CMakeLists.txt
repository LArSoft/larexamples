simple_plugin(AtomicNumberTest "module"
   art::Framework_Core
   art::Framework_Core
   art::Framework_Services_Registry
   art::Utilities canvas::canvas
   messagefacility::MF_MessageLogger
   
   fhiclcpp::fhiclcpp
   cetlib::cetlib cetlib_except
)


cet_test(AtomicNumberService_test HANDBUILT
   DATAFILES ./test_krypton_atomic_number.fcl
   TEST_EXEC lar
   TEST_ARGS --rethrow-all --config ./test_krypton_atomic_number.fcl
   )


# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
