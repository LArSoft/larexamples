/**
 * @file   larexamples/Algorithms/TotallyCheatTracks/CheatTrackData/CheatTrack.h
 * @brief  Pseudo-track data product for TotallyCheatTracks example.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 26, 2017
 * @version 10
 * @ingroup TotallyCheatTracks
 *
 * This is a header-only library (no implementation file).
 *
 */

#ifndef LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_CHEATTRACKDATA_CHEATTRACK_H
#define LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_CHEATTRACKDATA_CHEATTRACK_H

// LArSoft libraries
#include "lardataobj/RecoBase/Trajectory.h"

// ROOT libraries
#include "TDatabasePDG.h"

// C/C++ standard libraries
#include <ostream>
#include <string>
#include <utility> // std::move(), std::forward()

namespace lar {

  namespace example {
    // BEGIN TotallyCheatTracks group ------------------------------------------
    /// @ingroup TotallyCheatTracks
    /// @{

    /**
     * @brief Pseudo-track object for TotallyCheatTracks example.
     *
     * This class represents a reconstructed track via a trajectory in phase
     * space (position plus momentum), sampled in many points, and in addition
     * a particle identification code representing the type of the original
     * particle.
     *
     * It is expected to be unrealistically precise for a reconstructed object,
     * by being constructed from simulated "truth" information.
     *
     * This track object does _not_ implement the standard interface of a
     * LArSoft track (`recob::Track`).
     * It _can_ present a standard LArSoft `recob::Trajectory` though.
     *
     */
    class CheatTrack {

    public:
      using PDGID_t = int; ///< Type of the particle ID.

      /// Value of a particle ID that denotes it as invalid.
      static constexpr PDGID_t InvalidParticleID = 0;

      /// Default constructor, only for ROOT I/O (do not use it!).
      CheatTrack() = default;

      /**
       * @brief Constructor from trajectory (stolen) and particle ID.
       * @param traj the reconstructed trajectory of the track
       * @param pid particle ID, in PDG standard
       *
       * The trajectory in `traj` is moved into this object, and it will not be
       * valid in the caller scope any more.
       */
      CheatTrack(recob::Trajectory&& traj, PDGID_t pid) : fTraj(std::move(traj)), fPDGID(pid) {}

      /// Returns the trajectory of this track.
      recob::Trajectory const& trajectory() const { return fTraj; }

      // --- BEGIN access to data ----------------------------------------------
      /// @{
      /// @name Access to data

      /// Returns the initial momentum of the particle [MeV]
      double momentum() const { return trajectory().StartMomentum(); }

      /// Returns the particle ID, in PDG standard.
      PDGID_t particleId() const { return fPDGID; }

      /// Returns whether the particle ID is valid.
      bool hasParticleId() const { return particleId() != InvalidParticleID; }

      /// @}
      // --- END access to data ------------------------------------------------

      // --- BEGIN printing data -----------------------------------------------
      /// @{
      /// @name Printing data

      /// Default verbosity level.
      static constexpr unsigned int DefaultDumpVerbosity = 1U;

      /// Maximum verbosity level.
      static constexpr unsigned int MaxDumpVerbosity = recob::Trajectory::MaxDumpVerbosity;

      //@{
      /**
       * @brief Prints the content of this object into an output stream.
       * @tparam Stream type of the output text stream
       * @param out the output text stream
       * @param verbosity the amount of information printed
       *                  (_default: `DefaultDumpVerbosity`_)
       * @param indent indentation string for all output except the first line
       *               (_default: none_)
       * @param firstIndent indentation string for the first line
       *                    (_default: as `indent`_)
       *
       * Verbosity level is the same as the one of `recob::Trajectory::Dump()`.
       * In addition, the momentum and particle ID are always printed.
       *
       */
      template <typename Stream>
      void dump(Stream&& out,
                unsigned int verbosity,
                std::string indent,
                std::string firstIndent) const;
      template <typename Stream>
      void dump(Stream&& out,
                unsigned int verbosity = DefaultDumpVerbosity,
                std::string indent = "") const
      {
        dump(std::forward<Stream>(out), verbosity, indent, indent);
      }
      //@}

      ///@}

      // --- END printing data -------------------------------------------------

    private:
      recob::Trajectory fTraj;            ///< The trejectory of this track.
      PDGID_t fPDGID = InvalidParticleID; ///< Particle ID in PDG standard.

    }; // class CheatTrack

    /// Prints the content of the track into a text stream.
    /// @related lar::example::CheatTrack
    /// @ingroup TotallyCheatTracks
    inline std::ostream& operator<<(std::ostream& out, lar::example::CheatTrack const& track)
    {
      track.dump(out);
      return out;
    }

    /// @}
    // END TotallyCheatTracks group ------------------------------------------

  } // namespace example

} // namespace lar

//------------------------------------------------------------------------------
//---  template implementation
//------------------------------------------------------------------------------
template <typename Stream>
void lar::example::CheatTrack::dump(Stream&& out,
                                    unsigned int verbosity,
                                    std::string indent,
                                    std::string firstIndent) const
{

  // we could use ROOT's TDatabasePDG to get the name of the ID, but we'd rather
  // not depend on ROOT here...
  out << firstIndent << "particle: ";
  auto const* pPDGinfo = TDatabasePDG::Instance()->GetParticle(particleId());
  if (pPDGinfo)
    out << pPDGinfo->GetName() << " (ID=" << particleId() << ")";
  else
    out << "ID " << particleId();
  out << "; momentum: " << momentum() << " GeV/c; ";
  trajectory().Dump(std::forward<Stream>(out), verbosity, indent, "");

} // lar::example::CheatTrack::dump()

//------------------------------------------------------------------------------

#endif // LAREXAMPLES_ALGORITHMS_TOTALLYCHEATTRACKS_CHEATTRACKDATA_CHEATTRACK_H
