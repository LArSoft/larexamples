/**
 * @file   larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.cxx
 * @brief  Algorithm to "reconstruct" trajectories from simulated particles.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 26, 2017
 * @see    larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.h
 * @ingroup TotallyCheatTracks
 *
 */

// algorithm header
#include "larexamples/Algorithms/TotallyCheatTracks/TotallyCheatTrackingAlg.h"

// LArSoft libraries
#include "larcorealg/Geometry/geo_vectors_utils.h" // geo::vect namespace

// nusimdata libraries
#include "nusimdata/SimulationBase/MCParticle.h"

// C/C++ standard libraries
#include <utility> // std::move()


//------------------------------------------------------------------------------
//--- lar::example::TotallyCheatTrackingAlg implementation
//------------------------------------------------------------------------------
lar::example::CheatTrack lar::example::TotallyCheatTrackingAlg::makeTrack
  (simb::MCParticle const& mcParticle) const
{
  recob::Trajectory::Positions_t pos;
  recob::Trajectory::Momenta_t mom;

  for (auto const& pair: mcParticle.Trajectory()) {
    pos.push_back
      (geo::vect::rounded01(geo::vect::toPoint(pair.first.Vect()), 1e-8));
    mom.push_back
      (geo::vect::rounded01(geo::vect::toVector(pair.second.Vect()), 1e-8));
  } // for

  return { { std::move(pos), std::move(mom), true }, mcParticle.PdgCode() };
} // lar::example::TotallyCheatTrackingAlg::makeTrack()


//------------------------------------------------------------------------------
