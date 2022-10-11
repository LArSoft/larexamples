/**
 * @file   ShowerCalibrationGaloreScaleTestHelpers.h
 * @brief  Specialization of helper classes for LArSoft tests
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 11, 2016
 * @see    ShowerCalibrationGaloreScale.h
 * @ingroup ShowerCalibrationGalore
 *
 * This is a header-only library that does not require additional linking.
 *
 */

#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALETESTHELPERS_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALETESTHELPERS_H

/// LArSoft libraries
#include "larcorealg/TestUtils/ProviderTestHelpers.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreScale.h"

// C/C++ standard libraries
#include <string>

namespace testing {

  // BEGIN ShowerCalibrationGalore ---------------------------------------------
  /// @ingroup ShowerCalibrationGalore
  /// @{

  // instantiation of the standard ProviderSetupClass for this provider
  template struct ProviderSetupClass<lar::example::ShowerCalibrationGaloreScale>;

  /**
   * @brief Environment setup helper for ShowerCalibrationGaloreScale
   * @tparam TestEnv type of environment to set up
   * @see SimpleEnvironmentSetupClass, simpleEnvironmentSetup
   *
   * A service provider is set up in the environment, associated with the types
   * lar::example::ShowerCalibrationGaloreScale.
   * Its configuration is read from "services.ShowerCalibrationGaloreService".
   *
   * The environment is expected to expose an interface equivalent to the one
   * of `testing::TesterEnvironment`.
   *
   * This class specialisation enables the support of `SimpleProviderSetup()`
   * methods of `testing::TesterEnvironment`.
   * It should be possible to set up a testing environment by calling:
   *
   *     env.SimpleProviderSetup<lar::example::ShowerCalibrationGaloreScale>();
   *
   * The provider will be available from any of these two calls:
   *
   *     env.Provider<lar::example::ShowerCalibrationGaloreScale>();
   *     env.Provider<lar::example::ShowerCalibrationGalore>();
   *
   */
  template <typename TestEnv>
  struct SimpleEnvironmentSetupClass<lar::example::ShowerCalibrationGaloreScale, TestEnv> {
    static lar::example::ShowerCalibrationGaloreScale* setup(TestEnv& env)
    {
      return SimpleEnvironmentStandardSetupByName<
        lar::example::ShowerCalibrationGaloreScale,      // provider class
        lar::example::ShowerCalibrationGalore,           // provider interface
        TestEnv>(env, "ShowerCalibrationGaloreService"); // configuration key
    }
  }; // SimpleEnvironmentSetupClass<ShowerCalibrationGaloreScale>

  /// @}
  // END ShowerCalibrationGalore -----------------------------------------------

} // namespace testing

#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALETESTHELPERS_H
