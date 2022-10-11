/**
 * @file   ShowerCalibrationGalore.h
 * @brief  Interface for a shower calibration service provider
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 27, 2016
 * @ingroup ShowerCalibrationGalore
 *
 *
 */

#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORE_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORE_H

/// LArSoft libraries
#include "larcorealg/CoreUtils/UncopiableAndUnmovableClass.h"
#include "lardataobj/RecoBase/Shower.h"

// C/C++ standard libraries
#include <string>

namespace lar {
  namespace example {

    /**
       * @brief Interface for a shower calibration service provider.
       * @ingroup ShowerCalibrationGalore
       * @see @ref ShowerCalibrationGalore "ShowerCalibrationGalore example overview"
       *
       * The service provider computes a calibration factor for a reconstructed
       * shower.
       *
       * It offers:
       *
       * * correction() to get the calibration factor with uncertainty
       * * correctionFactor() to get just the calibration factor
       *
       * This is an abstract interface. The corresponding _art_ service
       * interface is called `ShowerCalibrationGaloreService`.
       *
       */
    class ShowerCalibrationGalore : protected lar::UncopiableAndUnmovableClass {
    public:
      //---------------------------------------------------------------------
      /// A correction factor with global uncertainty
      struct Correction_t {
        float factor; ///< correction factor
        float error;  ///< correction factor uncertainty

        Correction_t(float val = 1., float err = 0.) : factor(val), error(err) {}

        bool operator==(Correction_t const& as) const
        {
          return (factor == as.factor) && (error == as.error);
        }
        bool operator!=(Correction_t const& as) const
        {
          return (factor != as.factor) || (error != as.error);
        }
      }; // Correction_t

      /// A type representing a particle ID in Particle Data Group convention
      using PDGID_t = int;

      /// A mnemonic constant for unknown particle ID
      static constexpr PDGID_t unknownID = 0;

      //---------------------------------------------------------------------
      // Virtual destructor
      virtual ~ShowerCalibrationGalore() = default;

      /// @{
      /// @name Correction query

      //---------------------------------------------------------------------
      /**
          * @brief Returns a correction factor for a given reconstructed shower
          * @param shower the shower to be calibrated
          * @param PDGID hypothesis on type of particle originating the shower
          * @return the correction factor
          * @see correction()
          *
          * The returned value includes a correction factor to be applied to
          * the shower energy to calibrate it, but no uncertainty.
          *
          * The particle type hypothesis argument is optional, and the invalid
          * type `0` (`unknownID`) implies that no hypothesis is present.
          *
          */
      virtual float correctionFactor(recob::Shower const& shower,
                                     PDGID_t PDGID = unknownID) const = 0;

      /**
          * @brief Returns the correction for a given reconstructed shower
          * @param shower the shower to be calibrated
          * @param PDGID hypothesis on type of particle originating the shower
          * @return the correction with its uncertainty
          * @see correctionFactor()
          *
          * The returned value includes a correction factor to be applied to
          * the shower energy to calibrate it, with its global uncertainty.
          *
          * The particle type hypothesis argument is optional, and the invalid
          * type `0` (`unknownID`) implies that no hypothesis is present.
          *
          */
      virtual Correction_t correction(recob::Shower const& shower,
                                      PDGID_t PDGID = unknownID) const = 0;

      /// @}

      /// Returns a string with a short report of the current corrections
      virtual std::string report() const = 0;

      //---------------------------------------------------------------------

    }; // class ShowerCalibrationGalore

    /// Output operator for the correction type
    template <typename Stream>
    Stream& operator<<(Stream&& out, ShowerCalibrationGalore::Correction_t const& corr)
    {
      out << corr.factor << " +/- " << corr.error;
      return out;
    } // operator<< (Correction_t)

  } // namespace example
} // namespace lar

#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_PROVIDERS_SHOWERCALIBRATIONGALORE_H
