/**
 * @file   CreateTestShowerCalibrationFromPID_test.cc
 * @brief  Creates a test calibration file for ShowerCalibrationGaloreFromPID
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    CreateTestShowerCalibrationFromPID.h
 *
 * Command line arguments:
 *
 * CreateTestCalibrationFromPID  [OutputPath]
 *
 * OutputPath is a full ROOT directory path made of a UNIX path and a ROOT
 * directory path. For example, "data/calibrations.root:Showers/ByType" will
 * create a directory "data" and a "calibrations.root" ROOT file in it (or
 * update it if exists), create a structure of two nested ROOT directories,
 * "Showers/ByType", and write all the calibration graphs in there.
 *
 */

// LArSoft libraries
#include "test/Services/ShowerCalibrationGalore/Providers/CreateTestShowerCalibrationFromPID.h"

// C/C++ standard libraries
#include <string>

int main(int argc, char** argv)
{

  //
  // argument parsing
  //
  std::string OutputFilePath = "ShowerCalibrationTestFromPID.root:Test";

  char** param = argv + 1;
  char** endparam = argv + argc;

  if (param < endparam) OutputFilePath = *param;

  //
  // run
  //
  return lar::example::tests::CreateTestShowerCalibrationFromPID(OutputFilePath);

} // main()
