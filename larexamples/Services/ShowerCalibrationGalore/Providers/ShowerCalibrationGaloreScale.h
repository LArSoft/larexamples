/**
 * @file   ShowerCalibrationGaloreScale.h
 * @brief  Shower calibration service provider applying a uniform energy scale
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGalore.h
 * @ingroup ShowerCalibrationGalore
 *
 *
 */


#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALE_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALE_H


// LArSoft libraries
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"

// framework libraries
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/Table.h"

// C/C++ standard libraries
#include <sstream>
#include <string>


namespace lar {
   namespace example {

      /**
       * @brief A shower calibration service provider applying a uniform scale.
       * @ingroup ShowerCalibrationGalore
       * @see @ref ShowerCalibrationGalore "ShowerCalibrationGalore example overview"
       *
       * The service provider computes a calibration factor for a reconstructed
       * shower. The calibration factor is a single number for the whole
       * detector and for the entire data sample.
       *
       * Configuration parameters
       * -------------------------
       *
       * * *factor* (real, _mandatory_): the scale factor to apply
       * * *error* (real, _mandatory_): the total uncertainty on the correction
       *   factor
       *
       */
      class ShowerCalibrationGaloreScale: public ShowerCalibrationGalore {
            public:

         //---------------------------------------------------------------------
         /// Collection of configuration parameters for the service
         struct Config {
            using Name = fhicl::Name;
            using Comment = fhicl::Comment;

            fhicl::Atom<float> factor {
               Name("factor"),
               Comment("correction factor to be applied to all particles")
            };

            fhicl::Atom<float> error {
               Name("error"),
               Comment("uncertainty on the correction factor")
            };

         }; // struct Config

         /// Type describing all the parameters
         using parameters_type = fhicl::Table<Config>;


         //---------------------------------------------------------------------
         /// Constructor from the complete configuration object
         ShowerCalibrationGaloreScale(Config const& config)
           : corr(config.factor(), config.error())
           {}

         //---------------------------------------------------------------------
         /// Constructor from a parameter set
         ShowerCalibrationGaloreScale(fhicl::ParameterSet const& pset)
           : ShowerCalibrationGaloreScale
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
           (recob::Shower const&, PDGID_t = unknownID) const override
           { return corr.factor; }

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
           (recob::Shower const&, PDGID_t = unknownID) const override
           { return corr; }

         /// @}


         /// Returns a string with a short report of the current correctionß
         virtual std::string report() const override
           {
              std::ostringstream sstr;
              sstr << "Uniform correction: " << corr;
              return sstr.str();
           }


         //---------------------------------------------------------------------
            private:
         Correction_t corr;

      }; // class ShowerCalibrationGaloreScale


   } // namespace example
} // namespace lar


#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORESCALE_H

