cet_build_plugin(AtomicNumberService art::service
  IMPL_SOURCE
  LIBRARIES INTERFACE
  larexamples::AtomicNumber
  larcore::ServiceProviderWrappers
)

install_fhicl()
install_headers()

# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
