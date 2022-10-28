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
#include "larcore/CoreUtils/ServiceUtil.h"
#include "larcore/Geometry/Geometry.h"
#include "lardataobj/RecoBase/SpacePoint.h"

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/types/Atom.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <memory> // std::make_unique()

namespace lar::example::tests {

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
  class SpacePointMaker : public art::EDProducer {
  public:
    struct Config {
      fhicl::Atom<double> spacing{fhicl::Name("spacing"),
                                  fhicl::Comment("spacing between points [cm]")};
    };

    using Parameters = art::EDProducer::Table<Config>;

    /// Constructor; see the class documentation for the configuration
    explicit SpacePointMaker(Parameters const& config);

    /// Create and add the points on each event (although they are the same)
    void produce(art::Event& event) override;

  private:
    double spacing; ///< step size [cm]

  }; // class SpacePointMaker

  /// @}
  // END RemoveIsolatedSpacePoints group -----------------------------------

} // namespace lar::example::testss

//------------------------------------------------------------------------------
//--- module implementation
//---

lar::example::tests::SpacePointMaker::SpacePointMaker(Parameters const& config)
  : EDProducer{config}, spacing(config().spacing())
{
  produces<std::vector<recob::SpacePoint>>();
}

//------------------------------------------------------------------------------
void lar::example::tests::SpacePointMaker::produce(art::Event& event)
{
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
  for (auto const& TPC : geom->Iterate<geo::TPCGeo>()) {
    FillSpacePointGrid(*spacePoints, TPC, spacing);
  }

  //
  // result storage
  //
  mf::LogInfo("SpacePointMaker") << "Created " << spacePoints->size()
                                 << " space points using spacing " << spacing << " cm";
  event.put(std::move(spacePoints));
}

DEFINE_ART_MODULE(lar::example::tests::SpacePointMaker)
