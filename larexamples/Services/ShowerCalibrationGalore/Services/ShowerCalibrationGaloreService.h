/**
 * @file   ShowerCalibrationGaloreService.h
 * @brief  Interface for a shower calibration art service
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 27, 2016
 * @see    ShowerCalibrationGalore.h
 * @ingroup ShowerCalibrationGalore
 */

#ifndef LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_SERVICES_SHOWERCALIBRATIONGALORESERVICE_H
#define LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_SERVICES_SHOWERCALIBRATIONGALORESERVICE_H

/// LArSoft libraries
#include "larcore/CoreUtils/ServiceProviderWrappers.h"
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGalore.h"

/// framework libraries
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

namespace lar {
  namespace example {

    /**
       * @brief Interface for a shower calibration art service
       * @ingroup ShowerCalibrationGalore
       * @see @ref ShowerCalibrationGalore "ShowerCalibrationGalore example overview"
       *
       * Services derived from this interface just return a provider
       * implementing the ShowerCalibrationGalore service provider interface.
       *
       * Implementations of this service must override
       * `ShowerCalibrationGaloreService::do_provider()`.
       *
       */
    using ShowerCalibrationGaloreService =
      lar::ServiceProviderInterfaceWrapper<ShowerCalibrationGalore>;

  } // namespace example
} // namespace lar

DECLARE_ART_SERVICE_INTERFACE(lar::example::ShowerCalibrationGaloreService, LEGACY)

#endif // LAREXAMPLES_SERVICES_SHOWERCALIBRATIONGALORE_SERVICES_SHOWERCALIBRATIONGALORESERVICE_H
