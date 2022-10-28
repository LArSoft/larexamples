/**
 * @file   SpacePointIsolationAlg.cxx
 * @brief  Algorithm(s) dealing with space point isolation in space
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 26, 2016
 * @see    SpacePointIsolationAlg.h
 * @ingroup RemoveIsolatedSpacePoints
 *
 */

// LArSoft libraries
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/SpacePointIsolationAlg.h"
#include "larcorealg/Geometry/GeometryCore.h"

// infrastructure and utilities
#include "cetlib_except/exception.h"

// C/C++ standard libraries
#include <memory>    // std::make_unique()
#include <stdexcept> // std::runtime_error()

//------------------------------------------------------------------------------
//--- lar::example::SpacePointIsolationAlg
//---

void lar::example::SpacePointIsolationAlg::initialize()
{

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

  if (isolationAlg)
    isolationAlg->reconfigure(config);
  else
    isolationAlg = std::make_unique<PointIsolationAlg_t>(config);

} // lar::example::SpacePointIsolationAlg::initialize()

void lar::example::SpacePointIsolationAlg::fillAlgConfigFromGeometry(
  PointIsolationAlg_t::Configuration_t& config)
{
  // merge the volumes from all TPCs
  geo::BoxBoundedGeo box{};

  // a TPC is (also) a bounded box:
  for (geo::BoxBoundedGeo const& tpc : geom->Iterate<geo::TPCGeo>()) {
    box.ExtendToInclude(tpc);
  }

  // convert the box into the configuration structure
  config.rangeX = {box.MinX(), box.MaxX()};
  config.rangeY = {box.MinY(), box.MaxY()};
  config.rangeZ = {box.MinZ(), box.MaxZ()};

} // lar::example::SpacePointIsolationAlg::fillAlgConfigFromGeometry()
