cet_build_plugin(AtomicNumberService art::service
  IMPL_SOURCE
  LIBRARIES INTERFACE
  larexamples::AtomicNumber
  larcore::ServiceProviderWrappers
)

install_fhicl()
install_headers()
