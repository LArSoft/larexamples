/**
 * @file   CreateTestShowerCalibrationFromPID.h
 * @brief  Creates a test calibration file for ShowerCalibrationGaloreFromPID
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    CreateTestShowerCalibrationFromPID.cxx
 *
 * Provides in lar::example::tests namespace:
 *
 * * CreateTestShowerCalibrationFromPID()
 *
 */

#ifndef TEST_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_CREATETESTSHOWERCALIBRATIONFROMPID
#define TEST_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_CREATETESTSHOWERCALIBRATIONFROMPID

// C/C++ standard libraries
#include <string>

namespace lar {
  namespace example {
    namespace tests {

      /**
 * @brief Creates a test calibration file for ShowerCalibrationGaloreFromPID
 * @param outputPath UNIX + ROOT path for the output ROOT directory
 * @return an error code, 0 on success
 *
 * `outputPath` is a full ROOT directory path made of a UNIX path and a ROOT
 * directory path. For example, `"data/calibrations.root:Showers/ByType"` will
 * create a directory `data` and a `calibrations.root` ROOT file in it (or
 * update it if exists), create a structure of two nested ROOT directories,
 * `Showers/ByType`, and write all the calibration graphs in there.
 *
 * It currently writes:
 *
 * * `"Pi0"` (`TGraphErrors`):
 *   neutral pion calibration vs. reconstructed energy, [ 0; 2 ] GeV range
 * * `"Photon"` (`TGraphErrors`):
 *   photon calibration vs. reconstructed energy, [ 0; 2 ] GeV range
 * * `"Electron"` (`TGraphErrors`):
 *   electron/positron calibration vs. reconstructed energy, [ 0; 2 ] GeV range
 * * `"Muon"` (`TGraphErrors`):
 *   muon/antimuon calibration vs. reconstructed energy, [ 0; 2 ] GeV range
 * * `"Default"` (`TGraphErrors`):
 *   other particle calibration vs. reconstructed energy, [ 0; 5 ] GeV range
 *
 */
      int CreateTestShowerCalibrationFromPID(std::string outputPath);

    } // namespace tests
  }   // namespace example
} // namespace lar

#endif // TEST_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_CREATETESTSHOWERCALIBRATIONFROMPID
