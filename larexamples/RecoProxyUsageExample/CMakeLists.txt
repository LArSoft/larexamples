cet_build_plugin(RecoProxyUsageExample art::EDAnalyzer
  LIBRARIES PRIVATE
  lardata::RecoBaseProxy
  lardataobj::RecoBase
  art::Framework_Principal
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
)

install_headers()
install_fhicl()
install_scripts()
install_source()
