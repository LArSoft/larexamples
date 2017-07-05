/**
 * @file   SpacePointIsolationAlg.cxx
 * @brief  Algorithm(s) dealing with space point isolation in space
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 26, 2016
 * @see    SpacePointIsolationAlg.h
 * 
 */

// LArSoft libraries
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/SpacePointIsolationAlg.h"
#include "larcorealg/Geometry/GeometryCore.h"

// infrastructure and utilities
#include "cetlib/exception.h"

// C/C++ standard libraries
#include <stdexcept> // std::runtime_error()
#include <memory> // std::make_unique()


//------------------------------------------------------------------------------
//--- lar::example::SpacePointIsolationAlg
//--- 


void lar::example::SpacePointIsolationAlg::initialize() {
  
  PointIsolationAlg_t::Configuration_t config;
  
  config.radius2 = radius2; // square of isolation radius [cm^2]
  fillAlgConfigFromGeometry(config);
  
  // proceed to validate the configuration we are going to use
  try {
    PointIsolationAlg_t::validateConfiguration(config);
  }
  catch (std::runtime_error const& e) {
    throw cet::exception("SpacePointIsolationAlg")
      << "Error in PointIsolationAlg configuration: " << e.what() << "\n";
  }
  
  if (isolationAlg) isolationAlg->reconfigure(config);
  else isolationAlg = std::make_unique<PointIsolationAlg_t>(config);
  
} // lar::example::SpacePointIsolationAlg::initialize()


void lar::example::SpacePointIsolationAlg::fillAlgConfigFromGeometry
  (PointIsolationAlg_t::Configuration_t& config)
{
  // merge the volumes from all TPCs
  auto iTPC = geom->begin_TPC(), tpcend = geom->end_TPC();
  
  // a TPC is (also) a bounded box:
  geo::BoxBoundedGeo box = (geo::BoxBoundedGeo) *iTPC;
  
  while (++iTPC != tpcend) box.ExtendToInclude(*iTPC);
  
  // convert the box into the configuration structure
  config.rangeX = { box.MinX(), box.MaxX() };
  config.rangeY = { box.MinY(), box.MaxY() };
  config.rangeZ = { box.MinZ(), box.MaxZ() };
  
} // lar::example::SpacePointIsolationAlg::fillAlgConfigFromGeometry()

