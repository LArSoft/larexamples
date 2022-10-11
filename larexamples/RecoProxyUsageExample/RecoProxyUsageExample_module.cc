#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"
#include "messagefacility/MessageLogger/MessageLogger.h" // mf::LogVerbatim

#include "lardataobj/RecoBase/MCSFitResult.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/VertexAssnMeta.h"

#include "lardata/RecoBaseProxy/Track.h" //needed only if you do use the proxies
//#include "lardata/RecoBaseProxy/ProxyBase.h" //needed if you use proxies (already included above if using the Track proxy)

#include "canvas/Persistency/Common/FindMany.h"  //needed only if you do not use the proxies
#include "canvas/Persistency/Common/FindManyP.h" //needed only if you do not use the proxies

//
/**
   * @file  larexamples/RecoProxyUsageExample/RecoProxyUsageExample_module.cc
   * @class RecoProxyUsageExample
   *
   * @brief Example of analyzer accessing vertices, tracks, and hits, using RecoBaseProxy.
   *
   * The corresponding code without using RecoBaseProxy is also provided as a reference.
   * @see @ref LArSoftProxies.
   *
   * @author  G. Cerati (FNAL, MicroBooNE)
   * @date    2018
   * @version 1.0
   */
//

class RecoProxyUsageExample : public art::EDAnalyzer {
public:
  struct Config {
    fhicl::Atom<art::InputTag> trackInputTag{fhicl::Name("trackInputTag"),
                                             fhicl::Comment("data product tag for tracks")};
    fhicl::Atom<art::InputTag> vertexInputTag{fhicl::Name("vertexInputTag"),
                                              fhicl::Comment("data product tag for vertices")};
    fhicl::Atom<art::InputTag> mcsInputTag{
      fhicl::Name("mcsInputTag"),
      fhicl::Comment("data product tag for track momentum reconstruction")};
  }; // Config

  using Parameters = art::EDAnalyzer::Table<Config>;

