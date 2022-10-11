/**
 * @file   SpacePartition.h
 * @brief  Class to organise data into a 3D grid
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 27, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 * This library provides:
 *
 * * SpacePartition: class to organise data in space into a 3D grid
 * * CoordRange: simple coordinate range (interval) class
 * * PositionExtractor: abstraction to extract a 3D position from an object
 *
 * This library contains only template classes and it is header only.
 *
 */

#ifndef LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPARTITION_H
#define LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPARTITION_H

// LArSoft libraries
#include "lardata/Utilities/GridContainers.h"

// C/C++ standard libraries
#include <array>
#include <cmath>     // std::ceil()
#include <cstddef>   // std::ptrdiff_t
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

namespace lar {
  namespace example {

    // BEGIN RemoveIsolatedSpacePoints group -----------------------------------
    /// @ingroup RemoveIsolatedSpacePoints
    /// @{
    /**
     * @brief  Helper extractor for point coordinates
     * @tparam Point type of point structure
     *
     * The mandatory interface is:
     *
     * - `static T x(Point const& point)`: return x coordinate of point
     * - `static T y(Point const& point)`: return y coordinate of point
     * - `static T z(Point const& point)`: return z coordinate of point
     *
     * The type T must be convertible to a number (typically a real one).
     * Examples of specialisation:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *     struct SpaceTime_t {
     *       double x, y, z; ///< space coordinates
     *       double t;       ///< time coordinate
     *     };
     *
     *     namespace lar {
     *       namespace examples {
     *
     *         template <>
     *         struct PositionExtractor<SpaceTime_t> {
     *
     *           static double x(SpaceTime_t const& st) { return st.x; }
     *           static double y(SpaceTime_t const& st) { return st.y; }
     *           static double z(SpaceTime_t const& st) { return st.z; }
     *
     *         };
     *
     *         template <>
     *         struct PositionExtractor<SpaceTime_t const*> {
     *
     *           static double x(SpaceTime_t const* st) { return st->x; }
     *           static double y(SpaceTime_t const* st) { return st->y; }
     *           static double z(SpaceTime_t const* st) { return st->z; }
     *
     *         };
     *
     *       }
     *     }
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * The argument of the function just needs to be something the template
     * argument can be converted into: in the second example, the argument of
     * `X` is not required to br exactly `SpaceTime_t const* const&`.
     */
    template <typename Point>
    struct PositionExtractor;

    namespace details {
      template <typename Point>
      struct PointTraits_t;

      /// type of Point coordinate
      template <typename Point>
      using ExtractCoordType_t = typename details::PointTraits_t<Point>::Coord_t;

    } // namespace details

    /// Range of coordinates
    template <typename Coord>
    struct CoordRange {
      using Range_t = CoordRange<Coord>; ///< this type
      using Coord_t = Coord;             ///< data type for coordinate

      Coord_t lower; ///< lower boundary
      Coord_t upper; ///< upper boundary

      /// Returns whether c is contained in the range (inclusve)
      bool contains(Coord_t c) const;

      /// Returns whether the range is empty
      bool empty() const;

      /// Returns whether the range is valid (empty is also valid)
      bool valid() const;

      /// Returns the size of the range (no check)
      Coord_t size() const;

      /// Returns the distance of the specified coordinate from the lower bound
      Coord_t offset(Coord_t c) const { return c - lower; }

      /// Returns whether the specified range has the same limits as this
      bool operator==(const Range_t& as) const;

      /// Returns whether the specified range has limits different than this
      bool operator!=(const Range_t& than) const;

    }; // CoordRange<>

    /// Range of coordinates
    template <typename Coord>
    struct CoordRangeCells : public CoordRange<Coord> {
      using Base_t = CoordRange<Coord>;

      /// data type for coordinate
      using Coord_t = typename Base_t::Coord_t;

      Coord_t cellSize; ///< size of a single cell

