/**
 * @file   ShowerCalibrationGaloreFromPID.h
 * @brief  Shower energy calibration according to particle type
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGalore.h
 * @ingroup ShowerCalibrationGalore
 *
 *
 */


#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPID_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPID_H


// LArSoft libraries
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"

/// framework and utility libraries
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/Table.h"

// ROOT libraries
#include "Math/Interpolator.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TObject.h"

// C/C++ standard libraries
#include <string>
#include <vector>
#include <memory> // std::unique_ptr()
#include <type_traits> // std::is_base_of<>
#include <initializer_list>


// forward declarations
class TGraph; // from ROOT


namespace lar {
   namespace example {

      // BEGIN ShowerCalibrationGalore -----------------------------------------
      /// @ingroup ShowerCalibrationGalore
      /// @{

      namespace details {

         /// Reads an object from a ROOT directory, checking its type
         /// @throw cet::exception if no object or wrong type
         template <typename ROOTobj>
         std::unique_ptr<ROOTobj> readROOTobject
            (TDirectory* sourceDir, std::string name);

      } // namespace details


      /// Splits path into ROOT file name and directory path
      std::pair<std::string, std::string> splitROOTpath(std::string path);


      /**
       * @brief Shower calibration service provider correcting according to PID.
       *
       * The service provider computes a calibration factor for a reconstructed
       * shower. The calibration factor depends on an hypothesis on the type of
       * particle.
       * The calibration factors are extracted from the specified ROOT file.
       *
       * Calibration file format
       * ------------------------
       *
       * Calibration is represented by a list of objects with specific names:
       *
       * * `"Pi0"` (`TGraphErrors`):
       *   neutral pion calibration vs. reconstructed energy
       * * `"Photon"` (`TGraphErrors`):
       *   photon calibration vs. reconstructed energy
       * * `"Electron"` (`TGraphErrors`):
       *   electron/positron calibration vs. reconstructed energy
       * * `"Muon"` (`TGraphErrors`):
       *   muon/antimuon calibration vs. reconstructed energy
       * * `"Default"` (`TGraphErrors`):
       *   other particle calibration vs. reconstructed energy
       *
       * Each graph is required to hold at least one point, and its points must
       * be already sorted by energy.
       * Energy is measured in GeV.
       *
       *
       * Calibration factors from the input
       * -----------------------------------
       *
       * The input calibration objects are graphs with symmetric errors.
       * The independent variable is the best estimation of the reconstructed
       * energy of the shower. The correction factor is interpolated (by a cubic
       * spline) between the points in the graph; errors are likewise
       * interpolated. If the requested energy is outside the range of the graph
       * the correction is the same as the closest available energy point, with
       * its uncertainty doubled every half full range of the graph.
       * As a special case, if the graph has only one point, the correction is
       * uniform in the full energy spectrum (including its uncertainty).
       *
       *
       * Configuration parameters
       * -------------------------
       *
       * * *CalibrationFile* (string, _mandatory_): path to the file containing
       *   the full shower calibration information; it is made of a file system
       *   path to the ROOT file, and an optional ROOT directory path; for
       *   example: `path/to/file.root:Calibrations/Shower` expects a nested
       *   ROOT directory structure `Calibrations/Shower` in the ROOT file
       *   `path/to/file.root`, where `path` is accessible from the usual search
       *   path in `FW_SEARCH_PATH`
       *
       */
      class ShowerCalibrationGaloreFromPID: public ShowerCalibrationGalore {
            public:

         //---------------------------------------------------------------------
         /// Collection of configuration parameters for the service
         struct Config {
            using Name = fhicl::Name;
            using Comment = fhicl::Comment;

            fhicl::Atom<std::string> CalibrationFile {
               Name("CalibrationFile"),
               Comment(
                 "path to calibration file and ROOT directory"
                 " (e.g. path/to/file.root:Dir/Dir)"
                 )
            };

         }; // struct Config

         /// Type describing all the parameters
         using parameters_type = fhicl::Table<Config>;


