/**
 * @file    AtomicNumberService_service.cc
 * @brief   Returns the atomic number of the active material in the TPC
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    April 13, 2016
 * @see     AtomicNumberService.h
 * @ingroup AtomicNumber
 *
 * Implementation file.
 */

// LArSoft libraries
#include "art/Framework/Services/Registry/ServiceDefinitionMacros.h"
#include "larexamples/Services/AtomicNumber/Services/AtomicNumberService.h"

// -----------------------------------------------------------------------------
DEFINE_ART_SERVICE(lar::example::AtomicNumberService)

// -----------------------------------------------------------------------------
