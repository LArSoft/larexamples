/**
 * @file   SpacePointTestUtils.h
 * @brief  Utilities for tests based on space points
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 3, 2016
 * @see    SpacePointTestUtils.cxx
 * @ingroup RemoveIsolatedSpacePoints
 *
 * This file offers:
 *
 * * MakeSpacePoint(): helper to create a new space point
 * * FillSpacePointGrid(): helper to create a grid of space points
 *
 */

#ifndef LAREXAMPLES_TEST_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTTESTUTILS_H
#define LAREXAMPLES_TEST_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTTESTUTILS_H

// LArSoft libraries
#include "larcorealg/Geometry/BoxBoundedGeo.h"
#include "lardataobj/RecoBase/SpacePoint.h"

// C/C++ standard libraries
#include <vector>

namespace lar {
  namespace example {
    namespace tests {

      // BEGIN RemoveIsolatedSpacePoints group ---------------------------------
      /// @ingroup RemoveIsolatedSpacePoints
      /// @{
      /**
       * @brief Creates and returns a new space point
       * @param ID space point identifier
       * @param pos space point position: { x, y, z } [cm]
       * @param error uncertainty on position (applies to all the coordinates)
       * @return a new space point
       *
       * Points are uncorrelated.
       */
      recob::SpacePoint MakeSpacePoint(int ID, double const* pos, double error = 0.);

      /**
       * @brief Creates space points distributed in a grid
       * @param spacePoints the container to be filled
       * @param box the extend of volume to be filled
       * @param stepSize the spacing between points
       * @return the number of space points added
       *
       * The function adds space points to the specified collection.
       * The centre of the box hosts a space point. The other space points are
       * added shifting by multiples of stepSize in all directions.
       * The IDs are incremental, starting from the ID next to the one from the
       * last space point in the collection, or 1 if the collection is empty.
       *
       * Points have an uncertainty of @f$ s/\sqrt{12} @f$, with @f$ s @f$
       * the step size.
       */
      unsigned int FillSpacePointGrid(std::vector<recob::SpacePoint>& spacePoints,
                                      geo::BoxBoundedGeo const& box,
                                      double stepSize);

      /// @}
      // END RemoveIsolatedSpacePoints group -----------------------------------

    } // namespace tests
  }   // namespace example
} // namespace lar

#endif // LAREXAMPLES_TEST_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTTESTUTILS_H
