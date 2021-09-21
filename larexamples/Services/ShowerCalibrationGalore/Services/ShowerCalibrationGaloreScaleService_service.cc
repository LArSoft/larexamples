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

// our header
#include "larexamples/Services/ShowerCalibrationGalore/Services/ShowerCalibrationGaloreScaleService.h"
#include "art/Framework/Services/Registry/ServiceDefinitionMacros.h"

DEFINE_ART_SERVICE_INTERFACE_IMPL(
  lar::example::ShowerCalibrationGaloreScaleService,
  lar::example::ShowerCalibrationGaloreService
  )
