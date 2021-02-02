/**
 * @file   SpacePointTestUtils.cxx
 * @brief  Utilities for tests based on space points (implementation file)
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 3, 2016
 * @see    SpacePointTestUtils.h
 * @ingroup RemoveIsolatedSpacePoints
 *
 */

#include "SpacePointTestUtils.h"

// C/C++ standard libraries
#include <utility> // std::pair<>
#include <cmath> // std::sqrt()
#include <array>


// function prototype declarations (implementation below)
std::pair<int, int> ComputeRangeIndices
  (double min, double max, double stepSize);


//------------------------------------------------------------------------------
recob::SpacePoint lar::example::tests::MakeSpacePoint
  (int ID, double const* pos, double error /* = 0. */)
{
  // assume it's upper triangular; the documentation does not say
  double const err[6] = { error, 0., 0., error, 0., error };
  return {
    pos,
    err,
    0.0, // chisq
    ID
    };
} // lar::example::tests::MakeSpacePoint()


//------------------------------------------------------------------------------
unsigned int lar::example::tests::FillSpacePointGrid(
  std::vector<recob::SpacePoint>& spacePoints,
  geo::BoxBoundedGeo const& box,
  double stepSize
) {

  // determine the starting point
  auto const indicesX = ComputeRangeIndices(box.MinX(), box.MaxX(), stepSize);
  auto const indicesY = ComputeRangeIndices(box.MinY(), box.MaxY(), stepSize);
  auto const indicesZ = ComputeRangeIndices(box.MinZ(), box.MaxZ(), stepSize);

  int ID = spacePoints.empty()? 1: spacePoints.back().ID() + 1;
  size_t const origNPoints = spacePoints.size();
  double const error = stepSize / std::sqrt(12.);

  // fill the grid;
  // we don't use an increment (point[0] += stepping) to avoid rounding errors
  std::array<double, 3> const center
    {{ box.CenterX(), box.CenterY(), box.CenterZ() }};
  std::array<double, 3> point;
  for (int ix = indicesX.first; ix <= indicesX.second; ++ix) {
    point[0] = center[0] + ix * stepSize;

    for (int iy = indicesY.first; iy <= indicesY.second; ++iy) {
      point[1] = center[1] + iy * stepSize;

      for (int iz = indicesZ.first; iz <= indicesZ.second; ++iz) {
        point[2] = center[2] + iz * stepSize;

        spacePoints.push_back(MakeSpacePoint(ID++, point.data(), error));

      } // for z
    } // for y
  } // for x

  return spacePoints.size() - origNPoints;
} // lar::example::tests::FillSpacePointGrid()


//------------------------------------------------------------------------------
std::pair<int, int> ComputeRangeIndices
  (double min, double max, double stepSize)
{
  if (min >= max) return { 0, 0 };

  double const center = (min + max) / 2.;
  return { -int((center - min) / stepSize), int((max - center) / stepSize) };

} // ::ComputeRangeIndices()


//------------------------------------------------------------------------------
