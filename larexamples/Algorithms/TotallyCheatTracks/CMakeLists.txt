add_subdirectory(CheatTrackData)

cet_make_library(SOURCE TotallyCheatTrackingAlg.cxx
  LIBRARIES
  PUBLIC
  larexamples::CheatTrack
  PRIVATE
  larcorealg::Geometry
  nusimdata::SimulationBase
)

cet_build_plugin(TotallyCheatTracker art::EDProducer
  LIBRARIES PRIVATE
  larexamples::Algorithms_TotallyCheatTracks
  larexamples::CheatTrack
  nusimdata::SimulationBase
  art::Framework_Principal
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
)
install_headers()
install_fhicl()
