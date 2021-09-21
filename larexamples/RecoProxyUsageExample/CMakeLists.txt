simple_plugin(RecoProxyUsageExample "module"
              messagefacility::MF_MessageLogger
              ROOT::Core
              ${ROOT_GENVECTOR}
              lardataobj_RecoBase
)

install_headers()
install_fhicl()
install_scripts()
install_source()
