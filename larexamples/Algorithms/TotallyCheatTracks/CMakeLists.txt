add_subdirectory(CheatTrackData)

art_make(
  LIB_LIBRARIES
    ROOT::Core
    ROOT::Physics
    lardataobj_RecoBase
  MODULE_LIBRARIES
    messagefacility::MFMessageLogger
    ROOT::Core
    larexamples_Algorithms_TotallyCheatTracks
    nusimdata_SimulationBase
  )

install_headers()
install_fhicl()

# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS ${TESTFHICLFILES})
unset(TESTFHICLFILES)

install_source()
