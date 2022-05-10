cet_make_library(LIBRARY_NAME SpacePointTestUtils NO_INSTALL
  SOURCE SpacePointTestUtils.cxx
  LIBRARIES PUBLIC
  lardataobj::RecoBase
  larcorealg::Geometry
)

cet_build_plugin(CheckDataProductSize art::EDAnalyzer NO_INSTALL
  LIBRARIES PRIVATE
  lardataobj::RecoBase
  art::Framework_Principal
  canvas::canvas
  fhiclcpp::types
  cetlib_except::cetlib_except
)

cet_build_plugin(SpacePointMaker art::EDProducer NO_INSTALL
  LIBRARIES PRIVATE
  larexamples::SpacePointTestUtils
  larcore::Geometry_Geometry_service
  larcore::ServiceUtil
  lardataobj::RecoBase
  art::Framework_Principal
  messagefacility::MF_MessageLogger
  fhiclcpp::types
)

cet_test(PointIsolationAlg_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  larexamples::Algorithms_RemoveIsolatedSpacePoints
  cetlib::cetlib
)

cet_test(PointIsolationAlgRandom_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  larexamples::Algorithms_RemoveIsolatedSpacePoints
  larcorealg::StopWatch
  cetlib::cetlib
)

cet_test(
  PointIsolation_test
  HANDBUILT
  TEST_EXEC lar
  TEST_ARGS --rethrow-all -c point_isolation_test.fcl
  DATAFILES point_isolation_test.fcl
  )


# the following is not a real test in that it can't detect any error;
# the time taken by the stress test should be short even with debug qualifier!
cet_test(PointIsolationAlgStress_test
  LIBRARIES PRIVATE
  larexamples::Algorithms_RemoveIsolatedSpacePoints
  cetlib::cetlib
  TEST_ARGS 10000 0.05
  )

# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
