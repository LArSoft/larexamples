/**
 * @file   PointIsolationAlg.h
 * @brief  Algorithm(s) dealing with point isolation in space
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 27, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 * This library contains only template classes and it is header only.
 *
 */

#ifndef LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_POINTISOLATIONALG_H
#define LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_POINTISOLATIONALG_H

// LArSoft libraries
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/SpacePartition.h"

// infrastructure and utilities
#include "cetlib/pow.h" // cet::sum_squares()

// C/C++ standard libraries
#include <array>
#include <cassert>   // assert()
#include <cmath>     // std::sqrt()
#include <iterator>  // std::cbegin(), std::cend(), std::distance()
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

namespace lar {
  namespace example {

    // BEGIN RemoveIsolatedSpacePoints group -----------------------------------
    /// @ingroup RemoveIsolatedSpacePoints
    /// @{
    /**
     * @brief Algorithm to detect isolated space points
     * @tparam Coord type of the coordinate
     * @see @ref RemoveIsolatedSpacePoints "RemoveIsolatedSpacePoints example overview"
     *
     * This algorithm returns a selection of the input points which are not
     * isolated. Point @f$ i @f$ is defined as isolated if:
     * @f$ \min \left\{ \left| \vec{r}_{i} - \vec{r_{j}} \right| \right\}_{i \neq j} < R @f$
     * where @f$ \vec{r_{k}} @f$ describes the position of the point @f$ k @f$
     * in space and @f$ R @f$ is the isolation radius.
     *
     * This class must be configured by providing a complete Configuration_t
     * object. Configuration can be changed at any time after that.
     *
     * The configuration information (`Configuration_t`) defines the volume the
     * points span and the square of the isolation radius.
     * The information on the volume may be used to optimise the algorithm,
     * and it is not checked. If that information is wrong (that means input
     * points lie outside that volume), the result is undefined.
     * No check is automatically performed to assess if the configuration is
     * valid.
     *
     * The algorithm can be run on any collection of points, as long as the
     * point class supports the `PositionExtractor` class.
     * A typical cycle of use is:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *     // creation and configuration
     *     lar::examples::PointIsolationAlg<float>::Configuration_t config;
     *     config.rangeX = { -1., 1. };
     *     config.rangeY = { -1., 1. };
     *     config.rangeZ = { -5., 5. };
     *     config.radius2 = 0.25;
     *
     *     lar::examples::PointIsolationAlg<float> algo(config);
     *
     *     // preparation/retrieval of input
     *     std::vector<std::array<float, 3>> points;
     *     // points are filled here
     *
     *     // execution
     *     std::vector<size_t> indices
     *       = algo.removeIsolatedPoints(points.begin(), points.end());
     *
     *     // utilization of the result;
     *     // - e.g., create a collection of non-isolated points...
     *     std::vector<std::array<float, 3>> nonIsolatedPoints;
     *     nonIsolatedPoints.reserve(indices.size());
     *     for (size_t index: indices)
     *       nonIsolatedPoints.push_back(points[index]);
     *
     *     // - ... or their pointers
     *     std::vector<std::array<float, 3> const*> nonIsolatedPointPtrs;
     *     nonIsolatedPointPtrs.reserve(indices.size());
     *     for (size_t index: indices)
     *       nonIsolatedPointPtrs.push_back(&(points[index]));
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * The point type here is `std::array<float, 3>`, for which a
     * `lar::examples::PositionExtractor<std::array<float, 3>>` is defined
     * in this same library.
     * The algorithm can be executed multiple times, and the configuration can
     * be changed at any time (`reconfigure()`).
     *
     * Validation of the configuration is optional, and needs to be explicitly
     * called if desired (`validateConfiguration()`).
     *
     *
     * Description of the algorithm
     * -----------------------------
     *
     * The basic method to determine the isolation of a point is by brute force,
     * by computing the distance with all others and, as soon as one of them is
     * found too close, declare the point non-isolated.
     *
     * A refinement is implemented: the points are grouped in cubic "cells"
     * and points in cells that are farther than isolation radius are not
     * checked against each other. This requires some memory to allocate the
     * structure, that can become huge. The maximum memory parameter keeps this
     * sane.
     *
     * Other refinements are not implemented. When a point is found non-isolated
     * also the point that makes it non-isolated should also be marked so. Cell
     * radius might be tuned to be smaller. Some of the neighbour cells may be
     * too far and should not be checked. The grid allocates a vector for each
     * cell, whether it's empty or not; using a sparse structure might reduce
     * the memory; also if the grid contains pointers to vectors instead of
     * vectors, and the grid is very sparse, there should still be some memory
     * saving.
     *
     */
    template <typename Coord = double>
    class PointIsolationAlg {

