/**
 * @file   TotallyCheatTracker_module.cc
 * @brief  Module running `lar::example::TotallyCheatTrackingAlg` algorithm.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 26, 2017
 * @ingroup TotallyCheatTracks
 *
 * Provides:
 *
 * * `lar::example::TotallyCheatTracker` module
 *
 */

// LArSoft libraries
#include "larexamples/Algorithms/TotallyCheatTracks/CheatTrackData/CheatTrack.h"
#include "larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.h"
#include "nusimdata/SimulationBase/MCParticle.h"

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h" // art::ValidHandle<>
#include "art/Persistency/Common/PtrMaker.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C/C++ standard libraries
#include <memory> // std::make_unique()

namespace lar {
  namespace example {

    /**
     * @brief Module: creates tracks from simulated particles.
     * @ingroup TotallyCheatTracks
     * @see @ref TotallyCheatTracks "TotallyCheatTracks example overview"
     *
     * This module creates one LArSoft reconstructed track
     * (`lar::example::CheatTrack`) for each input simulated particle
     * (`simb::MCParticle`) passing the selection criteria.
     *
     * Input
     * ------
     *
     * A collection of `simb::MCParticle` is required.
     *
     *
     * Output
     * ------
     *
     * A collection of `lar::example::CheatTrack` is produced, a one-to-one
     * association of each of them to its original `simb::MCParticle`.
     * Associations are inserted in the same order as the trajectories.
     *
     *
     * Configuration parameters
     * -------------------------
     *
     * * *particles* (input tag, default: `largeant`): label of the data product
     *     with input simulated particles
     * * *minLength* (parameter set, default: 1.0): minimum length of the
     *     particle trajectory, in centimeters
     * * *minEnergy* (parameter set, default: 1.0): minimum energy of the
     *     particle, in GeV
     *
     */
    class TotallyCheatTracker : public art::EDProducer {

    public:
      /// Module configuration data
      struct Config {

        using Name = fhicl::Name;
        using Comment = fhicl::Comment;

        fhicl::Atom<art::InputTag> particles{
          Name("particles"),
          Comment("the data product of simulated particles to be processed"),
          "largeant" // default
        };

        fhicl::Atom<double> minLength{
          Name("minLength"),
          Comment("minimum length of particle trajectory [cm]"),
          1.0 // default
        };

        fhicl::Atom<double> minEnergy{
          Name("minEnergy"),
          Comment("minimum energy of particle [GeV]"),
          1.0 // default
        };

        fhicl::Table<lar::example::TotallyCheatTrackingAlg::Config> algoConfig{
          Name("algoConfig"),
          Comment("configuration of TotallyCheatTrackingAlg algorithm"),
          lar::example::TotallyCheatTrackingAlg::Config{} // default
        };

      }; // Config

      /// Standard _art_ alias for module configuration table
      using Parameters = art::EDProducer::Table<Config>;

      /// Constructor; see the class documentation for the configuration
      explicit TotallyCheatTracker(Parameters const& config);

      virtual void produce(art::Event& event) override;

      /// Returns whether the `particle` satisfies the selection criteria.
      bool acceptParticle(simb::MCParticle const& particle) const;

    private:
      art::InputTag particleTag; ///< Label of the input data product.

      double minLength; ///< Minimum particle length [cm]
      double minEnergy; ///< Minimum energy [GeV]

      /// Reconstruction algorithm.
      lar::example::TotallyCheatTrackingAlg trackMaker;

    }; // class TotallyCheatTracker

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- TotallyCheatTracker
//---
lar::example::TotallyCheatTracker::TotallyCheatTracker(Parameters const& config)
  : EDProducer{config}
  , particleTag(config().particles())
  , minLength(config().minLength())
  , minEnergy(config().minEnergy())
  , trackMaker(config().algoConfig())
{

  consumes<std::vector<simb::MCParticle>>(particleTag);

  produces<std::vector<lar::example::CheatTrack>>();
  produces<art::Assns<lar::example::CheatTrack, simb::MCParticle>>();

} // lar::example::TotallyCheatTracker::TotallyCheatTracker()

//------------------------------------------------------------------------------
void lar::example::TotallyCheatTracker::produce(art::Event& event)
{

  //
  // read the input
  //
  auto particleHandle = event.getValidHandle<std::vector<simb::MCParticle>>(particleTag);
  auto const& particles = *particleHandle;

  //
  // prepare the output structures
  //
  auto tracks = std::make_unique<std::vector<lar::example::CheatTrack>>();
  auto trackToPart = std::make_unique<art::Assns<lar::example::CheatTrack, simb::MCParticle>>();

  art::PtrMaker<simb::MCParticle> makePartPtr(event, particleHandle.id());
  art::PtrMaker<lar::example::CheatTrack> makeTrackPtr(event);

  //
  // set up the algorithm
  //
  trackMaker.setup();

  //
  // run the algorithm
  //
  for (std::size_t iParticle = 0U; iParticle < particles.size(); ++iParticle) {
    simb::MCParticle const& particle = particles[iParticle];

    //
    // apply filters
    //
    if (!acceptParticle(particle)) continue;

    //
    // run the algorithm
    //
    tracks->push_back(trackMaker.makeTrack(particle));

    //
    // create the association
    //
    auto const iTrack = tracks->size() - 1;
    art::Ptr<lar::example::CheatTrack> const trackPtr = makeTrackPtr(iTrack);
    art::Ptr<simb::MCParticle> const partPtr = makePartPtr(iParticle);
    trackToPart->addSingle(trackPtr, partPtr);

  } // for

  //
  // store the data products into the event (and print a short summary)
  //
  mf::LogInfo("TotallyCheatTracker")
    << "Reconstructed " << tracks->size() << " tracks out of " << particleHandle->size()
    << " particles from '" << particleTag.encode() << "'";

  event.put(std::move(tracks));
  event.put(std::move(trackToPart));

} // lar::example::TotallyCheatTracker::produce()

//------------------------------------------------------------------------------
bool lar::example::TotallyCheatTracker::acceptParticle(simb::MCParticle const& particle) const
{
  // skip empty particle
  if (particle.NumberTrajectoryPoints() == 0) return false;

  // energy at the first point
  if (particle.E() < minEnergy) return false;

  // path length
  if (particle.Trajectory().TotalLength() < minLength) return false;

  // good enough!
  return true;

} // lar::example::TotallyCheatTracker::acceptParticle()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::TotallyCheatTracker)

//------------------------------------------------------------------------------
