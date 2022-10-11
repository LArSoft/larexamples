/**
 * @file   ShowerCalibrationGaloreScale_test.cc
 * @brief  Tests ShowerCalibrationGaloreScale service provider
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGaloreScale.h
 *
 * Runs a test that instantiates a ShowerCalibrationGaloreScale provider with
 * a known configuration and verifies that the same factor is returned for
 * a nominal reconstructed shower.
 * It also prints on screen a "standard" table of corrections, as printed by
 * ShowerCalibrationTableTest().
 *
 * Command line arguments: none.
 *
 */

// LArSoft libraries
#include "larcorealg/TestUtils/unit_test_base.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreScale.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreScaleTestHelpers.h"
#include "test/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreTests.h"

// C/C++ standard libraries
#include <iostream>

//------------------------------------------------------------------------------
int main()
{

  //
  // prepare the test environment
  //
  lar::example::ShowerCalibrationGalore::Correction_t expected(1.02, 0.02);

  // provide a test name and a push a configuration for
  // "ShowerCalibrationGaloreService" ("service_provider" is inconsequential)
  testing::BasicEnvironmentConfiguration config("ShowerCalibrationGaloreScale_test");
  config.AddDefaultServiceConfiguration("ShowerCalibrationGaloreService", R"(
    service_provider: "ShowerCalibrationGaloreScaleService"
    factor: )" + std::to_string(expected.factor) + R"(
    error: )" + std::to_string(expected.error) + R"(
    )");

  // set up a basic testing environment with that configuration
  auto TesterEnv = testing::CreateTesterEnvironment(config);

  // set up a service provider
  // (ShowerCalibrationGaloreScale explicitly supports this one-step setup)
  TesterEnv.SimpleProviderSetup<lar::example::ShowerCalibrationGaloreScale>();

  //
  // computation of expected values
  //
  unsigned int nErrors = 0; // error count

  // get the provider we just set up (but accessing it by the interface)
  auto const* Calibration = TesterEnv.Provider<lar::example::ShowerCalibrationGalore>();

  //
  // run the test
  //
  nErrors += lar::example::tests::ShowerCalibrationTableTest(
    std::cout, Calibration, 0.0, 2.5, 0.1, {11, 13, -11, -13, 211, 111, 2112, 2212, 22});

  std::cout << Calibration->report() << std::endl;

  auto shower = lar::example::tests::MakeShower(1.0); // shower with 1 GeV
  auto corr = Calibration->correction(shower);
  if (corr != expected) {
    std::cerr << "Correction for a shower of energy " << shower.Energy().at(shower.best_plane())
              << " GeV is " << corr << ", expected " << expected << std::endl;
    ++nErrors;
  }
  auto corr_factor = Calibration->correctionFactor(shower);
  if (corr_factor != expected.factor) {
    std::cerr << "Correction factor for a shower of energy "
              << shower.Energy().at(shower.best_plane()) << " GeV is " << corr_factor
              << ", expected " << expected.factor << std::endl;
    ++nErrors;
  }

  return nErrors;
} // main()
