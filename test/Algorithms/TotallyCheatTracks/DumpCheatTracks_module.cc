/**
 * @file   DumpCheatTracks_module.cc
 * @brief  Dumps a `lar::example::CheatTrack` collection and associations.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 27, 2017
 * @ingroup TotallyCheatTracks
 *
 */

// LArSoft libraries
#include "larexamples/Algorithms/TotallyCheatTracks/CheatTrackData/CheatTrack.h"
#include "nusimdata/SimulationBase/MCParticle.h"

// framework libraries
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h" // art::ValidHandle
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/FindOneP.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Utilities/InputTag.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/types/Atom.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Boost
#include <boost/test/test_tools.hpp> // BOOST_CHECK_EQUAL()

// C/C++ standard libraries

namespace lar::example::tests {

  // BEGIN TotallyCheatTracks group ----------------------------------------
  /// @ingroup TotallyCheatTracks
  /// @{

  /**
   * @brief Dumps `lar::example::CheatTrack` data product and associations.
   *
   * The expectation can be expressed directly as a number of elements in the
   * collection, or indirectly as the requirement that the tested collection
   * has the same size as another one (still of `recob::SpacePoint`).
   *
   * Configuration parameters
   * =========================
   *
   * * *tracks* (input tag, _mandatory_): label of the data product with
   *   the collection of cheat tracks (and associations to particles)
   * * *expectedCount* (integer, _mandatory_): expected number of tracks in
   *   the collection
   *
   */
  class DumpCheatTracks : public art::EDAnalyzer {
  public:
    struct Config {

      using Name = fhicl::Name;
      using Comment = fhicl::Comment;

      fhicl::Atom<art::InputTag> tracks{Name("tracks"),
                                        Comment("label of the data product with the cheat tracks")};

      fhicl::Atom<unsigned int> expectedCount{
        Name("expectedCount"),
        Comment("number of expected tracks in the data product")};

    }; // Config

    using Parameters = art::EDAnalyzer::Table<Config>;

    /// Constructor; see the class documentation for the configuration
    explicit DumpCheatTracks(Parameters const& config)
      : art::EDAnalyzer(config)
      , fTrackTag(config().tracks())
      , fExpectedCount(config().expectedCount())
    {
      consumes<std::vector<lar::example::CheatTrack>>(fTrackTag);
      consumes<art::Assns<lar::example::CheatTrack, simb::MCParticle>>(fTrackTag);
    }

  private:
    void analyze(art::Event const& event) override;

    art::InputTag fTrackTag;     ///< Label of the input data product.
    unsigned int fExpectedCount; ///< Expected number of tracks.
  };                             // class DumpCheatTracks

  /// @}
  // END TotallyCheatTracks group ------------------------------------------

} // namespace lar::example::tests

//------------------------------------------------------------------------------
//--- DumpCheatTracks
//---
void lar::example::tests::DumpCheatTracks::analyze(art::Event const& event)
{

  //
  // read the input
  //
  auto const& trackHandle = event.getValidHandle<std::vector<lar::example::CheatTrack>>(fTrackTag);
  auto const& tracks = *trackHandle;
  art::FindOneP<simb::MCParticle> trackToPart{trackHandle, event, fTrackTag};

  std::size_t const nTracks = tracks.size();
  BOOST_TEST(nTracks == fExpectedCount);

  for (std::size_t iTrack = 0U; iTrack < nTracks; ++iTrack) {

    lar::example::CheatTrack const& track = tracks[iTrack];
    art::Ptr<simb::MCParticle> const& partPtr = trackToPart.at(iTrack);

    mf::LogVerbatim log("DumpCheatTracks");
    log << "[#" << iTrack << "] " << track;
    if (partPtr) { log << " associated to particle:\n    " << *partPtr; }
    else {
      log << " not associated to any particle";
    }

  } // for (iTrack)

} // lar::example::tests::DumpCheatTracks::analyze()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::tests::DumpCheatTracks)
