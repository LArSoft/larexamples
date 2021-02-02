/**
 * @file   PointIsolationAlgStress_test.cc
 * @brief  Stress test for PointIsolationAlg
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 27, 2016
 * @see    PointIsolationAlg.h
 * @ingroup RemoveIsolatedSpacePoints
 *
 * Usage
 * ======
 *
 * Runs a isolation removal algorithm on a set of points distributed in a cubic
 * grid.
 *
 * Usage:
 *
 *     PointIsolationAlg_test NumberOfPoints[+|-] IsolationRadius
 *
 * where NumberOfPoints is an approximation of the number of points to be
 * generated on a grid and processed.
 * Due to the strict geometric pattern, only perfect cubes are allowed as
 * number of points. The perfect cube closest to NumberOfPoints be effectively
 * used, unless "+" or "-" are specified, in which cases the next non-smaller
 * or non-larger cube will be used, respectively.
 * The points are places in a simple grid, with a distance of 1 (arbitrary unit)
 * one from the next on each direction.
 * The IsolationRadius parameter is measured in the same unit.
 *
 * On configuration failure, the test returns with exit code 1.
 * On test failure, the test returns with exit code 2.
 *
 */

// LArSoft libraries
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/PointIsolationAlg.h"

// C/C++ standard libraries
#include <cmath> // std::pow(), std::ceil(), std::floor()
#include <stdexcept> // std::logic_error
#include <chrono>
#include <sstream>
#include <iostream>
#include <array>


// BEGIN RemoveIsolatedSpacePoints group ---------------------------------------
/// @ingroup RemoveIsolatedSpacePoints
/// @{
//------------------------------------------------------------------------------
//--- Test code
//---
template <typename T> T cube(T side) { return side * side * side; }

template <typename Point>
std::vector<Point> createPointsInCube(unsigned int pointsPerSide) {

  std::vector<Point> points;
  points.reserve(cube(pointsPerSide));

  Point p;
  for (unsigned int i = 0; i < pointsPerSide; ++i) {
    p[0] = i;
    for (unsigned int j = 0; j < pointsPerSide; ++j) {
      p[1] = j;
      for (unsigned int k = 0; k < pointsPerSide; ++k) {
        p[2] = k;
        points.push_back(p);
      } // for k
    } // for j
  } // for i

  return points;
} // createPointsInCube()


//------------------------------------------------------------------------------
template <typename T>
void PrintConfiguration(
  typename lar::example::PointIsolationAlg<T>::Configuration_t const& config,
  std::ostream& out = std::cout
) {
  out << "PointIsolationAlg algorithm configuration:"
    << "\n  radius: " << std::sqrt(config.radius2)
    << "\n  bounding box:"
    << "\n    x: " << config.rangeX.lower << " -- " << config.rangeX.upper
    << "\n    y: " << config.rangeY.lower << " -- " << config.rangeY.upper
    << "\n    z: " << config.rangeZ.lower << " -- " << config.rangeZ.upper
    << std::endl;
} // PrintConfiguration()


