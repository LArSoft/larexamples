/**
 * @file   SpacePointMaker_module.cc
 * @brief  Module creating space points for a test
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 3, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 */

// LArSoft libraries
#include "SpacePointTestUtils.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/CoreUtils/ServiceUtil.h"

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "fhiclcpp/types/Atom.h"

// C/C++ standard libraries
#include <memory> // std::make_unique()


namespace lar {
  namespace example {
    namespace tests {

      // BEGIN RemoveIsolatedSpacePoints group ---------------------------------
      /// @ingroup RemoveIsolatedSpacePoints
      /// @{
      /**
       * @brief  Creates a collection of space points
       *
       * A collection of space points is added to the event.
       * The points are spaced by the value of the `spacing` configuration
       * parameter, in a cubic grid. Each TPC is independently filled,
       * so that the TPC centre hosts a space point.
       *
       * The space points are not associated to anything.
       *
       * Configuration parameters
       * =========================
       *
       * * *spacing* (real, _mandatory_): spacing between the points [cm]
       *
       */
      class SpacePointMaker: public art::EDProducer {

          public:

        struct Config {

          using Name    = fhicl::Name;
          using Comment = fhicl::Comment;

          fhicl::Atom<double> spacing{
            Name("spacing"),
            Comment("spacing between points [cm]")
            };

        }; // Config

        using Parameters = art::EDProducer::Table<Config>;

        /// Constructor; see the class documentation for the configuration
        explicit SpacePointMaker(Parameters const& config);

        /// Create and add the points on each event (although they are the same)
        virtual void produce(art::Event& event) override;

          private:

        double spacing; ///< step size [cm]

      }; // class SpacePointMaker

      /// @}
      // END RemoveIsolatedSpacePoints group -----------------------------------


    } // namespace tests
  } // namespace example
} // namespace lar


//------------------------------------------------------------------------------
//--- module implementation
//---

lar::example::tests::SpacePointMaker::SpacePointMaker(Parameters const& config)
  : EDProducer{config}
  , spacing(config().spacing())
{
  produces<std::vector<recob::SpacePoint>>();
} // lar::example::tests::SpacePointMaker::SpacePointMaker()


//------------------------------------------------------------------------------
void lar::example::tests::SpacePointMaker::produce(art::Event& event) {

  //
  // set up
  //

  // container for the data product
  auto spacePoints = std::make_unique<std::vector<recob::SpacePoint>>();

  // acquire the geometry information
  auto const* geom = lar::providerFrom<geo::Geometry>();

  //
  // creation of the points
  //

  // fill each TPC independently
  for (auto const& TPC: geom->IterateTPCs()) {

    FillSpacePointGrid(*spacePoints, TPC, spacing);

  } // for TPC

  //
  // result storage
  //
  mf::LogInfo("SpacePointMaker")
    << "Created " << spacePoints->size() << " space points using spacing "
    << spacing << " cm";

  event.put(std::move(spacePoints));

} // lar::example::tests::SpacePointMaker::produce()


//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::tests::SpacePointMaker)

//------------------------------------------------------------------------------