         //---------------------------------------------------------------------
         /// Constructor from the complete configuration object
         ShowerCalibrationGaloreFromPID(Config const& config)
           { readCalibration(config.CalibrationFile()); }

         //---------------------------------------------------------------------
         /// Constructor from a parameter set
         ShowerCalibrationGaloreFromPID(fhicl::ParameterSet const& pset)
           : ShowerCalibrationGaloreFromPID
             (parameters_type(pset, { "service_type", "service_provider" })())
           {}


         /// @{
         /// @name Correction query

         //---------------------------------------------------------------------
         /**
          * @brief Returns a correction factor for a given reconstructed shower
          * @return the uniform energy correction factor
          * @see correction()
          *
          * The returned value includes a correction factor to be applied to
          * the shower energy to calibrate it, but no uncertainty.
          *
          */
         virtual float correctionFactor
           (recob::Shower const&, PDGID_t = unknownID) const override;

         /**
          * @brief Returns the correction for a given reconstructed shower
          * @return the correction with its uncertainty
          * @see correctionFactor()
          *
          * The returned value includes a correction factor to be applied to
          * any shower energy to calibrate it, with its global uncertainty.
          *
          */
         virtual Correction_t correction
           (recob::Shower const&, PDGID_t = unknownID) const override;

         /// @}

         /// Returns a string with a short report of the current corrections
         virtual std::string report() const override
           { std::ostringstream sstr; reportTo(sstr); return sstr.str(); }


         /// Reads the calibration information from the specified file
         void readCalibration(std::string path);


         /// Prints a short report of the current corrections
         template <typename Stream>
         void reportTo(Stream&& out) const;


         /// Verifies that points in specified graph have increasing abscissa
         /// @throw cet::exception if points are not sorted by growing x
         static void verifyOrder(TGraph const* graph);


         //---------------------------------------------------------------------
            private:
         /// Internal structure containing the calibration information
         struct CalibrationInfo_t {
            std::vector<PDGID_t> appliesTo; ///< PID it applies to; unused
            double minE = -1.; ///< lower end of the energy range covered [GeV]
            double maxE = -1.; ///< upper end of the energy range covered [GeV]

            /// parametrisation of the correction factor
            std::unique_ptr<ROOT::Math::Interpolator> factor;

            /// parametrisation of the correction uncertainty
            std::unique_ptr<ROOT::Math::Interpolator> error;

            double evalFactor(double E) const;
            double evalError(double E) const;

            bool present() const { return maxE >= 0.; }
            bool uniform() const { return minE == maxE; }

            CalibrationInfo_t& applyTo(PDGID_t id);
            CalibrationInfo_t& applyTo(std::initializer_list<PDGID_t> ids);

            /// Prints a short report of this correction
            template <typename Stream>
            void reportTo(Stream&& out) const;
         }; // CalibrationInfo_t


         CalibrationInfo_t Calibration_pi0; ///< neutral pion calibration
         CalibrationInfo_t Calibration_photon; ///< photon calibration
         CalibrationInfo_t Calibration_electron; ///< electron/positron calibration
         CalibrationInfo_t Calibration_muon; ///< muon/antimuon calibration
         CalibrationInfo_t Calibration_other; ///< default calibration


         /// Returns the correct CalibrationInfo_t for specified id
         CalibrationInfo_t const& selectCorrection(PDGID_t id) const;

         /// Reads and returns calibration information from the specified graph
         CalibrationInfo_t readParticleCalibration
           (TDirectory* SourceDir, std::string GraphName) const;

         /// Reads and returns calibration information from the specified graph
         /// and register a particle ID in it
         CalibrationInfo_t readParticleCalibration
           (TDirectory* SourceDir, std::string GraphName, PDGID_t id) const;

         /// Reads and returns calibration information from the specified graph
         /// and register a list of particle IDs to it
         CalibrationInfo_t readParticleCalibration(
            TDirectory* SourceDir, std::string GraphName,
            std::initializer_list<PDGID_t> ids
            ) const;

         /// Opens the specified ROOT directory, as in path/to/file.root:dir/dir
         static TDirectory* OpenROOTdirectory(std::string path);

