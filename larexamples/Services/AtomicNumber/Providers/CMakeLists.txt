cet_make_library(LIBRARY_NAME AtomicNumber INTERFACE
  SOURCE AtomicNumber.h
  LIBRARIES INTERFACE
  fhiclcpp::types
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