    public:
      /// Type of coordinate
      using Coord_t = Coord;
      using Range_t = CoordRange<Coord_t>;

      /// Type containing all configuration parameters of the algorithm
      struct Configuration_t {
        Range_t rangeX;  ///< range in X of the covered volume
        Range_t rangeY;  ///< range in Y of the covered volume
        Range_t rangeZ;  ///< range in Z of the covered volume
        Coord_t radius2; ///< square of isolation radius [cm^2]
        size_t maxMemory = 100 * 1048576;
        ///< grid smaller than this number of bytes (100 MiB)
      }; // Configuration_t

      /// @{
      /// @name Configuration

      /**
       * @brief Constructor with configuration validation
       * @param first_config configuration parameter structure
       *
       * For the configuration, see `SpacePointIsolationAlg` documentation.
       * No validation is performed on the configuration.
       */
      PointIsolationAlg(Configuration_t const& first_config) : config(first_config) {}

      /// Reconfigures the algorithm with the specified configuration
      /// (no validation is performed)
      /// @see configuration()
      void reconfigure(Configuration_t const& new_config) { config = new_config; }

      /// Returns a constant reference to the current configuration
      /// @see reconfigure()
      Configuration_t& configuration() const { return config; }

      /// @}

      /**
       * @brief Returns the set of points that are not isolated
       * @tparam PointIter random access iterator to a point type
       * @param begin iterator to the first point to be considered
       * @param end iterator after the last point to be considered
       * @return a list of indices of non-isolated points in the input range
       *
       * This method is the operating core of the algorithm.
       *
       * The input is two iterators. The output is a collection of the
       * indices of the elements that are not isolated. The index is equivalent
       * to `std::distance(begin, point)`.
       * The order of the elements in the collection is not specified.
       *
       * This method can use any collection of input data, as long as a
       * `PositionExtractor` object is available for it.
       */
      template <typename PointIter>
      std::vector<size_t> removeIsolatedPoints(PointIter begin, PointIter end) const;

      /**
       * @brief Returns the set of points that are not isolated
       * @param points list of the reconstructed space points
       * @return a list of indices of non-isolated points in the vector
       * @see removeIsolatedPoints(PointIter begin, PointIter end) const
       */
      template <typename Cont>
      std::vector<size_t> removeIsolatedPoints(Cont const& points) const
      {
        return removeIsolatedPoints(std::cbegin(points), std::cend(points));
      }

      /**
       * @brief Brute-force reference algorithm
       * @tparam PointIter random access iterator to a point type
       * @param begin iterator to the first point to be considered
       * @param end iterator after the last point to be considered
       * @return a list of indices of non-isolated points in the input range
       * @see removeIsolatedPoints
       *
       * This algorithm executes the task in a @f$ N^{2} @f$ way, slow and
       * supposedly reliable.
       * The interface is the same as `removeIsolatedPoints`.
       * Use this only for tests.
       */
      template <typename PointIter>
      std::vector<size_t> bruteRemoveIsolatedPoints(PointIter begin, PointIter end) const;

      /// @{
      /// @name Configuration

      /// Throws an exception if the configuration is invalid
      /// @throw std::runtime_error if configuration is invalid
      static void validateConfiguration(Configuration_t const& config);

      /// @}

      /// Returns the maximum optimal cell size when using a isolation radius
      static Coord_t maximumOptimalCellSize(Coord_t radius) { return radius / std::sqrt(3.); }

