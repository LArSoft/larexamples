/**
 * @file   larexamples/DebuggingExamples/CatchException/Disturbance_module.cc
 * @brief  A module throwing a standard library exception.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   March 21, 2018
 * @ingroup CatchException
 */

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "canvas/Utilities/Exception.h"
#include "fhiclcpp/types/Atom.h"

namespace lar {
  namespace example {

    /**
     * @brief A silly module.
     *
     * This module throws and catches a lot of `art::Exception` exceptions.
     * Kids, don't do this at home!
     *
     *
     * Configuration
     * ==============
     *
     * * *NArtExceptions* (integer, mandatory): number of exceptions to throw
     *
     */
    class Disturbance : public art::EDProducer {
    public:
      struct Config {

        fhicl::Atom<unsigned int> NArtExceptions{fhicl::Name("NArtExceptions"),
                                                 fhicl::Comment("number of exceptions to throw")};

      }; // struct Config

      using Parameters = art::EDProducer::Table<Config>;

      /// Constructor.
      Disturbance(Parameters const& config);

      /// Executes the iterations.
      virtual void produce(art::Event&) override;

    private:
      unsigned int fNArtExceptions;

      /// Throws a `std::out_of_range` exception.
      static void throwArtException();

    }; // class Disturbance

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
lar::example::Disturbance::Disturbance(Parameters const& config)
  : EDProducer{config}, fNArtExceptions(config().NArtExceptions())
{}

//------------------------------------------------------------------------------
void lar::example::Disturbance::produce(art::Event&)
{

  //
  // art::Exception
  //
  for (unsigned int i = 0; i < fNArtExceptions; ++i) {
    try {
      throwArtException();
    }
    catch (art::Exception const&) {
    }
  } // for

} // lar::example::Disturbance::produce()

//------------------------------------------------------------------------------
void lar::example::Disturbance::throwArtException()
{

  throw art::Exception(art::errors::LogicError) << "I want to annoy you.\n";

} // lar::example::Disturbance::throwArtException()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::Disturbance)

//------------------------------------------------------------------------------
