/**
 * @file   RemoveIsolatedSpacePoints_module.cc
 * @brief  Module running `lar::example::SpacePointIsolationAlg` algorithm
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 3, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 * Provides:
 *
 * * `lar::example::RemoveIsolatedSpacePoints` module
 *
 */

// LArSoft libraries
#include "larcore/CoreUtils/ServiceUtil.h"
#include "larcore/Geometry/Geometry.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/SpacePointIsolationAlg.h"

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h" // art::ValidHandle
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <memory> // std::make_unique()

namespace lar {
  namespace example {

    /**
     * @brief _art_ module: removes isolated space points.
     * @see @ref RemoveIsolatedSpacePoints "RemoveIsolatedSpacePoints example overview"
     * @ingroup RemoveIsolatedSpacePoints
     *
     * A new collection of space points is added to the event, that contains
     * only the space points that are not isolated.
     *
     * Isolation is determined by the `SpacePointIsolationAlg` algorithm.
     *
     * The space points are not associated to anything.
     *
     * Input
     * ------
     *
     * A collection of `recob::SpacePoint` is required.
     *
     *
     * Output
     * ------
     *
     * A collection of `recob::SpacePoint` is produced, containing copies of
     * the non-isolated inpt points.
     *
     *
     * Configuration parameters
     * =========================
     *
     * * *spacePoints* (input tag, _mandatory_): label of the data product with
     *   input space points
     * * *isolation* (parameter set, _mandatory_): configuration for the
     *   isolation algorithm (see `SpacePointIsolationAlg` documentation)
     *
     */
    class RemoveIsolatedSpacePoints : public art::EDProducer {

    public:
      /// Module configuration data
      struct Config {

        using Name = fhicl::Name;
        using Comment = fhicl::Comment;

        fhicl::Atom<art::InputTag> spacePoints{Name("spacePoints"),
                                               Comment("the space points to be filtered")};

        fhicl::Table<SpacePointIsolationAlg::Config> isolation{
          Name("isolation"),
          Comment("settings for the isolation algorithm")};

      }; // Config

      /// Standard _art_ alias for module configuration table
      using Parameters = art::EDProducer::Table<Config>;

      /// Constructor; see the class documentation for the configuration
      explicit RemoveIsolatedSpacePoints(Parameters const& config);

      virtual void produce(art::Event& event) override;

    private:
      art::InputTag spacePointsLabel; ///< label of the input data product

      SpacePointIsolationAlg isolAlg; ///< instance of the algorithm

    }; // class RemoveIsolatedSpacePoints

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- RemoveIsolatedSpacePoints
//---
lar::example::RemoveIsolatedSpacePoints::RemoveIsolatedSpacePoints(Parameters const& config)
  : EDProducer{config}, spacePointsLabel(config().spacePoints()), isolAlg(config().isolation())
{
  consumes<std::vector<recob::SpacePoint>>(spacePointsLabel);
  produces<std::vector<recob::SpacePoint>>();
} // lar::example::RemoveIsolatedSpacePoints::RemoveIsolatedSpacePoints()

//------------------------------------------------------------------------------
void lar::example::RemoveIsolatedSpacePoints::produce(art::Event& event)
{

  //
  // read the input
  //
  auto spacePointHandle = event.getValidHandle<std::vector<recob::SpacePoint>>(spacePointsLabel);

  //
  // set up the algorithm
  //
  auto const* geom = lar::providerFrom<geo::Geometry>();
  isolAlg.setup(*geom);

  //
  // run the algorithm
  //

  // the return value is a list of indices of non-isolated space points
  auto const& spacePoints = *spacePointHandle;
  std::vector<size_t> socialPointIndices = isolAlg.removeIsolatedPoints(spacePoints);

  //
  // extract and save the results
  //
  auto socialSpacePoints = std::make_unique<std::vector<recob::SpacePoint>>();

  socialSpacePoints->reserve(socialPointIndices.size()); // preallocate
  for (size_t index : socialPointIndices)
    socialSpacePoints->push_back(spacePoints[index]);

  mf::LogInfo("RemoveIsolatedSpacePoints")
    << "Found " << socialSpacePoints->size() << "/" << spacePoints.size()
    << " isolated space points in '" << spacePointsLabel.encode() << "'";

  event.put(std::move(socialSpacePoints));

} // lar::example::RemoveIsolatedSpacePoints::produce()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::RemoveIsolatedSpacePoints)

//------------------------------------------------------------------------------