      /**
       * @brief Constructor: assigns range and cell size
       * @param low lower bound of the range
       * @param high upper bound of the range
       * @param cs size of each cell
       */
      CoordRangeCells(Coord_t low, Coord_t high, Coord_t cs);

      /**
       * @brief Constructor: assigns range and cell size
       * @param range lower and upper bound of the range
       * @param cs size of each cell
       */
      CoordRangeCells(Base_t const& range, Coord_t cs);

      /// Returns the index of the cell for coordinate c
      std::ptrdiff_t findCell(Coord_t c) const;

    }; // CoordRangeCells<>

    /**
     * @brief A container of points sorted in cells
     * @tparam PointIter type of iterator to the point
     *
     * This container arranges its elements into a 3D grid according to their
     * position in space.
     * The "position" is defined by the `PositionExtractor` class.
     *
     * The container stores a bit on information for each cell (it is not
     * _sparse_), therefore its size can become large very quickly.
     * Currently each (empty) cell in the grid uses
     * `sizeof(std::vector<...>)`, that is 24, bytes.
     *
     * Currently, no facility is provided to find an element, although from a
     * copy of the element, its position in the container can be computed with
     * `pointIndex()`.
     *
     * For example, suppose you need to arrange points in a box of 6 x 8 x 4
     * (arbitrary units) symmetric around the origin, each with 20 cells.
     * This already makes a quite large container of 8000 elements.
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * std::vector<std::array<double, 3U>> data;
     * // fill the data points
     *
     * lar::examples::SpacePartition<double const*> partition(
     *   { -3.0, 3.0, 0.3 },
     *   { -4.0, 4.0, 0.4 },
     *   { -2.0, 2.0, 0.2 }
     *   );
     *
     * // populate the partition
     * for (auto const& point: data) partition.fill(point.data());
     *
     * // find the cell for a reference point
     * const double refPoint[] = { 0.5, 0.5, 0.5 };
     * auto cellIndex = partition.pointIndex(refPoint);
     *
     * // do something with all the points in the same cell as the reference one
     * for (double const* point: partition[cellIndex]) {
     *   // ...
     * }
     *
     * // do something with all cells
     * for (auto const& cell: partition) {
     *   // and process all points in each cell
     *   for (double const* point: cell) {
     *     // ...
     *   }
     * }
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * Note that in the example the stored data is direct pointers to the data
     * in order to save space (the data is 3 doubles big, that is 24 bytes,
     * while a pointer is usually only 8 bytes).
     * The class `PositionExtractor` is specialized for `double const*` in this
     * same library.
     */
    template <typename PointIter>
    class SpacePartition {
      using Point_t = decltype(*(PointIter()));          ///< type of the point
      using Grid_t = ::util::GridContainer3D<PointIter>; ///< data container

    public:
      /// type of point coordinate
      using Coord_t = details::ExtractCoordType_t<Point_t>;
      using Range_t = CoordRangeCells<Coord_t>; ///< type of coordinate range

      /// type of index manager of the grid
      using Indexer_t = typename Grid_t::Indexer_t;

      /// type of difference between cell indices
      using CellIndexOffset_t = typename Indexer_t::CellIndexOffset_t;

      using CellIndex_t = typename Indexer_t::CellIndex_t; /// type of cell index

      /// type of cell identifier
      using CellID_t = typename Indexer_t::CellID_t;

      /// type of cell
      using Cell_t = typename Grid_t::Cell_t;

      /// Constructs the partition in a given volume with the given cell size
      SpacePartition(Range_t rangeX, Range_t rangeY, Range_t rangeZ);

      /// Fills the partition with the points in the specified range
      /// @throw std::runtime_error a point is outside the covered volume
      void fill(PointIter begin, PointIter end);

      /// Returns the index pertaining the point (might be invalid!)
      /// @throw std::runtime_error point is outside the covered volume
      CellIndexOffset_t pointIndex(Point_t const& point) const;

      /// Returns the index manager of the grid
      Indexer_t const& indexManager() const { return data.indexManager(); }

