/**
 * @file   ShowerCalibrationGaloreTests.h
 * @brief  Test functions for ShowerCalibrationGalore service providers
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 4th, 2016
 *
 * This is a template only header.
 * It provides:
 * - ShowerCalibrationTableTest(): prints correction table for different showers
 *
 * It requires linking with:
 *  - lardata_RecoBase
 */

// LArSoft libraries
#include "lardataobj/RecoBase/Shower.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"

// C/C++ standard libraries
#include <algorithm> // std::max()
#include <iomanip>
#include <string>
#include <tuple> // std::pair<>, std::get()
#include <vector>

namespace lar {
  namespace example {
    namespace tests {

      /// Returns a string padding s to be centered in a width w
      inline std::string centerString(std::string const& s, unsigned int width, char pad = ' ')
      {
        unsigned int const left = std::max(0U, (width - (unsigned int)s.length()) / 2);
        unsigned int const right = std::max(0U, width - left - (unsigned int)s.length());
        return std::string(left, pad) + s + std::string(right, pad);
      } // centerString()

      //------------------------------------------------------------------------------

      inline recob::Shower MakeShower(float E, int bestPlane = 2, int ID = 1)
      {
        // a shower on a 3-plane detector
        return recob::Shower({0., 0., 1.},       // direction (cosines) at vertex: along z axis
                             {1e-3, 1e-3, 1e-3}, // uncertainty on the above
                             {0., 0., 0.},       // start vertex (somewhere; we don't use geometry)
                             {1e-3, 1e-3, 1e-3}, // uncertainty on the above
                             {1., 1., 1.},       // consistent measurement of 1 (GeV?) on all planes
                             {1e-1, 1e-1, 1e-1}, // uncertainty on the above (10%)
                             {E, E, E},          // consistent measurement of 1 (GeV?) on all planes
                             {0.1 * E, 0.1 * E, 0.1 * E}, // uncertainty on the above (10%)
                             bestPlane,                   // elect the last as the best plane
                             ID                           // ID
        );
      } // MakeShower()

      //------------------------------------------------------------------------------
      /**
 * @brief Synthetic test: prints corrections for showers in a energy range
 * @tparam Stream type of output stream
 * @param out output stream
 * @param calibration service provider
 * @param Emin lower shower energy for the printout [GeV] (default: 0)
 * @param Emax upper shower energy for the printout [GeV] (default: 2.5)
 * @param Estep energy step size for the printout [GeV] (default: have 10 steps)
 * @param pids use these PIDs (default: { 11, 13, 111, 2212, 22 })
 * @return number of detected errors (currently always 0)
 *
 * The corrections are printed in a table like:
 * ~~~~
 * E [GeV]      particle1        particle2      ...
 *  0.000    1.000 +/- 0.000  1.023 +/- 0.003   ...
 *  ...
 * ~~~~
 */
      template <typename Stream>
      unsigned int ShowerCalibrationTableTest(
        Stream&& out,
        lar::example::ShowerCalibrationGalore const* calibration,
        float Emin = 0.0,
        float Emax = 2.5,
        float Estep = 0.1,
        std::vector<lar::example::ShowerCalibrationGalore::PDGID_t> const& pids =
          {11, 13, 111, 2212, 22})
      {

        // a shower with 2 GeV of energy
        recob::Shower shower = MakeShower(2.);

        using PIDInfo_t = std::pair<lar::example::ShowerCalibrationGalore::PDGID_t, std::string>;
        std::vector<PIDInfo_t> const KnownPIDs = {{11, "e-"},
                                                  {13, "mu-"},
                                                  {-11, "e+"},
                                                  {-13, "mu+"},
                                                  {211, "pi+"},
                                                  {111, "pi0"},
                                                  {2112, "n"},
                                                  {2212, "p"},
                                                  {22, "photon"},
                                                  {0, "default"}};
        PIDInfo_t const UnknownPID = {0, "<unnamed>"};

        // compute a default Estep to have 10 steps
        if (Estep == 0.) Estep = std::abs(Emax - Emin) / 10;
        if (Emax < Emin) Estep = -std::abs(Estep);
        const unsigned int nSteps = (Emax == Emin) ? 1U : (unsigned int)((Emax - Emin) / Estep);

        constexpr unsigned int widthE = 7;
        constexpr unsigned int widthF = 5;
        constexpr unsigned int widthFtoErr = 5;
        constexpr unsigned int widthFerr = 5;
        constexpr unsigned int widthCorr = widthF + widthFtoErr + widthFerr;
        const std::string sep = "  ";

        // table header
        out << centerString("E [GeV]", widthE);
        for (auto pid : pids) {
          auto iKnown =
            std::find_if(KnownPIDs.begin(), KnownPIDs.end(), [pid](PIDInfo_t const& info) {
              return pid == std::get<0>(info);
            });
          out << sep;
          if (iKnown == KnownPIDs.end()) {
            out << std::setw(widthF) << "PID=" << std::left << std::setw(widthFtoErr + widthFerr)
                << pid << std::right;
          }
          else {
            out << centerString(std::get<1>(*iKnown), widthCorr);
          }
        } // for

        // print a line of corrections for each energy
        for (unsigned int i = 0; i <= nSteps; ++i) {

          float const E = Emin + i * Estep;

          // set the same energy from every of the three planes
          shower.set_total_energy({E, E, E});

          out << "\n" << std::fixed << std::setw(widthE) << std::setprecision(3) << E;

          for (auto pid : pids) {
            auto corr = calibration->correction(shower, pid);

            out << sep << std::fixed << std::setw(widthF) << corr.factor << std::setw(widthFtoErr)
                << " +/- " << std::fixed << std::setw(widthFerr) << corr.error;

          } // for (pid)

        } // for (i)
        out << "\n";

        return 0; // no real error detection here, sorry
      }           // ShowerCalibrationTest()

      //------------------------------------------------------------------------------

    } // namespace tests
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------
