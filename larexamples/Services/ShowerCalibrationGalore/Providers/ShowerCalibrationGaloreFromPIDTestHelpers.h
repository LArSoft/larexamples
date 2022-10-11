/**
 * @file   ShowerCalibrationGaloreFromPIDTestHelpers.h
 * @brief  Specialization of helper classes for LArSoft tests
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 11, 2016
 * @see    ShowerCalibrationGaloreFromPID.h
 * @ingroup ShowerCalibrationGalore
 *
 * This is a header-only library that does not require additional linking.
 *
 */

#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPIDTESTHELPERS_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPIDTESTHELPERS_H

/// LArSoft libraries
#include "larcorealg/TestUtils/ProviderTestHelpers.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreFromPID.h"

// C/C++ standard libraries
#include <string>

namespace testing {

  // BEGIN ShowerCalibrationGalore ---------------------------------------------
  /// @ingroup ShowerCalibrationGalore
  /// @{

  // instantiation of the standard ProviderSetupClass for this provider
  template struct ProviderSetupClass<lar::example::ShowerCalibrationGaloreFromPID>;

  /**
   * @brief Environment setup helper for ShowerCalibrationGaloreFromPID
   * @tparam TestEnv type of environment to set up
   * @see SimpleEnvironmentSetupClass, simpleEnvironmentSetup
   *
   * A service provider is set up in the environment, associated with the types
   * lar::example::ShowerCalibrationGaloreFromPID.
   * Its configuration is read from "services.ShowerCalibrationGaloreService".
   *
   * The environment is expected to expose an interface equivalent to the one
   * of `testing::TesterEnvironment`.
   *
   * This class specialisation enables the support of `SimpleProviderSetup()`
   * methods of `testing::TesterEnvironment`.
   * It should be possible to set up a testing environment by calling:
   *
   *     env.SimpleProviderSetup<lar::example::ShowerCalibrationGaloreFromPID>();
   *
   * The provider will be available from any of these two calls:
   *
   *     env.Provider<lar::example::ShowerCalibrationGaloreFromPID>();
   *     env.Provider<lar::example::ShowerCalibrationGalore>();
   *
   *
   */
  template <typename TestEnv>
  struct SimpleEnvironmentSetupClass<lar::example::ShowerCalibrationGaloreFromPID, TestEnv> {
    static lar::example::ShowerCalibrationGaloreFromPID* setup(TestEnv& env)
    {
      return SimpleEnvironmentStandardSetupByName<
        lar::example::ShowerCalibrationGaloreFromPID,    // provider class
        lar::example::ShowerCalibrationGalore,           // providet interface
        TestEnv>(env, "ShowerCalibrationGaloreService"); // configuration key
    }
  }; // SimpleEnvironmentSetupClass<ShowerCalibrationGaloreFromPID>

  /// @}
  // END ShowerCalibrationGalore -----------------------------------------------

} // namespace testing

#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPIDTESTHELPERS_H
