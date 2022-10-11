/**
 * @file   CreateTestShowerCalibrationFromPID.cxx
 * @brief  Creates a test calibration file for ShowerCalibrationGaloreFromPID
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGaloreFromPID.h
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
 * Currently the following calibration objects are written:
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

// LArSoft libraries
#include "test/Services/ShowerCalibrationGalore/Providers/CreateTestShowerCalibrationFromPID.h"

// ROOT
#include "RtypesCore.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TSystem.h"

// C/C++ standard libraries
#include <iostream>
#include <memory>    // std::make_unique()
#include <stdexcept> // std::runtime_error
#include <string>
#include <tuple> // std::pair<>, std::tie()

namespace {

  //---------------------------------------------------------------------------

  /// Returns a pair with first what precedes sep in s (can be empty), second
  /// what follows sep (may be everything)
  std::pair<std::string, std::string> rsplit(std::string const& s, std::string const& sep)
  {
    auto iSep = s.rfind(sep);
    if (iSep == std::string::npos)
      return {{}, s};
    else {
      return {s.substr(0, iSep), s.substr(iSep + sep.length(), s.length())};
    }
  } // rsplit()

  /// Creates a ROOT directory from a path "path/to/file.root:dirA/dirB/dirC"
  TDirectory* CreateROOTdir(std::string path)
  {

    const std::string suffix = ".root";

    std::string filePath, rootDirPath;

    // find the ROOT file name
    std::string::size_type iSuffix = std::string::npos;
    do {
      iSuffix = path.rfind(suffix, iSuffix);
      if (iSuffix == std::string::npos) return nullptr;

      // if it's not "suffix:" or "suffix/" or at end of string, it's not it
      auto iAfter = iSuffix + suffix.length();
      if ((iAfter < path.length()) && (path[iAfter] != '/') && (path[iAfter] != ':')) {
        if (iSuffix == 0) return nullptr;
        --iSuffix;
        continue;
      }

      filePath = path.substr(0U, iAfter);
      if (iAfter < path.length()) rootDirPath = path.substr(iAfter + 1, path.length());

      break;
    } while (true);

    // split the file name path
    std::string fileDir, fileName;
    std::tie(fileDir, fileName) = rsplit(filePath, "/");

    std::cout << "Writing to output file: '" << filePath << "'";
    if (!rootDirPath.empty()) std::cout << " (directory: '" << rootDirPath << "')";
    std::cout << std::endl;

    // create the directory; the result is the same error code (-1)
    // whether it already exists or if creation failed;
    // we don't check it and rely on file creation later to report errors
    if (!fileDir.empty()) gSystem->mkdir(fileDir.c_str(), true);

    // we delete the TFile whatever bad happens;
    // we'll override this (release()) when we know we are keeping the file
    auto pFile = std::make_unique<TFile>(filePath.c_str(), "UPDATE");

    if (pFile->IsZombie() || !pFile->IsOpen()) return nullptr;
    if (rootDirPath.empty()) return pFile.release();

    if (!pFile->mkdir(rootDirPath.c_str())) return nullptr;

    TDirectory* pOutputDir = pFile->GetDirectory(rootDirPath.c_str());
    if (!pOutputDir) return nullptr;

    pFile.release(); // do not delete the file any more
    return pOutputDir;
  } // CreateROOTdir()

  //---------------------------------------------------------------------------
  void WriteCalibrationObject(TObject* pObj, std::string title = "calibration object")
  {
    if (!pObj) throw std::runtime_error("Can't write " + title);

    auto written = pObj->Write();
    if (written == 0) {
      throw std::runtime_error("Writing of " + title + " " + pObj->IsA()->GetName() + "[\"" +
                               pObj->GetName() + "\"] failed!");
    }
    std::cout << "Written " << title << " \"" << pObj->GetName() << "\" (" << pObj->IsA()->GetName()
              << ") [" << written << " bytes]" << std::endl;
  } // WriteCalibrationObject()

  //---------------------------------------------------------------------------
  TObject* CreateNeutralPionCalibration(std::string name = "Pi0")
  {

    constexpr Int_t NPoints = 21;
    constexpr Double_t MinE = 0.0; // GeV
    constexpr Double_t MaxE = 2.0; // GeV

    constexpr Double_t ERange = MaxE - MinE;              // GeV
    constexpr Double_t BinWidth = ERange / (NPoints - 1); // GeV

    TGraphErrors* pGCorr = new TGraphErrors(NPoints);
    pGCorr->SetNameTitle(name.c_str(),
                         "#pi^{0} energy calibration"
                         ";reconstructed energy [ GeV ]"
                         ";correction factor");

    for (Int_t i = 0; i < NPoints; ++i) {
      Double_t const E = MinE + BinWidth * i; // GeV

      Double_t const f = 1.1 - 0.2 * std::sin((E - MinE) / ERange * TMath::Pi() * 2.);

      pGCorr->SetPoint(i, E, f);

      pGCorr->SetPointError(i, BinWidth / 2., f * 0.1);

    } // for

    return pGCorr;
  } // CreateNeutralPionCalibration()

  //---------------------------------------------------------------------------
  TObject* CreatePhotonCalibration(std::string name = "Photon")
  {

    constexpr Int_t NPoints = 21;
    constexpr Double_t MinE = 0.0; // GeV
    constexpr Double_t MaxE = 2.0; // GeV

    constexpr Double_t ERange = MaxE - MinE;              // GeV
    constexpr Double_t BinWidth = ERange / (NPoints - 1); // GeV

    TGraphErrors* pGCorr = new TGraphErrors(NPoints);
    pGCorr->SetNameTitle(name.c_str(),
                         "#gamma energy calibration"
                         ";reconstructed energy [ GeV ]"
                         ";correction factor");

    for (Int_t i = 0; i < NPoints; ++i) {
      Double_t const E = MinE + BinWidth * i; // GeV

      Double_t const f = 1.1 + 0.1 * std::sin((E - MinE) / ERange * TMath::Pi() / 2.);

      pGCorr->SetPoint(i, E, f);

      pGCorr->SetPointError(i, BinWidth / 2., f * 0.1);

    } // for

    return pGCorr;
  } // CreatePhotonCalibration()

  //---------------------------------------------------------------------------
  TObject* CreateElectronCalibration(std::string name = "Electron")
  {

    constexpr Int_t NPoints = 21;
    constexpr Double_t MinE = 0.0; // GeV
    constexpr Double_t MaxE = 2.0; // GeV

    constexpr Double_t ERange = MaxE - MinE;              // GeV
    constexpr Double_t BinWidth = ERange / (NPoints - 1); // GeV

    TGraphErrors* pGCorr = new TGraphErrors(NPoints);
    pGCorr->SetNameTitle(name.c_str(),
                         "e^{#pm} energy calibration"
                         ";reconstructed energy [ GeV ]"
                         ";correction factor");

    for (Int_t i = 0; i < NPoints; ++i) {
      Double_t const E = MinE + BinWidth * i; // GeV

      Double_t const f = 1.15 + 0.1 * std::sin((E - MinE) / ERange * TMath::Pi());

      pGCorr->SetPoint(i, E, f);

      pGCorr->SetPointError(i, BinWidth / 2., f * 0.1);

    } // for

    return pGCorr;
  } // CreateElectronCalibration()

  //---------------------------------------------------------------------------
  TObject* CreateMuonCalibration(std::string name = "Muon")
  {

    constexpr Int_t NPoints = 21;
    constexpr Double_t MinE = 0.0; // GeV
    constexpr Double_t MaxE = 2.0; // GeV

    constexpr Double_t ERange = MaxE - MinE;              // GeV
    constexpr Double_t BinWidth = ERange / (NPoints - 1); // GeV

    TGraphErrors* pGCorr = new TGraphErrors(NPoints);
    pGCorr->SetNameTitle(name.c_str(),
                         "#mu^{#pm} energy calibration"
                         ";reconstructed energy [ GeV ]"
                         ";correction factor");

    for (Int_t i = 0; i < NPoints; ++i) {
      Double_t const E = MinE + BinWidth * i; // GeV

      Double_t const f = 1.05 + 0.02 * std::sin((E - MinE) / ERange * TMath::Pi() * 1.5);

      pGCorr->SetPoint(i, E, f);

      pGCorr->SetPointError(i, BinWidth / 2., f * 0.1);

    } // for

    return pGCorr;
  } // CreateMuonCalibration()

  //---------------------------------------------------------------------------
  TObject* CreateGeneralCalibration(std::string name = "Default")
  {

    constexpr Double_t MinE = 0.0; // GeV
    constexpr Double_t MaxE = 2.2; // GeV

    constexpr Double_t ERange = MaxE - MinE; // GeV

    TGraphErrors* pGCorr = new TGraphErrors(1);
    pGCorr->SetNameTitle(name.c_str(),
                         "Generic energy calibration"
                         ";reconstructed energy [ GeV ]"
                         ";correction factor");

    Double_t const E = MinE + ERange / 2.; // GeV
    Double_t const f = 1.10;

    pGCorr->SetPoint(0, E, f);

    pGCorr->SetPointError(0, ERange / 2., f * 0.1);

    return pGCorr;
  } // CreateGeneralCalibration()

  //---------------------------------------------------------------------------

} // local namespace

//------------------------------------------------------------------------------
int lar::example::tests::CreateTestShowerCalibrationFromPID(std::string outputPath)
{

  //
  // create output file
  //
  TDirectory* pOutputDir = CreateROOTdir(outputPath);
  if (!pOutputDir) {
    std::cerr << "Can't create ROOT directory '" << outputPath << "'" << std::endl;
    return 1;
  }
  TFile* pOutputFile = pOutputDir->GetFile();

  //
  // create the calibration histograms
  //
  pOutputDir->cd();
  try {
    WriteCalibrationObject(CreateNeutralPionCalibration(), "pion calibration");
    WriteCalibrationObject(CreatePhotonCalibration(), "photon calibration");
    WriteCalibrationObject(CreateElectronCalibration(), "electron calibration");
    WriteCalibrationObject(CreateMuonCalibration(), "muon calibration");
    WriteCalibrationObject(CreateGeneralCalibration(), "generic calibration");
  }
  catch (std::runtime_error const& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return 1;
  }

  //
  // close output and leave (deliberately ONLY if writing above was successful)
  //
  pOutputDir->Write();
  pOutputFile->Write();
  delete pOutputFile;

  return 0;
} // lar::example::tests::CreateTestShowerCalibrationFromPID()

//------------------------------------------------------------------------------
