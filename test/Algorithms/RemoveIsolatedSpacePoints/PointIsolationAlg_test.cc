/**
 * @file   PointIsolationAlg_test.cc
 * @brief  Unit tests for PointIsolationAlg
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 27, 2016
 * @see    PointIsolationAlg.h
 * @ingroup RemoveIsolatedSpacePoints
 *
 * This test sets up point distributions with known isolation features,
 * runs the algorithm with various isolation radius settings and verifies that
 * the results are as expected.
 *
 * The test is run with no arguments.
 *
 * Two tests are run:
 *
 * * `PointIsolationTest1`: low multiplicity unit tests
 * * `PointIsolationTest2`: larger scale test
 *
 * See the documentation of the two functions for more information.
 *
 */

// LArSoft libraries
#include "cetlib/pow.h"
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/PointIsolationAlg.h"

// Boost libraries
#define BOOST_TEST_MODULE (PointIsolationAlg_test)
#include "boost/test/unit_test.hpp"

// C/C++ standard libraries
#include <algorithm> // std::sort()
#include <array>
#include <numeric> // std::iota()

// BEGIN RemoveIsolatedSpacePoints group ---------------------------------------
/// @ingroup RemoveIsolatedSpacePoints
/// @{
//------------------------------------------------------------------------------
//--- Test code
//---
/**
 * @brief Low-multiplicity unit test
 *
 * This tests exercises simple point dispositions: a single point, two points,
 * three points at different distance, and two "pairs" of points.
 * The isolation radius of the test is fixed.
 *
 * This test uses coordinate type `float`.
 *
 */
void PointIsolationTest1()
{

  using Coord_t = float;
  using PointIsolationAlg_t = lar::example::PointIsolationAlg<Coord_t>;

  using Point_t = std::array<Coord_t, 3U>;

  PointIsolationAlg_t::Configuration_t config;
  config.radius2 = cet::square(1.);
  config.rangeX = {-2., +2.};
  config.rangeY = {-2., +2.};
  config.rangeZ = {-2., +2.};

  std::vector<Point_t> points;
  std::vector<size_t> result, expected;

  PointIsolationAlg_t algo(config);

  // first test: a single point
  points.push_back({{+1., +1., +1.}});
  expected.clear();

  result = algo.removeIsolatedPoints(points);
  std::sort(result.begin(), result.end());
  BOOST_TEST(result == expected, boost::test_tools::per_element());

  // second test: two far points
  points.push_back({{-1., -1., -1.}});
  expected.clear();

  result = algo.removeIsolatedPoints(points);
  std::sort(result.begin(), result.end());
  BOOST_TEST(result == expected, boost::test_tools::per_element());

  // third test: two close points, another isolated
  points.push_back({{+0.5, +1.0, +1.0}});
  expected.insert(expected.end(), {0U, 2U});
  std::sort(expected.begin(), expected.end());

  result = algo.removeIsolatedPoints(points);
  std::sort(result.begin(), result.end());
  BOOST_TEST(result == expected, boost::test_tools::per_element());

  // fourth test: two close points, another two also close
  points.push_back({{-0.5, -1.0, -1.0}});
  expected.insert(expected.end(), {1U, 3U});
  std::sort(expected.begin(), expected.end());

  result = algo.removeIsolatedPoints(points);
  std::sort(result.begin(), result.end());
  BOOST_TEST(result == expected, boost::test_tools::per_element());

} // PointIsolationTest1()

//------------------------------------------------------------------------------
/**
 * @brief Creates a "star" disposition of points
 * @tparam T type of coordinate being used
 * @param nShells number of points on each ray of the star (origin excluded)
 * @param distance distance from the origin of the farthest point
 * @return a collection of points (each a `std::array<T, 3>`
 *
 * Points are aligned on a semi-axis, sparser and sparser as they go away from
 * origin. nShell is the number of points beside the origin on each semi-axis.
 * Origin is always included.
 * A sequence is generated for each of the semi-axes (x, y, and z, two
 * directions each).
 * Example for `nShells = 5`, showing only one axis (that is, two semi-axes):
 * ~~~~
 * o               o       o   o o O o o   o       o               o
 * ~~~~
 * (points are marked with `o`, with `O` being the origin)
 *
 * The order of the points in the set is:
 *
 */