    private:
      /// type managing cell indices
      using Indexer_t = ::util::GridContainer3DIndices; // same in GridContainer

      /// type of neighbourhood cell offsets
      using NeighAddresses_t = std::vector<Indexer_t::CellIndexOffset_t>;

      template <typename PointIter>
      using Partition_t = SpacePartition<PointIter>;

      template <typename PointIter>
      using Point_t = decltype(*PointIter());

      Configuration_t config; ///< all configuration data

      /// Computes the cell size to be used
      template <typename PointIter = std::array<double, 3> const*>
      Coord_t computeCellSize() const;

      /// Returns a list of cell offsets for the neighbourhood of given radius
      NeighAddresses_t buildNeighborhood(Indexer_t const& indexer, unsigned int neighExtent) const;

      /// Returns whether a point is isolated with respect to all the others
      template <typename PointIter>
      bool isPointIsolatedFrom(Point_t<PointIter> const& point,
                               typename Partition_t<PointIter>::Cell_t const& otherPoints) const;

      /// Returns whether a point is isolated in the specified neighbourhood
      template <typename PointIter>
      bool isPointIsolatedWithinNeighborhood(Partition_t<PointIter> const& partition,
                                             Indexer_t::CellIndex_t cellIndex,
                                             Point_t<PointIter> const& point,
                                             NeighAddresses_t const& neighList) const;

      /// Returns whether A and B are close enough to be considered non-isolated
      template <typename Point>
      bool closeEnough(Point const& A, Point const& B) const;

      /// Helper function. Returns a string `"(<from> to <to>)"`
      static std::string rangeString(Coord_t from, Coord_t to);

      /// Helper function. Returns a string `"(<from> to <to>)"`
      static std::string rangeString(Range_t range)
      {
        return rangeString(range.lower, range.upper);
      }

    }; // class PointIsolationAlg

