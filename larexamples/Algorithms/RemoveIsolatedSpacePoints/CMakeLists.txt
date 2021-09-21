art_make(
  LIB_LIBRARIES
    larcorealg_Geometry
    cetlib_except::cetlib_except
    ROOT::Core
  MODULE_LIBRARIES
    larexamples_Algorithms_RemoveIsolatedSpacePoints
    art::Framework_Services_Registry
    messagefacility::MF_MessageLogger
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
