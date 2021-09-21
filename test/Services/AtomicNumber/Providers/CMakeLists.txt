# simple test with hard-coded configurations
cet_test(AtomicNumber_test
   LIBRARIES
      larexamples_Services_AtomicNumber_Providers
      messagefacility::MF_MessageLogger
      
      fhiclcpp::fhiclcpp
      cetlib::cetlib cetlib_except
)

# test with external configuration (happens to use Boost)
cet_test(BoostedAtomicNumber_test
   DATAFILES ./test_krypton_atomic_number.fcl
   TEST_ARGS -- ./test_krypton_atomic_number.fcl
   LIBRARIES
      larexamples_Services_AtomicNumber_Providers
      messagefacility::MF_MessageLogger
      
      fhiclcpp::fhiclcpp
      cetlib::cetlib cetlib_except
   USE_BOOST_UNIT
   )

# tests just the configuration
cet_test(ValidateAtomicNumberConfiguration_test
   LIBRARIES
      larexamples_Services_AtomicNumber_Providers
      messagefacility::MF_MessageLogger
      
      fhiclcpp::fhiclcpp
      cetlib::cetlib cetlib_except
   DATAFILES ./test_krypton_atomic_number.fcl
   TEST_ARGS ./test_krypton_atomic_number.fcl
   )


# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
