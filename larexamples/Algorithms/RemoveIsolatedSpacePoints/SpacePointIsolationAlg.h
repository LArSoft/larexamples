/**
 * @file   SpacePointIsolationAlg.h
 * @brief  Algorithm(s) dealing with space point isolation in space
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 26, 2016
 * @ingroup RemoveIsolatedSpacePoints
 *
 */

#ifndef LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTISOLATIONALG_H
#define LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTISOLATIONALG_H

// LArSoft libraries
#include "lardataobj/RecoBase/SpacePoint.h"
#include "larexamples/Algorithms/RemoveIsolatedSpacePoints/PointIsolationAlg.h"

// infrastructure and utilities
#include "cetlib/pow.h" // cet::square()
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/Table.h"

// C/C++ standard libraries
#include <memory>      // std::unique_ptr<>
#include <type_traits> // std::decay_t<>, std::is_base_of<>
#include <vector>

// forward declarations
namespace geo {
  class GeometryCore;
}

namespace lar {
  namespace example {

    // BEGIN RemoveIsolatedSpacePoints group -----------------------------------
    /// @ingroup RemoveIsolatedSpacePoints
    /// @{
    /**
     * @brief Algorithm to detect isolated space points.
     * @see @ref RemoveIsolatedSpacePoints "RemoveIsolatedSpacePoints example overview"
     *
     * This algorithm applies the isolation algorithm implemented in
     * `PointIsolationAlg` to a collection of `recob::SpacePoint` objects.
     *
     * Usage example
     * --------------
     *
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *     //
     *     // preparation
     *     //
     *
     *     // get the algorithm configuration; in art:
     *     fhicl::ParameterSet config
     *       = pset.get<fhicl::ParameterSet>("isolation");
     *
     *     // get the geometry service provider; in art:
     *     geo::GeometryCore const* geom = lar::providerFrom<geo::Geometry>();
     *
     *     // get the input data; in art:
     *     std::vector<recob::SpacePoint> const& points
     *       = *(event.getValidHandle<std::vector<recob::SpacePoint>>("sps"));
     *
     *     //
     *     // algorithm execution
     *     //
     *
     *     // construct and configure
     *     lar::examples::SpacePointIsolationAlg algo(config);
     *
     *     // set up
     *     // (might be needed again if geometry changed, e.g. between runs)
     *     algo.Setup(*geom);
     *
     *     // execution
     *     std::vector<size_t> nonIsolatedPointIndices
     *       = algo.removeIsolatedPoints(points);
     *
     *     //
     *     // use of results
     *     //
     *
     *     // e.g. create a collection of pointers to non-isolated points
     *     std::vector<recob::SpacePoint const*> nonIsolatedPoints;
     *     nonIsolatedPoints.reserve(nonIsolatedPointIndices.size());
     *     recob::SpacePoint const* basePoint = &(points.front());
     *     for (size_t index: nonIsolatedPointIndices)
     *       nonIsolatedPoints.push_back(basePoint + index);
     *
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *
     *
     * Configuration parameters
     * =========================
     *
     * * *radius* (real, mandatory): isolation radius [cm]
     *
     */
    class SpacePointIsolationAlg {

    public:
      /// Type of coordinate in recob::SpacePoint (`double` in LArSoft 5)
      using Coord_t = std::decay_t<decltype(recob::SpacePoint().XYZ()[0])>;

      /// Algorithm configuration
      struct Config {

        using Name = fhicl::Name;
        using Comment = fhicl::Comment;

        fhicl::Atom<double> radius{Name("radius"), Comment("the radius for the isolation [cm]")};

      }; // Config

      /// @{
      /// @name Construction and configuration

      /**
       * @brief Constructor with configuration validation
       * @param config configuration parameter structure
       *
       * For the configuration, see `SpacePointIsolationAlg` documentation.
       */
      SpacePointIsolationAlg(Config const& config) : radius2(cet::square(config.radius())) {}

