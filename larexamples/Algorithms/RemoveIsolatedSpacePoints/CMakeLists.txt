cet_make_library(SOURCE
  PointIsolationAlg.h
  SpacePartition.h
  SpacePointIsolationAlg.cxx
  LIBRARIES
  PUBLIC
  lardata::headers
  lardataobj::RecoBase
  fhiclcpp::types
  fhiclcpp::fhiclcpp
  cetlib::cetlib
  PRIVATE
  larcorealg::Geometry
  cetlib_except::cetlib_except
)

cet_build_plugin(RemoveIsolatedSpacePoints art::EDProducer
  LIBRARIES PRIVATE
  larexamples::Algorithms_RemoveIsolatedSpacePoints
  larcore::Geometry_Geometry_service
  larcore::ServiceUtil
  lardataobj::RecoBase
)

install_headers()
install_fhicl()
