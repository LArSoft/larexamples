cet_make_library(LIBRARY_NAME AtomicNumber INTERFACE
  SOURCE AtomicNumber.h
  LIBRARIES INTERFACE
  fhiclcpp::types
  )

install_fhicl()
install_headers()