      /// Returns whether there is a cell with the specified index (signed!)
      bool has(CellIndexOffset_t ofs) const { return data.has(ofs); }

      /// Returns the cell with the specified index
      Cell_t const& operator[](CellIndex_t index) const { return data[index]; }

      /// Returns a constant iterator pointing to the first cell
      typename Grid_t::const_iterator begin() const;

      /// Returns a constant iterator pointing after the last cell
      typename Grid_t::const_iterator end() const;

    protected:
      using CellDimIndex_t = typename Grid_t::CellDimIndex_t;

      Coord_t cellSize; ///< length of the side of each cubic cell

      Range_t xRange; ///< coordinates of the contained volume on x axis
      Range_t yRange; ///< coordinates of the contained volume on z axis
      Range_t zRange; ///< coordinates of the contained volume on z axis

      Grid_t data; ///< container of points

    }; // SpacePartition<>

    //--------------------------------------------------------------------------

    /// Implementation detail namespace (content is not documented)
    namespace details {
      /// Base for PositionExtractor on random-access containers
      template <typename Cont, typename Data>
      struct PositionExtractorFromArray {
        static Data x(Cont const& p) { return p[0]; }
        static Data y(Cont const& p) { return p[1]; }
        static Data z(Cont const& p) { return p[2]; }
      }; // PositionExtractorFromArray<T*>

      template <typename Point>
      auto extractPositionX(Point const& point)
      {
        return PositionExtractor<Point>::x(point);
      }
      template <typename Point>
      auto extractPositionY(Point const& point)
      {
        return PositionExtractor<Point>::y(point);
      }
      template <typename Point>
      auto extractPositionZ(Point const& point)
      {
        return PositionExtractor<Point>::z(point);
      }

      template <typename Point>
      struct PointTraits_t {
        /// type of Point coordinate
        using Coord_t = decltype(extractPositionX(std::decay_t<Point>()));
      }; // ExtractCoordTypeHelper_t

    } // namespace details

    /// Specialisation of PositionExtractor for C array: { x, y, z }
    template <typename T>
    struct PositionExtractor<T*> : public details::PositionExtractorFromArray<T*, T> {};

    /// Specialisation of PositionExtractor for C++ array: { x, y, z }
    template <typename T>
    struct PositionExtractor<std::array<T, 3U>>
      : public details::PositionExtractorFromArray<std::array<T, 3U>, T> {};

    /// Specialisation of PositionExtractor for C++ vector: { x, y, z }
    /// (size is not checked!)
    template <typename T>
    struct PositionExtractor<std::vector<T>>
      : public details::PositionExtractorFromArray<std::vector<T>, T> {};

    /// @}
    // END RemoveIsolatedSpacePoints group -------------------------------------

  } // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- lar::example::details
//---
namespace lar {
  namespace example {
    namespace details {