         /// Creates a ROOT interpolator from a set of N points
         static std::unique_ptr<ROOT::Math::Interpolator>
         createInterpolator(unsigned int N, double const* x, double const* y);

      }; // class ShowerCalibrationGaloreFromPID

      /// @}
      // END ShowerCalibrationGalore -----------------------------------------------


   } // namespace example
} // namespace lar



//------------------------------------------------------------------------------
//--- template implementation
//---
template <typename ROOTobj>
std::unique_ptr<ROOTobj> lar::example::details::readROOTobject
   (TDirectory* sourceDir, std::string name)
{
   if (!sourceDir) {
      throw cet::exception("readROOTobject")
        << "Invalid source ROOT directory\n";
   }

   // read the object and immediately claim its ownership
   TObject* pObj = sourceDir->Get(name.c_str());
   if (!pObj) {
      throw cet::exception("readROOTobject")
        << "No object '" << name << "' in ROOT directory '"
        << sourceDir->GetPath() << "'\n";
   }

   if (std::is_base_of<TH1, std::decay_t<ROOTobj>>::value)
      static_cast<TH1*>(pObj)->SetDirectory(nullptr);
//   if (std::is_base_of<TTree, std::decay_t<ROOTobj>>::value)
//      static_cast<TTree*>(pObj)->SetDirectory(nullptr);
   std::unique_ptr<TObject> obj(pObj);

   // use ROOT to investigate whether this is the right type
   if (!obj->InheritsFrom(ROOTobj::Class())) {
      throw cet::exception("readROOTobject")
        << "Object '" << name << "' in ROOT directory '"
        << sourceDir->GetPath() << "' is a " << obj->IsA()->GetName()
        << ", not derived from " << ROOTobj::Class()->GetName() << "\n";
   }

   // now that we are sure obj is of the right type, we transfer ownership
   // from obj to the return value (that here is implicitly constructed)
   return std::unique_ptr<ROOTobj>(static_cast<ROOTobj*>(obj.release()));
} // lar::example::details::readROOTobject()

//------------------------------------------------------------------------------
//--- lar::example::ShowerCalibrationGaloreFromPID
//---
template <typename Stream>
void lar::example::ShowerCalibrationGaloreFromPID::reportTo(Stream&& out) const
{
   out << "Corrections for:";
   out << "\n  - neutral pion:      ";
   Calibration_pi0.reportTo(std::forward<Stream>(out));
   out << "\n  - photon:            ";
   Calibration_photon.reportTo(std::forward<Stream>(out));
   out << "\n  - electron/positron: ";
   Calibration_electron.reportTo(std::forward<Stream>(out));
   out << "\n  - muon/antimuon:     ";
   Calibration_muon.reportTo(std::forward<Stream>(out));
   out << "\n  - other (default):   ";
   Calibration_other.reportTo(std::forward<Stream>(out));
   out << "\n";
} // lar::example::ShowerCalibrationGaloreFromPID::report()


//------------------------------------------------------------------------------
template <typename Stream>
void lar::example::ShowerCalibrationGaloreFromPID::CalibrationInfo_t::reportTo
   (Stream&& out) const
{
   if (!present()) {
      out << "not present";
      return;
   }
   if (uniform()) {
      out << "uniform correction " << factor->Eval(minE) << " +/- "
         << error->Eval(minE) << " for all energies";
   }
   else {
      out << "correction valid from E=" << minE
         << " GeV (" << factor->Eval(minE) << " +/- " << error->Eval(minE)
         << ") to E=" << maxE
         << " GeV (" << factor->Eval(maxE) << " +/- " << error->Eval(maxE)
         << ")";
   }
   if (!appliesTo.empty()) {
      out << "; covers particles ID={";
      for (auto id: appliesTo) out << " " << id;
      out << " }";
   }

} // lar::example::ShowerCalibrationGaloreFromPID::CalibrationInfo_t::report()


//------------------------------------------------------------------------------


#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALOREFROMPID_H

