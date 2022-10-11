/**
 * @file   AtomicNumber_test.cc
 * @brief  Tests the AtomicNumber service provider
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 13, 2016
 * @see    AtomicNumber.h
 * @ingroup AtomicNumber
 *
 * The exit code of the test is the number of triggered errors.
 * This text is expected to pass with 0 errors.
 *
 * This test does not exercise the configuration via file.
 *
 */

// LArSoft libraries
#include "larcorealg/TestUtils/unit_test_base.h" // testing namespace
#include "larexamples/Services/AtomicNumber/Providers/AtomicNumber.h"

// support libraries
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <string>

// BEGIN AtomicNumber ----------------------------------------------------------
/// @ingroup AtomicNumber
/// @{

/// Structure to hold enough information to computed the expected results
struct Results_t {
  unsigned int Z; ///< atomic number
};                // Results_t

//------------------------------------------------------------------------------
/**
 * @fn TestConfiguration
 * @brief Runs a test with a specific configuration
 * @param testName name of this test for output purposes
 * @param configuration provider configuration as a string in FHiCL format
 * @param expected the expected values
 * @return number of errors encountered
 */
unsigned int TestConfiguration(std::string testName,
                               std::string configuration,
                               Results_t const& expected)
{

  //
  // configuration of the test
  //

  // provide a test name and a push a configuration for "AtomicNumberService"
  testing::BasicEnvironmentConfiguration config(testName);
  config.AddDefaultServiceConfiguration("AtomicNumberService", configuration);

  // set up a basic testing environment with that configuration
  auto TesterEnv = testing::CreateTesterEnvironment(config);

  //
  // computation of expected values
  //
  unsigned int nErrors = 0; // error count

  // create a new service provider with configuration from the environment
  lar::example::AtomicNumber Zprov(TesterEnv.ServiceParameters("AtomicNumberService"));

  //
  // here goes the test...
  //
  unsigned int Z = Zprov.Z();
  if (Z != expected.Z) {
    mf::LogError("AtomicNumber_test")
      << testName << ": wrong atomic number: " << Z << " (expected: " << expected.Z << ")";
    ++nErrors;
  }

  //
  // done!
  //
  return nErrors;
} // TestConfiguration()

//------------------------------------------------------------------------------
unsigned int TestDefaultConfiguration()
{
  Results_t expected;
  expected.Z = 18;

  return TestConfiguration("TestDefaultConfiguration", "", expected);

} // TestDefaultConfiguration()

//------------------------------------------------------------------------------
unsigned int TestXenonConfiguration()
{

  Results_t expected;
  expected.Z = 54;

  return TestConfiguration("TestXenonConfiguration", // test name
                           R"(
         AtomicNumber: 54
      )",                                            // provider configuration
                           expected                  // expected values
  );
} // TestXenonConfiguration()

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{

  unsigned int nErrors = 0;
  nErrors += TestDefaultConfiguration();
  nErrors += TestXenonConfiguration();

  return nErrors;
} // main()

/// @}
// END AtomicNumber ------------------------------------------------------------