  explicit RecoProxyUsageExample(Parameters const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  RecoProxyUsageExample(RecoProxyUsageExample const&) = delete;
  RecoProxyUsageExample(RecoProxyUsageExample&&) = delete;
  RecoProxyUsageExample& operator=(RecoProxyUsageExample const&) = delete;
  RecoProxyUsageExample& operator=(RecoProxyUsageExample&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

private:
  // Declare member data here.
  art::InputTag trkTag, vtxTag, mcsTag;
};

RecoProxyUsageExample::RecoProxyUsageExample(Parameters const& config)
  : EDAnalyzer(config)
  , trkTag(config().trackInputTag())
  , vtxTag(config().vertexInputTag())
  , mcsTag(config().mcsInputTag())
{}

void RecoProxyUsageExample::analyze(art::Event const& e)
{

  //
  // Example using proxies.
  //

  //
  // Get vertex collection proxy and associated tracks, with meta data
  auto const& vertices = proxy::getCollection<std::vector<recob::Vertex>>(
    e, vtxTag, proxy::withAssociatedMeta<recob::Track, recob::VertexAssnMeta>());
  //
  // Get track collection proxy and parallel mcs fit data (associated hits loaded by default)
  // Note: if tracks were produced from a TrackTrajectory collection you could access the original trajectories adding ',proxy::withOriginalTrajectory()' to the list of arguments
  auto const& tracks = proxy::getCollection<proxy::Tracks>(
    e, trkTag, proxy::withParallelData<recob::MCSFitResult>(mcsTag));
  //
  // Loop over vertex proxies (get recob::Vertex with '->')
  for (const auto& v : vertices) {
    mf::LogVerbatim("ProxyExample") << "vertex pos=" << v->position() << " chi2=" << v->chi2();
    //
    // Get tracks(+meta) associated to vertex, and loop over them
    const auto& assocTracks = v.get<recob::Track>();
    for (const auto& trackAssn : assocTracks) {
      //
      // Note that here we access the methods of recob::Track using '->' and that we get the recob::VertexAssnMeta with '.data()'
      mf::LogVerbatim("ProxyExample")
        << "track with key=" << trackAssn.key() << " and length=" << trackAssn->Length()
        << " has propDist from vertex=" << trackAssn.data().propDist();
      //
      // Now get the track proxy from the key, and use it to access the parallel MCSFitResult; note that the track proxy has already access to the associated hits
      const auto& track = tracks[trackAssn.key()];
      const recob::MCSFitResult& assocMCS = track.get<recob::MCSFitResult>();
      //
      // Print some information; here we access the methods of recob::Track using '->' and proxy::Track with '.'
      // Note: if the original trajectories were associated to the proxy, you could get the original/unfitted length with 'track(proxy::Tracks::Unfitted)->Length()'
      mf::LogVerbatim("ProxyExample")
        << "\tCountValidPoints=" << track->CountValidPoints() << " and nHits=" << track.nHits()
        << " and MCSMom=" << assocMCS.bestMomentum();
      //
      // Now loop over the associated hits from the track proxy
      if (track.nHits() < 50) {
        for (const art::Ptr<recob::Hit>& h : track.hits()) {
          mf::LogVerbatim("ProxyExample")
            << "\t\thit wire=" << h->WireID() << " peak time=" << h->PeakTime();
        }
      }
    } // for associated tracks
  }   // for vertices

  //
  // Same example without using proxies.
  //

  //
  // Get vertex collection handle and get associated tracks with meta data using FindManyP
  auto const& vertexHandle = e.getValidHandle<std::vector<recob::Vertex>>(vtxTag);
  auto const& vertexColl = *vertexHandle;
  art::FindManyP<recob::Track, recob::VertexAssnMeta> assocTracksWithMeta(vertexHandle, e, vtxTag);
  //
  // Get track collection handle, get associated hits using FindMany, and get mcs collection (parallel to track collection)
  auto const& trackHandle = e.getValidHandle<std::vector<recob::Track>>(trkTag);
  art::FindMany<recob::Hit> assocHits(trackHandle, e, trkTag);
  auto const& mcsColl = *(e.getValidHandle<std::vector<recob::MCSFitResult>>(mcsTag));
  //
  // Loop over the vertex collection
  for (size_t iv = 0; iv < vertexColl.size(); ++iv) {
    const recob::Vertex& v = vertexColl[iv];
    mf::LogVerbatim("ProxyExample") << "vertex pos=" << v.position() << " chi2=" << v.chi2();
    //
    // Get tracks(+meta) associated to vertex, and loop over them
    auto const& assocTks = assocTracksWithMeta.at(iv);
    auto const& assocTksMeta = assocTracksWithMeta.data(iv);
    for (size_t itk = 0; itk < assocTks.size(); ++itk) {
      //
      // Get the recob::Track and VertexAssnMeta instance
      const art::Ptr<recob::Track>& trackAssn = assocTks[itk];
      const recob::VertexAssnMeta* trackMeta = assocTksMeta[itk];
      mf::LogVerbatim("ProxyExample")
        << "track with key=" << trackAssn.key() << " and length=" << trackAssn->Length()
        << " has propDist from vertex=" << trackMeta->propDist();
      //
      // Get the associated recob::Hit and the MCSFitResult
      const recob::MCSFitResult& assocMCS = mcsColl[trackAssn.key()];
      const std::vector<recob::Hit const*>& hits = assocHits.at(trackAssn.key());
      //
      // Print some information
      mf::LogVerbatim("ProxyExample")
        << "\tCountValidPoints=" << trackAssn->CountValidPoints() << " and nHits=" << hits.size()
        << " and MCSMom=" << assocMCS.bestMomentum();
      //
      // Now loop over the associated hits
      if (hits.size() < 50) {
        for (const recob::Hit* h : hits) {
          mf::LogVerbatim("ProxyExample")
            << "\t\thit wire=" << h->WireID() << " peak time=" << h->PeakTime();
        }
      }
    } // for track
  }   // for vertex

} // RecoProxyUsageExample::analyze()

DEFINE_ART_MODULE(RecoProxyUsageExample)
