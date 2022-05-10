cet_build_plugin(Disturbance art::EDProducer
  LIBRARIES PRIVATE
  canvas::canvas
  fhiclcpp::types
)

cet_build_plugin(Exploder art::EDAnalyzer
  LIBRARIES PRIVATE
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
)

install_source()
install_fhicl()
