# simple test with hard-coded configurations
cet_test(AtomicNumber_test
  LIBRARIES PRIVATE
  larexamples::AtomicNumber
  larcorealg::unit_test_base
  messagefacility::MF_MessageLogger
)

# test with external configuration (happens to use Boost)
cet_test(BoostedAtomicNumber_test USE_BOOST_UNIT
  DATAFILES ./test_krypton_atomic_number.fcl
  TEST_ARGS -- ./test_krypton_atomic_number.fcl
  LIBRARIES PRIVATE
  larexamples::AtomicNumber
  larcorealg::unit_test_base
)

# tests just the configuration
cet_test(ValidateAtomicNumberConfiguration_test
  LIBRARIES PRIVATE
  larexamples::AtomicNumber
  larcorealg::unit_test_base
  fhiclcpp::types
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