    //--------------------------------------------------------------------------
    /// @}
    // END RemoveIsolatedSpacePoints group -------------------------------------

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- template implementation
//------------------------------------------------------------------------------
template <typename Coord>
template <typename PointIter>
std::vector<size_t> lar::example::PointIsolationAlg<Coord>::removeIsolatedPoints(
  PointIter begin,
  PointIter end) const
{

  std::vector<size_t> nonIsolated;

  Coord_t const R = std::sqrt(config.radius2);

  //
  // determine space partition settings: cell size
  //
  // maximum: the volume of a single cell must be contained in a sphere with
  // radius equal to the isolation radius R
  //
  // minimum: needs tuning
  //

  Coord_t cellSize = computeCellSize<PointIter>();
  assert(cellSize > 0);
  Partition_t<PointIter> partition(
    {config.rangeX, cellSize}, {config.rangeY, cellSize}, {config.rangeZ, cellSize});

  // if a cell is contained in a sphere with
  bool const cellContainedInIsolationSphere = (cellSize <= maximumOptimalCellSize(R));

  //
  // determine neighbourhood
  // the neighbourhood is the number of cells that might contain points closer
  // than R to a cell; it is equal to R in cell size units, rounded up;
  // it's expressed as a list of coordinate shifts from a base cell to all the
  // others in the neighbourhood; it is contained in a cube
  //
  unsigned int const neighExtent = (int)std::ceil(R / cellSize);
  NeighAddresses_t neighList = buildNeighborhood(partition.indexManager(), neighExtent);

  // if a cell is not fully contained in a isolation radius, we need to check
  // the points of the cell with each other: their cell becomes part of the
  // neighbourhood
  if (!cellContainedInIsolationSphere) neighList.insert(neighList.begin(), {0, 0, 0});

  //
  // populate the partition
  //
  partition.fill(begin, end);

  //
  // for each cell in the partition:
  //
  size_t const nCells = partition.indexManager().size();
  for (Indexer_t::CellIndex_t cellIndex = 0; cellIndex < nCells; ++cellIndex) {
    auto const& cellPoints = partition[cellIndex];

    //
    // if the cell has more than one element, mark all points as non-isolated;
    // true only if the cell is completely contained within a R rßadius
    //
    if (cellContainedInIsolationSphere && (cellPoints.size() > 1)) {
      for (auto const& pointPtr : cellPoints)
        nonIsolated.push_back(std::distance(begin, pointPtr));
      continue;
    } // if all non-isolated

    //
    // brute force approach: try all the points in this cell against all the
    // points in the neighbourhood
    //
    for (auto const pointPtr : cellPoints) {
      //
      // optimisation (speed): mark the points from other cells as non-isolated
      // when they trigger non-isolation in points of the current one
      //

      // TODO

      if (!isPointIsolatedWithinNeighborhood(partition, cellIndex, *pointPtr, neighList)) {
        nonIsolated.push_back(std::distance(begin, pointPtr));
      }
    } // for points in cell

  } // for cell

  return nonIsolated;
} // lar::example::PointIsolationAlg::removeIsolatedPoints()

//--------------------------------------------------------------------------
template <typename Coord>
void lar::example::PointIsolationAlg<Coord>::validateConfiguration(Configuration_t const& config)
{
  std::vector<std::string> errors;
  if (config.radius2 < Coord_t(0)) {
    errors.push_back("invalid radius squared (" + std::to_string(config.radius2) + ")");
  }
  if (!config.rangeX.valid()) { errors.push_back("invalid x range " + rangeString(config.rangeX)); }
  if (!config.rangeY.valid()) { errors.push_back("invalid y range " + rangeString(config.rangeY)); }
  if (!config.rangeZ.valid()) { errors.push_back("invalid z range " + rangeString(config.rangeZ)); }

  if (errors.empty()) return;

  // compose the full error message as concatenation:
  std::string message(std::to_string(errors.size()) + " configuration errors found:");

  for (auto const& error : errors)
    message += "\n * " + error;
  throw std::runtime_error(message);

} // lar::example::PointIsolationAlg::validateConfiguration()

//--------------------------------------------------------------------------
template <typename Coord>
template <typename PointIter /* = std::array<double, 3> const* */>
Coord lar::example::PointIsolationAlg<Coord>::computeCellSize() const
{

  Coord_t const R = std::sqrt(config.radius2);

  // maximum: the maximum distance between two points in the cell (that is,
  //   the diagonal of the cell) must be no larger than the isolation radius R;
  // minimum: needs tuning
  Coord_t cellSize = maximumOptimalCellSize(R); // try the minimum for now

  //
  // optimisation (memory): determine minimum size of box
  //

  // TODO

  if (config.maxMemory == 0) return cellSize;

  do {
    std::array<size_t, 3> partition =
      details::diceVolume(CoordRangeCells<Coord_t>{config.rangeX, cellSize},
                          CoordRangeCells<Coord_t>{config.rangeY, cellSize},
                          CoordRangeCells<Coord_t>{config.rangeZ, cellSize});

    size_t const nCells = partition[0] * partition[1] * partition[2];
    if (nCells <= 1) break; // we can't reduce it any further

    // is memory low enough?
    size_t const memory = nCells * sizeof(typename SpacePartition<PointIter>::Cell_t);
    if (memory < config.maxMemory) break;

    cellSize *= 2;
  } while (true);

  return cellSize;
} // lar::example::PointIsolationAlg<Coord>::computeCellSize()

//------------------------------------------------------------------------------
template <typename Coord>
typename lar::example::PointIsolationAlg<Coord>::NeighAddresses_t
lar::example::PointIsolationAlg<Coord>::buildNeighborhood(Indexer_t const& indexer,
                                                          unsigned int neighExtent) const
{
  unsigned int const neighSize = 1 + 2 * neighExtent;
  NeighAddresses_t neighList;
  neighList.reserve(neighSize * neighSize * neighSize - 1);

  using CellID_t = Indexer_t::CellID_t;
  using CellDimIndex_t = Indexer_t::CellDimIndex_t;

  //
  // optimisation (speed): reshape the neighbourhood
  // neighbourhood might cut out cells close to the vertices
  //

  // TODO

  CellDimIndex_t const ext = neighExtent; // convert into the right signedness

  CellID_t center{{0, 0, 0}}, cellID;
  for (CellDimIndex_t ixOfs = -ext; ixOfs <= ext; ++ixOfs) {
    cellID[0] = ixOfs;
    for (CellDimIndex_t iyOfs = -ext; iyOfs <= ext; ++iyOfs) {
      cellID[1] = iyOfs;
      for (CellDimIndex_t izOfs = -ext; izOfs <= ext; ++izOfs) {
        if ((ixOfs == 0) && (iyOfs == 0) && (izOfs == 0)) continue;
        cellID[2] = izOfs;

        neighList.push_back(indexer.offset(center, cellID));

      } // for ixOfs
    }   // for iyOfs
  }     // for izOfs

  return neighList;
} // lar::example::PointIsolationAlg<Coord>::buildNeighborhood()

//--------------------------------------------------------------------------
template <typename Coord>
template <typename PointIter>
bool lar::example::PointIsolationAlg<Coord>::isPointIsolatedFrom(
  Point_t<PointIter> const& point,
  typename Partition_t<PointIter>::Cell_t const& otherPoints) const
{

  for (auto const& otherPointPtr : otherPoints) {
    // make sure that we did not compare the point with itself
    if (closeEnough(point, *otherPointPtr) && (&point != &*otherPointPtr)) return false;
  }

  return true;

} // lar::example::PointIsolationAlg<Coord>::isPointIsolatedFrom()

//--------------------------------------------------------------------------
template <typename Coord>
template <typename PointIter>
bool lar::example::PointIsolationAlg<Coord>::isPointIsolatedWithinNeighborhood(
  Partition_t<PointIter> const& partition,
  Indexer_t::CellIndex_t cellIndex,
  Point_t<PointIter> const& point,
  NeighAddresses_t const& neighList) const
{

  // check in all cells of the neighbourhood
  for (Indexer_t::CellIndexOffset_t neighOfs : neighList) {

    //
    // optimisation (speed): have neighbour offsets so that the invalid ones
    // are all at the beginning and at the end, so that skipping is faster
    //

    if (!partition.has(cellIndex + neighOfs)) continue;
    auto const& neighCellPoints = partition[cellIndex + neighOfs];

    if (!isPointIsolatedFrom<PointIter>(point, neighCellPoints)) return false;

  } // for neigh cell

  return true;

} // lar::example::PointIsolationAlg<Coord>::isPointIsolatedWithinNeighborhood()

//--------------------------------------------------------------------------
template <typename Coord>
template <typename PointIter>
std::vector<size_t> lar::example::PointIsolationAlg<Coord>::bruteRemoveIsolatedPoints(
  PointIter begin,
  PointIter end) const
{
  //
  // reference implementation: brute force dumb one
  //

  std::vector<size_t> nonIsolated;

  size_t i = 0;
  for (auto it = begin; it != end; ++it, ++i) {

    for (auto ioth = begin; ioth != end; ++ioth) {
      if (it == ioth) continue;

      if (closeEnough(*it, *ioth)) {
        nonIsolated.push_back(i);
        break;
      }

    } // for oth

  } // for (it)

  return nonIsolated;
} // lar::example::PointIsolationAlg::bruteRemoveIsolatedPoints()

//--------------------------------------------------------------------------
template <typename Coord>
template <typename Point>
bool lar::example::PointIsolationAlg<Coord>::closeEnough(Point const& A, Point const& B) const
{
  return cet::sum_of_squares(details::extractPositionX(A) - details::extractPositionX(B),
                             details::extractPositionY(A) - details::extractPositionY(B),
                             details::extractPositionZ(A) - details::extractPositionZ(B)) <=
         config.radius2;
} // lar::example::PointIsolationAlg<Point>::closeEnough()

//--------------------------------------------------------------------------
template <typename Coord>
std::string lar::example::PointIsolationAlg<Coord>::rangeString(Coord_t from, Coord_t to)
{
  return "(" + std::to_string(from) + " to " + std::to_string(to) + ")";
}

//--------------------------------------------------------------------------

#endif // LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_POINTISOLATIONALG_H
