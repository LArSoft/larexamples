simple_plugin(ParticleMaker "module"
    nusimdata_SimulationBase
    art_Framework_Core
    art_Framework_Principal
    art_Persistency_Provenance
    canvas
    messagefacility::MF_MessageLogger
    fhiclcpp::fhiclcpp fhiclcpp::types
    ROOT::Core
    ROOT::Physics
  NO_INSTALL
  )

simple_plugin(DumpCheatTracks "module"
    lardataobj_RecoBase
    nusimdata_SimulationBase
    art_Framework_Core
    art_Framework_Principal
    art_Persistency_Provenance
    canvas
    messagefacility::MF_MessageLogger
    fhiclcpp::fhiclcpp fhiclcpp::types
    ROOT::Core
    ROOT::EG
  USE_BOOST_UNIT
  NO_INSTALL
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
