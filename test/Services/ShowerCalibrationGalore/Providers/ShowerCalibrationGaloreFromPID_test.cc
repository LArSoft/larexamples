/**
 * @file   ShowerCalibrationGaloreFromPID_test.cc
 * @brief  Tests ShowerCalibrationGaloreFromPID service provider
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGaloreFromPID.h
 *
 * Runs a test that instantiates a ShowerCalibrationGaloreFromPID provider with
 * a known configuration and verifies that the same factor is returned for
 * a nominal reconstructed shower.
 * It also prints on screen a "standard" table of corrections, as printed by
 * ShowerCalibrationTableTest().
 *
 */

// LArSoft libraries
#include "larcorealg/TestUtils/unit_test_base.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreFromPID.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreFromPIDTestHelpers.h"
#include "test/Services/ShowerCalibrationGalore/Providers/CreateTestShowerCalibrationFromPID.h"
#include "test/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreTests.h"

// utility libraries
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <string>
#include <vector>

//------------------------------------------------------------------------------
int main()
{

  //
  // prepare the test environment
  //

  // create a test calibration file on the spot
  std::string const CalibrationFullPath = "TestCalibration.root:Showers";
  lar::example::tests::CreateTestShowerCalibrationFromPID(CalibrationFullPath);

  // provide a test name and a push a configuration for
  // "ShowerCalibrationGaloreService" ("service_provider" is inconsequential)
  testing::BasicEnvironmentConfiguration config("ShowerCalibrationGaloreFromPID_test");
  config.AddDefaultServiceConfiguration("ShowerCalibrationGaloreService", R"(
    service_provider: "ShowerCalibrationGaloreFromPIDService"
    CalibrationFile: ")" + CalibrationFullPath + R"("
    )");

  // set up a basic testing environment with that configuration
  auto TesterEnv = testing::CreateTesterEnvironment(config);

  // set up a service provider
  // (ShowerCalibrationGaloreFromPID explicitly supports this one-step setup)
  TesterEnv.SimpleProviderSetup<lar::example::ShowerCalibrationGaloreFromPID>();

  //
  // computation of expected values
  //
  unsigned int nErrors = 0; // error count

  // get the provider we just set up (but accessing it by the interface)ß
  auto const* Calibration = TesterEnv.Provider<lar::example::ShowerCalibrationGalore>();

  //
  // run the test
  //
  nErrors += lar::example::tests::ShowerCalibrationTableTest(
    mf::LogVerbatim("ShowerCalibrationGaloreFromPID_test"),
    Calibration,
    0.0,
    2.5,
    0.1,
    {11, 13, -11, -13, 211, 111, 2112, 2212, 22});

  mf::LogInfo("ShowerCalibrationGaloreFromPID_test") << Calibration->report() << std::endl;

  return nErrors;
} // main()
