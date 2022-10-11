/**
 * @file   CheckDataProductSize_module.cc
 * @brief  Checks the size of a collection
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 3, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 */

// LArSoft libraries
#include "lardataobj/RecoBase/SpacePoint.h"

// framework libraries
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h" // art::ValidHandle
#include "canvas/Utilities/InputTag.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalAtom.h"

// C/C++ standard libraries

namespace lar {
  namespace example {
    namespace tests {

      // BEGIN RemoveIsolatedSpacePoints group -----------------------------------
      /// @ingroup RemoveIsolatedSpacePoints
      /// @{
      /**
     * @brief Checks the size of the specified collection
     *
     * Throws an exception if the size of the collection in the specified
     * data product is not as expected.
     * The expectation can be expressed directly as a number of elements in the
     * collection, or indirectly as the requirement that the tested collection
     * has the same size as another one (still of `recob::SpacePoint`).
     *
     * Configuration parameters
     * =========================
     *
     * * *inputLabel* (input tag, _mandatory_): label of the data product with
     *   the collection
     * * *expectedSize* (integer): expected number of elements in
     *   the collection
     * * *sameSizeAs* (inputTag): expected number of elements is the same as
     *   this other data product
     *
     */
      class CheckDataProductSize : public art::EDAnalyzer {

        using Data_t = recob::SpacePoint;
        using OtherData_t = recob::SpacePoint;

      public:
        struct Config {

          using Name = fhicl::Name;
          using Comment = fhicl::Comment;

          fhicl::Atom<art::InputTag> inputLabel{Name("inputLabel"),
                                                Comment("label of the data product to be checked")};

          fhicl::OptionalAtom<size_t> expectedSize{
            Name("expectedSize"),
            Comment("number of expected entries in the data product")};

          fhicl::OptionalAtom<art::InputTag> sameSizeAs{
            Name("sameSizeAs"),
            Comment("label of a data product with the same size as the input")};

        }; // Config

        using Parameters = art::EDAnalyzer::Table<Config>;

        /// Constructor; see the class documentation for the configuration
        explicit CheckDataProductSize(Parameters const& config)
          : art::EDAnalyzer(config), inputLabel(config().inputLabel())
        {
          doCheckExpectedSize = config().expectedSize(expectedSize);
          doCheckSameSize = config().sameSizeAs(sameSizeAs);
          consumes<std::vector<Data_t>>(inputLabel);
        }

        virtual void analyze(art::Event const& event) override;

      private:
        art::InputTag inputLabel; ///< label of the input data product

        bool doCheckExpectedSize; ///< check that the size is the specified one
        bool doCheckSameSize;     ///< check that size is the same as another product

        size_t expectedSize;      ///< expected size of the data product collection
        art::InputTag sameSizeAs; ///< label of the data product with same size

      }; // class CheckDataProductSize

      /// @}
      // END RemoveIsolatedSpacePoints group -------------------------------------

    } // namespace tests
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- CheckDataProductSize
//---
void lar::example::tests::CheckDataProductSize::analyze(art::Event const& event)
{

  //
  // read the input
  //
  auto collectionHandle = event.getValidHandle<std::vector<Data_t>>(inputLabel);

  if (doCheckExpectedSize) {
    if (collectionHandle->size() != expectedSize) {
      throw cet::exception("CheckDataProductSize")
        << "Data product '" << inputLabel.encode() << "' has " << collectionHandle->size()
        << " elements, " << expectedSize << " were expected!\n";
    }
  } // if doCheckExpectedSize

  if (doCheckSameSize) {
    auto otherCollectionHandle = event.getValidHandle<std::vector<OtherData_t>>(sameSizeAs);
    if (collectionHandle->size() != otherCollectionHandle->size()) {
      throw cet::exception("CheckDataProductSize")
        << "Data product '" << inputLabel.encode() << "' has " << collectionHandle->size()
        << " elements, " << expectedSize << " were expected as in '" << sameSizeAs.encode()
        << "'!\n";
    }
  } // if doCheckSameSize

} // lar::example::tests::CheckDataProductSize::analyze()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::tests::CheckDataProductSize)

//------------------------------------------------------------------------------
