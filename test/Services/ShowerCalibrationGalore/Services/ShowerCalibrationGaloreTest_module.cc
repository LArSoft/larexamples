/**
 * @file   ShowerCalibrationGaloreTest_module.cc
 * @brief  A simple test for ShowerCalibrationGaloreService example services
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 4, 2016
 * @see    ShowerCalibrationGaloreService.h
 *
 */

// LArSoft libraries
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"
#include "larexamples/Services/ShowerCalibrationGalore/Services/ShowerCalibrationGaloreService.h"
#include "test/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreTests.h"

// framework and support libraries
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "canvas/Utilities/Exception.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C++ standard libraries

namespace lar {
  namespace example {
    namespace tests {

      /**
          * @brief Tests ShowerCalibrationGaloreService
          *
          *
          * Prints the available corrections from the configured shower
          * calibration service.
          *
          *
          * Configuration parameters
          * -------------------------
          *
          * * *Emin* (real, default: 0), *Emax* (real, default: 2.5):
          *     lower and upper shower energy for printout, in GeV.
          * * *Estep* (real, default: 0): shower energy step size for the
          *     printout, in GeV. The special value of 0 directs the module to
          *     arrange 10 steps in the full range.
          * * *PID* (list of integers, default: { 11, 13, 111, 2212, 22 }):
          *     print the correction for particles with type from this list;
          *     each number is the PDG particle ID.
          *
          */
      class ShowerCalibrationGaloreTest : public art::EDAnalyzer {
      public:
        struct Config {

          using Name = fhicl::Name;
          using Comment = fhicl::Comment;

          fhicl::Atom<float> Emin{
            Name("Emin"),
            Comment("starting shower energy [GeV]"),
            0.0 // default
          };

          fhicl::Atom<float> Emax{
            Name("Emax"),
            Comment("last shower energy [GeV]"),
            2.5 // default
          };

          fhicl::Atom<float> Estep{
            Name("Estep"),
            Comment("shower energy step [GeV] (0 to have 10 steps)"),
            0.0 // default
          };

          fhicl::Sequence<int> PID{
            Name("PID"),
            Comment("list of ID of particles to test shower calibration of"),
            {11, 13, 111, 2212, 22} // default
          };

        }; // Config

        explicit ShowerCalibrationGaloreTest(art::EDAnalyzer::Table<Config> const& config);

        // correction printout is repeated for each event
        virtual void analyze(art::Event const&) override;

        virtual void beginRun(art::Run const&) override;

      private:
        float Emin = 0.0;
        float Emax = 1.0;
        float Estep = 0.0;
        std::vector<lar::example::ShowerCalibrationGalore::PDGID_t> PIDs;

      }; // class ShowerCalibrationGaloreTest

    } // namespace tests
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------
lar::example::tests::ShowerCalibrationGaloreTest::ShowerCalibrationGaloreTest(
  art::EDAnalyzer::Table<Config> const& config)
  : art::EDAnalyzer(config)
  , Emin(config().Emin())
  , Emax(config().Emax())
  , Estep(config().Estep())
  , PIDs(config().PID())
{}

//------------------------------------------------------------------------------
void lar::example::tests::ShowerCalibrationGaloreTest::beginRun(art::Run const&)
{

  auto const* calib = lar::providerFrom<lar::example::ShowerCalibrationGaloreService>();

  mf::LogInfo("ShowerCalibrationGaloreTest") << calib->report();

} // lar::example::tests::ShowerCalibrationGaloreTest::beginRun()

//------------------------------------------------------------------------------
void lar::example::tests::ShowerCalibrationGaloreTest::analyze(art::Event const&)
{
  auto const* calib = lar::providerFrom<lar::example::ShowerCalibrationGaloreService>();

  lar::example::tests::ShowerCalibrationTableTest(
    mf::LogVerbatim("ShowerCalibrationGaloreTest"), calib, Emin, Emax, Estep, PIDs);

} // lar::example::tests::ShowerCalibrationGaloreTest::beginRun()

//------------------------------------------------------------------------------

DEFINE_ART_MODULE(lar::example::tests::ShowerCalibrationGaloreTest)