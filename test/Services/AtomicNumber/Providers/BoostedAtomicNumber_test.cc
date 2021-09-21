/**
 * @file   BoostedAtomicNumber_test.cc
 * @brief  Tests the AtomicNumber service provider (with Boost unit test)
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 14, 2016
 * @see    AtomicNumber.h
 * @ingroup AtomicNumber
 *
 *
 * This test takes a configuration file as first command line argument.
 * The content of the configuration file is expected to include:
 * - a `services.AtomicNumberService` section with the configuration of the
 *   provider
 * - a `physics.analyzers.expected` section with the expected values (same
 *   format as the service configuration)
 *
 */


// LArSoft libraries
#define BOOST_TEST_MODULE (Boosted atomic number test)
#include "larexamples/Services/AtomicNumber/Providers/AtomicNumber.h"
#include "larcorealg/TestUtils/unit_test_base.h" // testing::TesterEnvironment
#include "larcorealg/TestUtils/boost_unit_test_base.h" // testing::BoostCommandLineConfiguration

// BEGIN AtomicNumber ----------------------------------------------------------
/// @ingroup AtomicNumber
/// @{

//------------------------------------------------------------------------------
//--- Testing environment
//---
using BoostBasicFixture = testing::TesterEnvironment<
   testing::BoostCommandLineConfiguration
      <testing::BasicEnvironmentConfiguration>
   >;

struct AtomicNumberTestFixture: private BoostBasicFixture {

   // Constructor (to give a name to the test)
   AtomicNumberTestFixture()
      : BoostBasicFixture( "BoostedAtomicNumber_test" )
      , Zprov(ServiceParameters("AtomicNumberService"))
      , pset_expected(TesterParameters("expected"))
      {}

   // public provider
   lar::example::AtomicNumber const Zprov;

   // expected values (as a parameter set)
   fhicl::ParameterSet const pset_expected;

}; // AtomicNumberTestFixture


//------------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(BoostedAtomicNumberTest, AtomicNumberTestFixture)

BOOST_AUTO_TEST_CASE(AllTests) {

   // these are the results we expect:
   auto const expected_Z = pset_expected.get<unsigned int>("AtomicNumber");

   // here goes the test... Zprov is the data member of the fixture
   BOOST_CHECK_EQUAL(Zprov.Z(), expected_Z);

} // BOOST_AUTO_TEST_CASE(AllTests)

BOOST_AUTO_TEST_SUITE_END()

/// @}
// END AtomicNumber ------------------------------------------------------------
