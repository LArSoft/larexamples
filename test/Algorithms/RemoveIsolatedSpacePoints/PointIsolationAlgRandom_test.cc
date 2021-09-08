/**
 * @file   PointIsolationAlgRandom_test.cc
 * @brief  Unit test with random data for PointIsolationAlg
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 30, 2016
 * @see    PointIsolationAlg.h
 * @ingroup RemoveIsolatedSpacePoints
 *
 * This test populate datasets with random data and tests the isolation
 * algorithm with them.
 *
 * The test accepts one optional argument:
 *
 *     PointIsolationAlg_test  [seed]
 *
 * to set the random seed to a particular value.
 *
 */

// LArSoft libraries
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/PointIsolationAlg.h"
#include "larcorealg/TestUtils/StopWatch.h"

// infrastructure and utilities
#include "cetlib/pow.h" // cet::sum_squares()

// Boost libraries
#define BOOST_TEST_MODULE ( PointIsolationAlg_test )
#include <cetlib/quiet_unit_test.hpp> // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK(), BOOST_CHECK_EQUAL()

// C/C++ standard libraries
#include <array>
#include <random>
#include <chrono>
#include <ratio> // std::milli
#include <iostream>
#include <algorithm> // std::sort()


// BEGIN RemoveIsolatedSpacePoints group ---------------------------------------
/// @ingroup RemoveIsolatedSpacePoints
/// @{
//------------------------------------------------------------------------------
//--- Test code
//---
/**
 * @fn PointIsolationTest
 * @brief Tests various isolation radii on a random-distributed set of points
 * @param generator engine used to create the random input sample
 * @param nPoints points in the input sample
 * @param radii list of isolation radii to test
 *
 *
 */
template <typename Engine, typename Coord = float>
void PointIsolationTest(
  Engine& generator, unsigned int nPoints, std::vector<Coord> const& radii
) {

  using Coord_t = Coord;

  //
  // create the input sample
  //
  std::uniform_real_distribution<Coord_t> uniform(-1., +1.);
  auto randomCoord = std::bind(uniform, generator);

  using Point_t = std::array<Coord_t, 3U>;
  std::vector<Point_t> points;

  points.reserve(nPoints);
  for (unsigned int i = 0; i < nPoints; ++i)
    points.push_back({{ randomCoord(), randomCoord(), randomCoord() }});
  std::cout
    << "\n" << std::string(75, '=')
    << "\nTest with " << nPoints << " points"
    << "\n" << std::string(72, '-')
    << std::endl;

  //
  // create the algorithm
  //
  using PointIsolationAlg_t = lar::example::PointIsolationAlg<Coord_t>;

  typename PointIsolationAlg_t::Configuration_t config;
  config.rangeX = { -2., +2. };
  config.rangeY = { -2., +2. };
  config.rangeZ = { -2., +2. };
  config.radius2 = 1.;
//  config.maxMemory = 100 * 1048576; // we keep the default memory setting
  PointIsolationAlg_t algo(config);

  //
  // for each isolation radius:
  //

  // measurement in milliseconds, double precision:
  testing::StopWatch<std::chrono::duration<double, std::milli>> timer;
  for (Coord_t radius: radii) {

    //
    // set up the algorithm
    //
    config.radius2 = cet::square(radius);
    algo.reconfigure(config);

    std::cout << "Isolation radius: " << radius << std::endl;

    //
    // run the algorithm with the brute force approach
    //
    timer.restart();
    auto expected
      = algo.bruteRemoveIsolatedPoints(points.begin(), points.end());
    auto elapsed = timer.elapsed();
    std::sort(expected.begin(), expected.end());
    std::cout << "  brute force: " << elapsed << " ms"
      << std::endl;

    //
    // run the algorithm with the default approach
    //
    timer.restart();
    auto actual = algo.removeIsolatedPoints(points);
    elapsed = timer.elapsed();
    std::sort(actual.begin(), actual.end());
    std::cout << "  regular:     " << elapsed << " ms"
      << std::endl;

    //
    // sort and compare the results
    //
    BOOST_TEST(actual == expected, boost::test_tools::per_element());

  } // for isolation radius

  std::cout << std::string(72, '-') << std::endl;

} // PointIsolationTest()


//------------------------------------------------------------------------------
//--- tests
//
struct ArgsFixture {
  ArgsFixture()
    : argc(boost::unit_test::framework::master_test_suite().argc)
    , argv(boost::unit_test::framework::master_test_suite().argv)
    {}
  int argc;
  char **argv;
}; // ArgsFixture


BOOST_FIXTURE_TEST_CASE(PointIsolationTestCase, ArgsFixture) {

  // we explicitly set the seed, even if with a default value
  auto seed = std::default_random_engine::default_seed;

  if (argc > 1) {
    std::istringstream sstr;
    sstr.str(argv[1]);
    sstr >> seed;
    if (!sstr) {
      throw std::runtime_error
        ("Invalid seed specified: " + std::string(argv[1]));
    }
  } // if seed specified

  // this engine can be arbitrarily crappy; don't use it for real physics!
  std::default_random_engine generator(seed);
  std::cout << "Random seed: " << seed << std::endl;

  // try all these isolation radii
  std::vector<float>        const Radii { 0.05, 0.1, 0.5, 2.0 };
  std::vector<unsigned int> const DataSizes { 100, 10000 };

  for (unsigned int nPoints: DataSizes)
    PointIsolationTest(generator, nPoints, Radii);

} // PointIsolationTestCase()


/// @}
// END RemoveIsolatedSpacePoints group -----------------------------------------
