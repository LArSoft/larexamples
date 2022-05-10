/**
 * @file   ShowerCalibrationGaloreScaleService_service.cc
 * @brief  A shower calibration service providing a single calibration factor
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 28, 2016
 * @see    ShowerCalibrationGaloreScale.h
 * @ingroup ShowerCalibrationGalore
 *
 *
 */

/// LArSoft libraries
#include "larexamples/Services/ShowerCalibrationGalore/Providers/ShowerCalibrationGaloreScale.h"
#include "larexamples/Services/ShowerCalibrationGalore/Services/ShowerCalibrationGaloreService.h"
#include "larcore/CoreUtils/ServiceUtil.h" // lar::providerFrom() (for includers)

// framework libraries
#include "art/Framework/Services/Registry/ServiceTable.h"
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

// C/C++ standard libraries
#include <memory> // std::unique_ptr<>, std::make_unique()


namespace lar {
  namespace example {

    /**
     * @brief A shower calibration art service returning a single factor
     * @ingroup ShowerCalibrationGalore
     * @see @ref ShowerCalibrationGalore "ShowerCalibrationGalore example overview"
     *
     * See the ShowerCalibrationGaloreScale provider documentation for
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
     * `ShowerCalibrationGaloreScaleService` service.
     *
     *
     * Configuration parameters
     * -------------------------
     *
     * In addition to the service provider:
     *
     * * *service_provider* must be set to
     *   `"ShowerCalibrationGaloreScaleService"` in order to tell _art_ to
     *   load this implementation.
     *
     */
    class ShowerCalibrationGaloreScaleService:
      public ShowerCalibrationGaloreService
    {

         public:
       /// type of service provider implementation
       using concrete_provider_type = ShowerCalibrationGaloreScale;

       /// art service interface class
       using service_interface_type = ShowerCalibrationGaloreService;

       /// Type of configuration parameter (for art description)
       using Parameters
         = art::ServiceTable<typename ShowerCalibrationGaloreScale::Config>;


       /// Constructor (using a configuration table)
       ShowerCalibrationGaloreScaleService
         (Parameters const& config, art::ActivityRegistry&)
         : prov(std::make_unique<ShowerCalibrationGaloreScale>(config()))
         {}


          private:
       std::unique_ptr<ShowerCalibrationGaloreScale> prov; ///< service provider

       /// Returns a constant pointer to the service provider
       virtual ShowerCalibrationGalore const* do_provider() const override
         { return prov.get(); }

    }; // ShowerCalibrationGaloreScaleService

  } // namespace example
} // namespace lar


DECLARE_ART_SERVICE_INTERFACE_IMPL(
  lar::example::ShowerCalibrationGaloreScaleService,
  lar::example::ShowerCalibrationGaloreService,
  LEGACY
  )

#include "art/Framework/Services/Registry/ServiceDefinitionMacros.h"

DEFINE_ART_SERVICE_INTERFACE_IMPL(
  lar::example::ShowerCalibrationGaloreScaleService,
  lar::example::ShowerCalibrationGaloreService
  )
