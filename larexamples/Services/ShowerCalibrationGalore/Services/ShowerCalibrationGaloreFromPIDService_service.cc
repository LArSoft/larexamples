/**
 * @file   ShowerCalibrationGaloreFromPIDService_service.cc
 * @brief  Shower energy calibration according to particle type.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGaloreFromPID.h
 * @ingroup ShowerCalibrationGalore
 *
 *
 */

/// LArSoft libraries
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreFromPID.h"
#include "larexamples/Services/ShowerCalibrationGalore/Services/ShowerCalibrationGaloreService.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"


namespace lar {
  namespace example {

    /**
     * @brief Service for shower energy calibration according to particle type
     * @ingroup ShowerCalibrationGalore
     * @see @ref ShowerCalibrationGalore "ShowerCalibrationGalore example overview"
     *
     * See the ShowerCalibrationGaloreFromPID provider documentation for
     * configuration instructions and implementation details.
     *
     * See the ShowerCalibrationGalore provider class documentation for an
     * explanation of the interface.
     *
     * Use this service and its provider by its interface only:
     *
     *     lar::example::ShowerCalibrationGalore const* calib
     *       = lar::providerFrom<lar::example::ShowerCalibrationGaloreService>();
     *
     * The code does not need to mention, nor to include, nor to link to
     * `ShowerCalibrationGaloreFromPIDService` service.
     *
     *
     * Configuration parameters
     * -------------------------
     *
     * In addition to the service provider:
     *
     * * *service_provider* must be set to
     *   `"ShowerCalibrationGaloreFromPIDService"` in order to tell _art_ to
     *   load this implementation.
     *
     */
    using ShowerCalibrationGaloreFromPIDService
      = lar::ServiceProviderImplementationWrapper
        <ShowerCalibrationGaloreFromPID, ShowerCalibrationGaloreService>;

  } // namespace example
} // namespace lar


DECLARE_ART_SERVICE_INTERFACE_IMPL(
  lar::example::ShowerCalibrationGaloreFromPIDService,
  lar::example::ShowerCalibrationGaloreService,
  LEGACY
  )

#include "art/Framework/Services/Registry/ServiceDefinitionMacros.h"

DEFINE_ART_SERVICE_INTERFACE_IMPL(
  lar::example::ShowerCalibrationGaloreFromPIDService,
  lar::example::ShowerCalibrationGaloreService
  )
