/**
 * @file    AtomicNumberService.h
 * @brief   Returns the atomic number of the active material in the TPC
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    April 13, 2016
 * @see     AtomicNumberService_service.cc
 * @ingroup AtomicNumber
 *
 */

#ifndef LAREXAMPLES_SERVICES_ATOMICNUMBER_SERVICE_ATOMICNUMBERSERVICE_H
#define LAREXAMPLES_SERVICES_ATOMICNUMBER_SERVICE_ATOMICNUMBERSERVICE_H

// LArSoft libraries
#include "larcore/CoreUtils/ServiceProviderWrappers.h"
#include "larexamples/Services/AtomicNumber/Providers/AtomicNumber.h"

// framework and support libraries
#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

namespace lar {

  namespace example {

    /**
       * @brief Service giving access to AtomicNumber service provider.
       * @see AtomicNumber, @ref AtomicNumber "AtomicNumber example overview"
       * @ingroup AtomicNumber
       *
       * Access the provider with the standard LArSoft access pattern:
       *
       *     auto const* atomicNumber
       *       = lar::providerFrom<lar::example::AtomicNumberService>();
       *
       *
       */
    using AtomicNumberService = lar::SimpleServiceProviderWrapper<AtomicNumber>;

  } // namespace example
} // namespace lar

DECLARE_ART_SERVICE(lar::example::AtomicNumberService, LEGACY)

#endif // LAREXAMPLES_SERVICES_ATOMICNUMBER_SERVICE_ATOMICNUMBERSERVICE_H
