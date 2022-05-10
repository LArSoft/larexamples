/**
 * @file   larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.h
 * @brief  Algorithm to "reconstruct" trajectories from simulated particles.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 26, 2017
 * @see    larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.cxx
 * @ingroup TotallyCheatTracks
 *
 */

#ifndef LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_TOTALLYCHEATTRACKINGALG_H
#define LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_TOTALLYCHEATTRACKINGALG_H

// LArSoft libraries
#include "larexamples/Algorithms/TotallyCheatTracks/CheatTrackData/CheatTrack.h"

namespace simb { class MCParticle; }

namespace lar {
  namespace example {


    /**
     * @brief Reconstructs tracks from simulated particles.
     * @ingroup TotallyCheatTracks
     * @see @ref TotallyCheatTracks "TotallyCheatTracks example overview"
     *
     * This totally cheating tracking algorithm will return one
     * `lar::example::CheatTrack` object for each `simb::MCParticle` inserted.
     * The information of the track will perfectly reflect the content of
     * the simulated particle trajectory.
     *
     * @note The produced track is not a standard LArSoft `recob::Track`.
     *
     * Example of usage:
     * -------------------------------------------------------------------{.cpp}
     * lar::example::TotallyCheatTrackingAlg cheaterAlg({});
     * cheaterAlg.setup();
     *
     * std::vector<lar::example::CheatTrack> tracks;
     * for (auto const& mcParticle: mcParticles)
     *   tracks.push_back(cheaterAlg.makeTrack(mcParticle));
     * -------------------------------------------------------------------
     * (assuming `mcParticles` a collection of `simb::MCParticle`).
     *
     *
     * Configuration
     * ==============
     *
     * Currently, none.
     *
     *
     * Dependencies and setup
     * =======================
     *
     * Currently none.
     *
     */
    class TotallyCheatTrackingAlg {

        public:
      struct Config {};

      /// Constructor: accepts a configuration (currently unused).
      TotallyCheatTrackingAlg(Config const&) {}


      /// Set up the algorithm (currently no operation).
      void setup() {}

      /**
       * @brief Returns a reconstructed track from the specified particle.
       * @param mcParticle the simulated particle to be reconstructed
       * @return a reconstructed `lar::example::CheatTrack` track
       *
       * The reconstucted track has one trajectory point per trajectory
       * point of the input particle.
       */
      lar::example::CheatTrack makeTrack
        (simb::MCParticle const& mcParticle) const;


    }; // class TotallyCheatTrackingAlg


  } // namespace example
} // namespace lar



#endif // LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_TOTALLYCHEATTRACKINGALG_H
