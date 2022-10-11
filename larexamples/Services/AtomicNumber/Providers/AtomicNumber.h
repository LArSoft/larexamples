/**
 * @file    AtomicNumber.h
 * @brief   Provider returning atomic number of the active material in the TPC
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    April 13, 2016
 * @see     AtomicNumber.cxx AtomicNumberService.h
 * @ingroup AtomicNumber
 *
 */

#ifndef LAREXAMPLES_SERVICES_ATOMICNUMBER_PROVIDER_ATOMICNUMBER_H
#define LAREXAMPLES_SERVICES_ATOMICNUMBER_PROVIDER_ATOMICNUMBER_H

// support libraries
namespace fhicl {
  class ParameterSet;
}

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/Table.h"

namespace lar {
  namespace example {

    /** **********************************************************************
       * @brief Provides information about the active material in the TPC.
       * @see @ref AtomicNumber "AtomicNumber example overview"
       * @ingroup AtomicNumber
       *
       * Configuration parameters
       * =========================
       *
       * See `AtomicNumber` service provider class.
       *
       * - *AtomicNumber* (integer, default: 18): atomic number of the active
       *   material
       *
       */
    class AtomicNumber {
    public:
      //---------------------------------------------------------------------
      /// Collection of configuration parameters for the service
      struct Config {
        using Name = fhicl::Name;
        using Comment = fhicl::Comment;

        fhicl::Atom<unsigned int> AtomicNumber{
          Name("AtomicNumber"),
          Comment("atomic number of the active material in the TPC"),
          18U // default value
        };

      }; // struct Config

      /// Type describing all the parameters
      using parameters_type = fhicl::Table<Config>;

      //---------------------------------------------------------------------
      /// Constructor from the complete configuration object
      AtomicNumber(Config const& config) : Z_(config.AtomicNumber()) {}

      //---------------------------------------------------------------------
      /// Constructor from a parameter set
      AtomicNumber(fhicl::ParameterSet const& pset)
        : AtomicNumber(parameters_type(pset, {"service_type"})())
      {}

      //---------------------------------------------------------------------
      // copy and moving of service providers is "forbidden":
      AtomicNumber(AtomicNumber const& pset) = delete;
      AtomicNumber(AtomicNumber&& pset) = delete;
      AtomicNumber& operator=(AtomicNumber const& pset) = delete;
      AtomicNumber& operator=(AtomicNumber&& pset) = delete;

      //---------------------------------------------------------------------
      /// @name Accessors
      /// @{

      /// Returns the atomic number
      unsigned int Z() const { return Z_; }

      /// @}

      //---------------------------------------------------------------------
    private:
      unsigned int Z_; ///< atomic number

    }; // AtomicNumber

  } // namespace example
} // namespace lar

#endif // LAREXAMPLES_SERVICES_ATOMICNUMBER_PROVIDER_ATOMICNUMBER_H
