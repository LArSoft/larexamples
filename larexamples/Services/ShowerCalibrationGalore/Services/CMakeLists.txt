cet_make_library(LIBRARY_NAME ShowerCalibrationGaloreService INTERFACE
  SOURCE ShowerCalibrationGaloreService.h
  LIBRARIES INTERFACE
  larexamples::ShowerCalibrationGalore
  larcore::ServiceProviderWrappers
  art::Framework_Services_Registry
)

cet_write_plugin_builder(larexamples::ShowerCalibrationGaloreService
  art::service Modules INSTALL_BUILDER
  LIBRARIES CONDITIONAL
  larexamples::ShowerCalibrationGaloreService
)

include(larexamples::ShowerCalibrationGaloreService)

cet_build_plugin(ShowerCalibrationGaloreFromPIDService
  larexamples::ShowerCalibrationGaloreService
  LIBRARIES PRIVATE
  larexamples::Services_ShowerCalibrationGalore_Providers
)

cet_build_plugin(ShowerCalibrationGaloreScaleService
  larexamples::ShowerCalibrationGaloreService
  IMPL_SOURCE
)

install_fhicl()
install_headers()