template <typename T>
auto CreateStarOfPoints(unsigned int nShells, T distance = T(1)) -> decltype(auto)
{

  using Coord_t = T;
  using Point_t = std::array<Coord_t, 3U>;

  std::vector<Point_t> points;
  points.reserve(1 + 1 * nShells);

  // fill shell by shell
  while (nShells-- > 0) {
    points.push_back({{distance, 0., 0.}});
    points.push_back({{-distance, 0., 0.}});
    points.push_back({{0., distance, 0.}});
    points.push_back({{0., -distance, 0.}});
    points.push_back({{0., 0., distance}});
    points.push_back({{0., 0., -distance}});
    distance /= 2;
  } // while

  // add the origin
  points.push_back({{0., 0., 0.}});

  return points;
} // CreateStarOfPoints()

/**
 * @brief Tests various isolation radii on a star-distributed set of points
 * @param levels number of shells in the star (0 is only its centre)
 *
 * The test uses a star-distributed set of points, as produced by
 * `CreateStarOfPoints()`.
 * This distribution has the characteristic that all the points farther than
 * the isolation radius _from the origin_ are indeed isolated. This makes the
 * prediction of the number of isolated points easier.
 *
 *
 * This test uses coordinate type `double`.
 *
 */
void PointIsolationTest2(unsigned int levels)
{

  using Coord_t = double;
  using PointIsolationAlg_t = lar::example::PointIsolationAlg<Coord_t>;

  using Point_t = std::array<Coord_t, 3U>;

  //
  // prepare the input
  //
  constexpr Coord_t starRadius = 1.;
  std::vector<Point_t> points = CreateStarOfPoints<Coord_t>(levels, starRadius);

  //
  // prepare the algorithm
  //
  PointIsolationAlg_t::Configuration_t config;
  config.radius2 = cet::square(1.);
  config.rangeX = {-2., +2.};
  config.rangeY = {-2., +2.};
  config.rangeZ = {-2., +2.};
  PointIsolationAlg_t algo(config);

  //
  // check every level
  //
  constexpr unsigned int nSemiDirections = 6;
  // small step (smaller than smallest distance between shells):
  const Coord_t epsilonStep = starRadius / (2 << levels);
  double baseRadius = starRadius; // starting radius

  // with the wider isolation radius, we expect all the points to be
  // non-isolated; the most isolated points are at the beginning of the list
  size_t const maxExpectedPoints = 1 + levels * nSemiDirections;
  assert(maxExpectedPoints == points.size());

  std::vector<size_t> expectedBase(maxExpectedPoints);
  std::iota(expectedBase.begin(), expectedBase.end(), 0U);

  // check radii that fall in between all the shells;
  // the first radius is a bit more than half the star radius, and it is
  // expected to include all points; the next, a bit more than a fourth of the
  // star radius, will define the 6 points on the outer shell as isolated;
  // and so forth.
  // Level N has a radius that includes N shells plus the origin.
  // Level 0 has a radius so small that it includes only the origin.
  unsigned int level = levels;
  do {

    // compute and set up a proper isolation radius for this level
    baseRadius /= 2.;
    config.radius2 = cet::square(baseRadius + epsilonStep);
    algo.reconfigure(config);

    BOOST_TEST_MESSAGE("[" << level << "] testing with radius " << (baseRadius + epsilonStep));

    // we expect to progressively have less and less non-isolated points...
    unsigned int const nExpected = (level > 1) ? (1 + level * nSemiDirections) : 0;
    // ... and we expect those points to be the first ones in the collection
    std::vector<size_t> expected(expectedBase.end() - nExpected, expectedBase.end());

    std::vector<size_t> result = algo.removeIsolatedPoints(points);
    BOOST_TEST(result.size() == expected.size());

    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    BOOST_TEST(result == expected, boost::test_tools::per_element());

  } while (--level > 0);

} // PointIsolationTest2()

//------------------------------------------------------------------------------
//--- tests
//
BOOST_AUTO_TEST_CASE(PointIsolationAlgTest)
{
  PointIsolationTest1();
} // PointIsolationAlgTest()

BOOST_AUTO_TEST_CASE(PointIsolationAlgVerificationTest)
{
  PointIsolationTest2(5);
} // PointIsolationAlgVerificationTest()

/// @}
// END RemoveIsolatedSpacePoints group -----------------------------------------
