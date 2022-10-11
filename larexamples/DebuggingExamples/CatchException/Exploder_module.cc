/**
 * @file   larexamples/DebuggingExamples/CatchException/Exploder_module.cc
 * @brief  A module throwing exceptions.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   March 21, 2018
 * @ingroup CatchException
 */

// framework libraries
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "canvas/Utilities/Exception.h"
#include "fhiclcpp/types/Atom.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <array>
#include <new>       // std::bad_alloc
#include <stdexcept> // std::out_of_range, std::length_error

namespace lar {
  namespace example {

    /**
     * @brief A buggy module.
     *
     * This module executes a number of iterations.
     * It does something in some of those iterations, depending on the module
     * configuration.
     *
     *
     * Configuration
     * ==============
     *
     * * *NIterations* (integer, default: `10`): number of iterations executed
     * * *OutOfRange* (integer, optional): if specified, the module throws a
     *   `std::out_of_range` exception at the specified loop number
     *
     */
    class Exploder : public art::EDAnalyzer {
    public:
      struct Config {

        fhicl::Atom<bool> ManageBadAlloc{
          fhicl::Name("ManageBadAlloc"),
          fhicl::Comment("whether to catch the std::bad_alloc we throw"),
          true // default
        };

        fhicl::Atom<bool> ManageOutOfRange{
          fhicl::Name("ManageOutOfRange"),
          fhicl::Comment("whether to catch the std::out_of_range we throw"),
          true // default
        };

        fhicl::Atom<bool> ManageArtException{
          fhicl::Name("ManageArtException"),
          fhicl::Comment("whether to catch the art::Exception we throw"),
          true // default
        };

      }; // struct Config

      using Parameters = art::EDAnalyzer::Table<Config>;

      /// Constructor.
      Exploder(Parameters const& config);

      /// Executes the iterations.
      virtual void analyze(art::Event const&) override;

    private:
      bool fManageBadAlloc;
      bool fManageOutOfRange;
      bool fManageArtException;

      /// Throws a `std::length_error` exception.
      static unsigned int throwOutOfRange();

      /// Throws a `std::out_of_range` exception.
      static void throwBadAlloc();

      /// Throws a `std::out_of_range` exception.
      static void throwArtException();

    }; // class Exploder

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
lar::example::Exploder::Exploder(Parameters const& config)
  : art::EDAnalyzer(config)
  , fManageBadAlloc(config().ManageBadAlloc())
  , fManageOutOfRange(config().ManageOutOfRange())
  , fManageArtException(config().ManageArtException())
{}

//------------------------------------------------------------------------------
void lar::example::Exploder::analyze(art::Event const&)
{

  //
  // std::length_error
  //
  if (fManageBadAlloc) {
    try {
      throwBadAlloc();
    }
    catch (std::bad_alloc const&) {
    }
  }
  else {
    throwBadAlloc();
  }

  //
  // std::out_of_range
  //
  if (fManageOutOfRange) {
    try {
      throwOutOfRange();
    }
    catch (std::out_of_range const&) {
    }
  }
  else {
    throwOutOfRange();
  }

  //
  // art::Exception
  //
  if (fManageArtException) {
    try {
      throwArtException();
    }
    catch (art::Exception const&) {
    }
  }
  else {
    throwArtException();
  }

} // lar::example::Exploder::analyze()

//------------------------------------------------------------------------------
unsigned int lar::example::Exploder::throwOutOfRange()
{

  std::vector<int> intData(5, 0);

  int intTotal = 0;
  for (unsigned int i = 0; i < 10; ++i) {
    mf::LogVerbatim("Exploder") << "Starting TOOR iteration #" << i;

    // possible std::out_of_range throw
    intTotal += intData.at(i);

  } // for
  mf::LogVerbatim("Exploder") << "TOOR iterations completed.";

  return intTotal;
} // lar::example::Exploder::throwOutOfRange()

//------------------------------------------------------------------------------
void lar::example::Exploder::throwBadAlloc()
{

  using OneMebibyte = std::array<unsigned char, 1048576U>;

  std::vector<OneMebibyte> manyMebibytes;

  // this is allowed, but we don't have enough memory
  mf::LogVerbatim("Exploder") << "Now allocating: " << manyMebibytes.max_size() << " x "
                              << sizeof(OneMebibyte) << " bytes";
  manyMebibytes.resize(manyMebibytes.max_size());

} // lar::example::Exploder::throwBadAlloc()

//------------------------------------------------------------------------------
void lar::example::Exploder::throwArtException()
{

  throw art::Exception(art::errors::LogicError) << "I hate the world and I am vengeful.\n";

} // lar::example::Exploder::throwArtException()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::Exploder)

//------------------------------------------------------------------------------
