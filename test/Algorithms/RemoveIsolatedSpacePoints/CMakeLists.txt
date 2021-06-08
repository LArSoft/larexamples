art_make(
  EXCLUDE
    PointIsolationAlg_test.cc
    PointIsolationAlgRandom_test.cc
    PointIsolationAlgStress_test.cc
  LIB_LIBRARIES
    lardataobj_RecoBase
    ROOT::Core
  MODULE_LIBRARIES
    lardataobj_RecoBase
    larcorealg_Geometry
    larcore_Geometry_Geometry_service
    larexamples_Algorithms_RemoveIsolatedSpacePoints
    test_Algorithms_RemoveIsolatedSpacePoints
    art_Framework_Core
    art_Framework_Services_Registry
    art_Framework_Principal
    art_Persistency_Provenance
    art_Utilities canvas
    ${MF_MESSAGELOGGER}
    
    ${FHICLCPP}
  )

cet_test(PointIsolationAlg_test USE_BOOST_UNIT)
cet_test(PointIsolationAlgRandom_test USE_BOOST_UNIT)

cet_test(
  PointIsolation_test
  HANDBUILT
  TEST_EXEC lar
  TEST_ARGS --rethrow-all -c point_isolation_test.fcl
  DATAFILES point_isolation_test.fcl
  )


# the following is not a real test in that it can't detect any error;
# the time taken by the stress test should be short even with debug qualifier!
cet_test(
  PointIsolationAlgStress_test
  TEST_ARGS 10000 0.05
  )

# install all sources, plus CMakeLists.txt and all configuration files
file(GLOB TESTFHICLFILES
     LIST_DIRECTORIES false
     *.fcl
     )
install_source(EXTRAS CMakeLists.txt ${TESTFHICLFILES})
unset(TESTFHICLFILES)
