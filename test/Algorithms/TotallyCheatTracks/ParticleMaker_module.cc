/**
 * @file   ParticleMaker_module.cc
 * @brief  Module creating simulated particles for a test.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 27, 2017
 * @ingroup TotallyCheatTracks
 *
 */

// LArSoft libraries
#include "larcorealg/Geometry/geo_vectors_utils.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"
#include "nusimdata/SimulationBase/MCParticle.h"

// framework libraries
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// ROOT libraries
#include "TLorentzVector.h"
#include "TVector3.h"

// C/C++ standard libraries
#include <array>
#include <memory> // std::make_unique()

namespace lar {
  namespace example {
    namespace tests {

      // BEGIN TotallyCheatTracks group ----------------------------------------
      /// @ingroup TotallyCheatTracks
      /// @{

      /**
       * @brief  Creates a collection of simulated particles.
       *
       * A collection of `simb::MCParticle` is added to the event.
       * The particles are one starting where the previous one ended (starting
       * from the origin).
       * The configuration specifies the number, type and energy of the
       * particles, and their path length.
       *
       * Configuration parameters
       * =========================
       *
       * * *particles* (list of structures, _mandatory_): each element in the
       *   list is a table with entries:
       *     * *length* (real, _mandatory_): particle path length [cm]
       *     * *energy* (real, _mandatory_): particle energy [GeV]
       *     * *type* (integer, _mandatory_): particle type as PDG ID
       *
       */
      class ParticleMaker : public art::EDProducer {

      public:
        struct ParticleConfig {

          using Name = fhicl::Name;
          using Comment = fhicl::Comment;

          fhicl::Atom<double> length{Name("length"), Comment("length of the particle path [cm]")};

          fhicl::Atom<double> energy{Name("energy"),
                                     Comment("initial energy of the particle [GeV]")};

          fhicl::Atom<int> type{Name("type"), Comment("particle type (as PDG ID)")};

        }; // struct ParticleConfig

        struct Config {

          using Name = fhicl::Name;
          using Comment = fhicl::Comment;

          fhicl::Sequence<fhicl::Table<ParticleConfig>> particles{
            Name("particles"),
            Comment("list of particle specification")};

        }; // struct Config

        using Parameters = art::EDProducer::Table<Config>;

        /// Constructor; see the class documentation for the configuration.
        explicit ParticleMaker(Parameters const& config);

        /// Create and add the particles (the same for all events).
        virtual void produce(art::Event& event) override;

      private:
        struct ParticleSpecs {
          double length;
          double energy;
          int type;

          ParticleSpecs(ParticleConfig const& config)
            : length(config.length()), energy(config.energy()), type(config.type())
          {}
        }; // ParticleSpecs

        std::vector<ParticleSpecs> fParticleSpecs; ///< Settings for particles.

      }; // class ParticleMaker

      /// @}
      // END TotallyCheatTracks group ------------------------------------------

    } // namespace tests
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- module implementation
//---

lar::example::tests::ParticleMaker::ParticleMaker(Parameters const& config) : EDProducer{config}
{

  auto const& particleSpecs = config().particles();
  fParticleSpecs.assign(particleSpecs.begin(), particleSpecs.end());

  // consumes: nothing

  // produces:
  produces<std::vector<simb::MCParticle>>();

} // lar::example::tests::ParticleMaker::ParticleMaker()

//------------------------------------------------------------------------------
void lar::example::tests::ParticleMaker::produce(art::Event& event)
{

  //
  // set up
  //

  // container for the data product
  auto particles = std::make_unique<std::vector<simb::MCParticle>>();

  //
  // creation of the particles
  //
  static std::array<TVector3, 6U> const Dirs = {
    {geo::vect::rounded01(geo::Xaxis<TVector3>(), 1e-8),
     geo::vect::rounded01(geo::Yaxis<TVector3>(), 1e-8),
     geo::vect::rounded01(geo::Zaxis<TVector3>(), 1e-8),
     geo::vect::rounded01(-geo::Xaxis<TVector3>(), 1e-8),
     geo::vect::rounded01(-geo::Yaxis<TVector3>(), 1e-8),
     geo::vect::rounded01(-geo::Zaxis<TVector3>(), 1e-8)}}; // Dirs

  int trackID = 0;
  TLorentzVector pos;
  for (auto const& specs : fParticleSpecs) {

    int const motherID = trackID - 1;
    if (motherID >= 0) (*particles)[motherID].AddDaughter(trackID);

    particles->emplace_back(trackID // track ID
                            ,
                            specs.type // pdg
                            ,
                            "magic" // process
                            ,
                            motherID // mother
    );
    simb::MCParticle& particle = particles->back();

    auto const& dir = Dirs[trackID % 6];
    TLorentzVector const mom{specs.energy * dir, specs.energy};
    unsigned int const nSteps = std::ceil(specs.length);
    particle.AddTrajectoryPoint(pos, mom);
    for (unsigned int i = 1; i <= nSteps; ++i) {
      double const stepSize = std::min(specs.length - double(i - 1), 1.0);
      pos += TLorentzVector{dir * stepSize, 1.0};
      particle.AddTrajectoryPoint(pos, mom);
    } // for

    ++trackID;

  } // for specs

  //
  // result storage
  //
  mf::LogInfo("ParticleMaker") << "Created " << particles->size() << " space points.";

  event.put(std::move(particles));

} // lar::example::tests::ParticleMaker::produce()

//------------------------------------------------------------------------------
DEFINE_ART_MODULE(lar::example::tests::ParticleMaker)

//------------------------------------------------------------------------------
