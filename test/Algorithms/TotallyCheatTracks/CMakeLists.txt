cet_build_plugin(ParticleMaker art::EDProducer NO_INSTALL
  LIBRARIES PRIVATE
  larcorealg::geo_vectors_utils
  larcoreobj::geo_vectors
  nusimdata::SimulationBase
  messagefacility::MF_MessageLogger
  fhiclcpp::types
  ROOT::Physics
)

cet_build_plugin(DumpCheatTracks art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  larexamples::CheatTrack
  nusimdata::SimulationBase
  art::Framework_Principal
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
  cetlib_except::cetlib_except
  Boost::unit_test_framework
)

cet_test(
  TotallyCheatTracker_test
  HANDBUILT
  TEST_EXEC lar_ut
  TEST_ARGS -- --rethrow-all --config test_totallycheattracker.fcl
  DATAFILES test_totallycheattracker.fcl
  )


# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