      /**
       * @brief Constructor with configuration validation
       * @param pset FHiCL configuration parameter set
       * @see SpacePointIsolationAlg(Config const&)
       *
       * Translates the parameter set into a configuration object and uses the
       * validating constructor to initialise the object.
       *
       * For the configuration, see `SpacePointIsolationAlg` documentation.
       */
      SpacePointIsolationAlg(fhicl::ParameterSet const& pset)
        : SpacePointIsolationAlg(fhicl::Table<Config>(pset, {})())
      {}

      /// @}

      /// @{
      /// @name Set up

      /**
       * @brief Sets up the algorithm
       * @param geometry the geometry service provider
       *
       * Acquires the geometry description.
       * This method must be called every time the geometry is changed.
       */
      void setup(geo::GeometryCore const& geometry)
      {
        geom = &geometry;
        initialize();
      }

      /// @}

      /**
       * @brief Returns the set of reconstructed 3D points that are not isolated
       * @tparam PointIter random access iterator to a space point
       * @param begin iterator to the first point to be considered
       * @param end iterator after the last point to be considered
       * @return a list of indices of non-isolated points in the input range
       * @see PointIsolationAlg::removeIsolatedPoints(PointIter, PointIter) const
       *
       * This method can use iterators from any collection of input space
       * points.
       */
      template <typename PointIter>
      std::vector<size_t> removeIsolatedPoints(PointIter begin, PointIter end) const
      {
        static_assert(std::is_base_of<recob::SpacePoint, std::decay_t<decltype(*begin)>>::value,
                      "iterator does not point to recob::SpacePoint");
        return isolationAlg->removeIsolatedPoints(begin, end);
      }

      /**
       * @brief Returns the set of reconstructed 3D points that are not isolated
       * @param points list of the reconstructed space points
       * @return a list of indices of non-isolated points in the vector
       * @see removeIsolatedPoints(PointIter, PointIter) const
       */
      std::vector<size_t> removeIsolatedPoints(std::vector<recob::SpacePoint> const& points) const
      {
        return removeIsolatedPoints(points.begin(), points.end());
      }

    private:
      /// Type of isolation algorithm
      using PointIsolationAlg_t = PointIsolationAlg<Coord_t>;

      /// Pointer to the geometry to be used
      geo::GeometryCore const* geom = nullptr;

      Coord_t radius2; ///< square of isolation radius [cm^2]

      /// the actual generic algorithm
      std::unique_ptr<PointIsolationAlg_t> isolationAlg;

      /// Initialises the algorithm with the current configuration and setup
      void initialize();

      /// Detects the boundaries of the volume to be sorted from the geometry
      void fillAlgConfigFromGeometry(PointIsolationAlg_t::Configuration_t& config);

    }; // class SpacePointIsolationAlg

    //--------------------------------------------------------------------------
    //--- PositionExtractor<recob::SpacePoint>

    /**
     * @brief Specialization of PositionExtractor for recob::SpacePoint
     *
     * This class extracts coordinates from `recob::SpacePoint::XYZ()` method.
     */
    template <>
    struct PositionExtractor<recob::SpacePoint> {

      /// Return x coordinate of the space point position
      static double x(recob::SpacePoint const& p) { return p.XYZ()[0]; }

      /// Return y coordinate of the space point position
      static double y(recob::SpacePoint const& p) { return p.XYZ()[1]; }

      /// Return z coordinate of the space point position
      static double z(recob::SpacePoint const& p) { return p.XYZ()[2]; }

    }; // PositionExtractor<recob::SpacePoint>

    /// @}
    // END RemoveIsolatedSpacePoints group -------------------------------------

  } // namespace example
} // namespace lar

#endif // LAREXAMPLES_ALGORITHMS_REMOVEISOLATEDSPACEPOINTS_SPACEPOINTISOLATIONALG_H
