/**
 * @file   AtomicNumberTest_module.cc
 * @brief  A simple test for AtomicNumberService example service
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   April 14, 2016
 * @see    AtomicNumberService.h
 *
 */

// LArSoft libraries
#include "larexamples/Services/AtomicNumber/Providers/AtomicNumber.h"
#include "larexamples/Services/AtomicNumber/Services/AtomicNumberService.h"

// framework and support libraries
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "canvas/Utilities/Exception.h"
#include "fhiclcpp/ParameterSet.h"

namespace lar {
  namespace example {
    namespace tests {

      /**
          * @brief Tests AtomicNumberService
          *
          * Configuration parameters
          * -------------------------
          *
          * - *AtomicNumber* (unsigned integer, mandatory): the expected atomic
          *   number
          *
          */
      class AtomicNumberTest : public art::EDAnalyzer {
      public:
        explicit AtomicNumberTest(fhicl::ParameterSet const& pset);

        /// We don't really care about events
        virtual void analyze(art::Event const&) override {}

        /// Test happens at begin job
        virtual void beginJob() override;

      private:
        unsigned int expected_Z; ///< expected atomic number

      }; // class AtomicNumberTest

    } // namespace tests
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------

lar::example::tests::AtomicNumberTest::AtomicNumberTest(fhicl::ParameterSet const& pset)
  : art::EDAnalyzer(pset), expected_Z(pset.get<unsigned int>("AtomicNumber"))
{}

void lar::example::tests::AtomicNumberTest::beginJob()
{

  auto const* Zprov = lar::providerFrom<lar::example::AtomicNumberService>();

  unsigned int const Z = Zprov->Z();
  if (Z != expected_Z) {
    throw art::Exception(art::errors::LogicError)
      << "Unexpected atomic number " << Z << ", should be " << expected_Z;
  }

} // lar::example::tests::AtomicNumberTest::beginJob()

DEFINE_ART_MODULE(lar::example::tests::AtomicNumberTest)