//------------------------------------------------------------------------------
template <typename T>
void StressTest(
  unsigned int pointsPerSide,
  typename lar::example::PointIsolationAlg<T>::Configuration_t const& config
) {

  using Coord_t = T;
  using PointIsolationAlg_t = lar::example::PointIsolationAlg<Coord_t>;

  using Point_t = std::array<Coord_t, 3U>;

  //
  // creation of the input points
  //
  auto start_init_time = std::chrono::high_resolution_clock::now();

  // create the points in a cube
  std::vector<Point_t> points = createPointsInCube<Point_t>(pointsPerSide);

  auto stop_init_time = std::chrono::high_resolution_clock::now();
  auto elapsed_init = std::chrono::duration_cast<std::chrono::duration<float>>
    (stop_init_time - start_init_time); // seconds

  unsigned int const expected = (config.radius2 >= 1.)? points.size(): 0;
  std::cout << "Processing " << points.size() << " points." << std::endl;

  //
  // algorithm initialisation and execution
  //
  PointIsolationAlg_t::validateConfiguration(config);
  PointIsolationAlg_t algo(config);
  auto start_run_time = std::chrono::high_resolution_clock::now();
  std::vector<size_t> result = algo.removeIsolatedPoints(points);
  auto stop_run_time = std::chrono::high_resolution_clock::now();

  auto elapsed_run = std::chrono::duration_cast<std::chrono::duration<float>>
    (stop_run_time - start_run_time); // seconds

  //
  // report results on screen
  //
  PrintConfiguration<Coord_t>(config);
  std::cout << "Found " << result.size() << "/" << points.size()
    << " non-isolated points in " << (elapsed_run.count()*1000.) << " ms"
    << " (" << (elapsed_init.count()*1000.) << " ms for initialization)"
    << std::endl;

  if (result.size() != expected) {
    throw std::logic_error(
      "Expected " + std::to_string(expected) + " non-isolated points, found "
      + std::to_string(points.size()) + "."
      );
  }

} // StressTest()


//------------------------------------------------------------------------------
//--- main()
//---
int main(int argc, char** argv) {
  using Coord_t = double;

  //
  // argument parsing
  //
  if (argc != 3) {
    std::cerr << "Usage:  " << argv[0]
      << "  NumberOfPoints[+|-] IsolationRadius"
      << std::endl;
    return 1;
  }

  std::istringstream sstr;

  enum RoundMode_t { rmNearest, rmCeil, rmFloor, rmDefault = rmNearest };

  unsigned int requestedPoints = 0;
  RoundMode_t roundMode = rmDefault;
  sstr.str(argv[1]);
  sstr >> requestedPoints;
  if (!sstr) {
    std::cerr << "Error: expected number of points as first argument, got '"
      << argv[1] << "' instead." << std::endl;
    return 1;
  }
  char c;
  sstr >> c;
  if (sstr.eof()) roundMode = rmDefault;
  else {
    switch (c) {
      case '+': roundMode = rmCeil; break;
      case '-': roundMode = rmFloor; break;
      default:
        std::cerr << "Invalid round mode specification '" << c
          << "' (must be '+', '-' or omitted)" << std::endl;
        return 1;
    } // switch round mode spec
  } // if has round mode spec

  Coord_t radius;
  sstr.clear();
  sstr.str(argv[2]);
  sstr >> radius;
  if (!sstr) {
    std::cerr << "Error: expected isolation radius as second argument, got '"
      << argv[2] << "' instead." << std::endl;
    return 1;
  }


  //
  // prepare the configuration
  //

  // decide on the points per side
  double sideLength = std::pow(double(requestedPoints), 1./3.);
  unsigned int pointsPerSide = (unsigned int) std::floor(sideLength);
  switch (roundMode) {
    case rmFloor: break;
    case rmCeil: pointsPerSide = (unsigned int) std::ceil(sideLength); break;
    case rmNearest: {
      unsigned int const nFloorPoints =  cube(pointsPerSide),
        nCeilPoints = cube(pointsPerSide + 1);
      if ((requestedPoints - nFloorPoints) >= (nCeilPoints - requestedPoints))
        ++pointsPerSide;
      break;
    } // case rmNearest
  } // switch roundMode
  if (pointsPerSide < 1) pointsPerSide = 1; // sanity check

  // enclosing volume has a 0.5 margin
  constexpr Coord_t margin = 0.5;
  lar::example::PointIsolationAlg<Coord_t>::Configuration_t config;
  config.radius2 = cet::square(radius);
  config.rangeX = { -margin, pointsPerSide - 1.0 + margin };
  config.rangeY = config.rangeX;
  config.rangeZ = config.rangeX;

  try {
    StressTest<Coord_t>(pointsPerSide, config);
  }
  catch (std::logic_error const& e) {
    std::cerr << "Test failure!\n" << e.what() << std::endl;
    return 2;
  }

  return 0;
} // main()

/// @}
// END RemoveIsolatedSpacePoints group -----------------------------------------