      /// Returns the dimensions of a grid diced with the specified size
      template <typename Coord>
      std::array<size_t, 3> diceVolume(CoordRangeCells<Coord> const& rangeX,
                                       CoordRangeCells<Coord> const& rangeY,
                                       CoordRangeCells<Coord> const& rangeZ)
      {
        return {{size_t(std::ceil(rangeX.size() / rangeX.cellSize)),
                 size_t(std::ceil(rangeY.size() / rangeY.cellSize)),
                 size_t(std::ceil(rangeZ.size() / rangeZ.cellSize))}};
      } // diceVolume()

    } // namespace details
  }   // namespace example
} // namespace lar

//------------------------------------------------------------------------------
//--- template implementation
//------------------------------------------------------------------------------
//--- lar::example::CoordRange
//---
template <typename Coord>
bool lar::example::CoordRange<Coord>::contains(Coord_t c) const
{
  return (lower <= c) && (upper >= c);
}

template <typename Coord>
bool lar::example::CoordRange<Coord>::Range_t::empty() const
{
  return lower == upper;
}

template <typename Coord>
bool lar::example::CoordRange<Coord>::valid() const
{
  return lower <= upper;
}

template <typename Coord>
Coord lar::example::CoordRange<Coord>::size() const
{
  return upper - lower;
}

template <typename Coord>
bool lar::example::CoordRange<Coord>::operator==(const Range_t& as) const
{
  return (upper == as.upper) && (lower == as.lower);
}

template <typename Coord>
bool lar::example::CoordRange<Coord>::operator!=(const Range_t& than) const
{
  return (upper != than.upper) || (lower != than.lower);
}

//------------------------------------------------------------------------------
//--- lar::example::CoordRange
//---
template <typename Coord>
lar::example::CoordRangeCells<Coord>::CoordRangeCells(Coord_t low, Coord_t high, Coord_t cs)
  : Base_t(low, high), cellSize(cs)
{}

template <typename Coord>
lar::example::CoordRangeCells<Coord>::CoordRangeCells(Base_t const& range, Coord_t cs)
  : Base_t(range), cellSize(cs)
{}

//------------------------------------------------------------------------------
template <typename Coord>
std::ptrdiff_t lar::example::CoordRangeCells<Coord>::findCell(Coord_t c) const
{
  return std::ptrdiff_t(Base_t::offset(c) / cellSize);
}

//------------------------------------------------------------------------------
//--- lar::example::SpacePartition
//---
template <typename PointIter>
lar::example::SpacePartition<PointIter>::SpacePartition(Range_t rangeX,
                                                        Range_t rangeY,
                                                        Range_t rangeZ)
  : xRange(rangeX)
  , yRange(rangeY)
  , zRange(rangeZ)
  , data(details::diceVolume(xRange, yRange, zRange))
{
  /*
    std::cout << "Grid: "
      << indexManager().sizeX() << " x "
      << indexManager().sizeY() << " x " << indexManager().sizeZ()
      << " (" << indexManager().size() << " cells)"
      << "\n  range X: " << xRange.lower << " -- " << xRange.upper << " [/" << xRange.cellSize << "]"
      << "\n  range Y: " << yRange.lower << " -- " << yRange.upper << " [/" << yRange.cellSize << "]"
      << "\n  range Z: " << zRange.lower << " -- " << zRange.upper << " [/" << zRange.cellSize << "]"
      << std::endl;
  */
} // lar::example::SpacePartition<>::SpacePartition

//--------------------------------------------------------------------------
template <typename PointIter>
void lar::example::SpacePartition<PointIter>::fill(PointIter begin, PointIter end)
{

  PointIter it = begin;
  while (it != end) {
    // if the point is outside the volume, pointIndex will throw an exception
    data.insert(pointIndex(*it), it);
    ++it;
  } // while

} // lar::example::SpacePartition<>::fill()

//--------------------------------------------------------------------------
template <typename PointIter>
typename lar::example::SpacePartition<PointIter>::CellIndexOffset_t
lar::example::SpacePartition<PointIter>::pointIndex(Point_t const& point) const
{
  // compute the cell ID coordinates
  Coord_t const x = details::extractPositionX(point);
  CellDimIndex_t const xc = xRange.findCell(x);
  if (!data.hasX(xc)) {
    throw std::runtime_error("Point out of the volume (x = " + std::to_string(x) + ")");
  }

  Coord_t const y = details::extractPositionY(point);
  CellDimIndex_t const yc = yRange.findCell(y);
  if (!data.hasY(yc)) {
    throw std::runtime_error("Point out of the volume (y = " + std::to_string(y) + ")");
  }

  Coord_t const z = details::extractPositionZ(point);
  CellDimIndex_t const zc = zRange.findCell(z);
  if (!data.hasZ(zc)) {
    throw std::runtime_error("Point out of the volume (z = " + std::to_string(z) + ")");
  }

  // return its index
  return data.index(CellID_t{{xc, yc, zc}});

} // lar::example::SpacePartition<>::pointIndex()

//--------------------------------------------------------------------------

#endif // LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPARTITION_H
