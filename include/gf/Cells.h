/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_CELLS_H
#define GF_CELLS_H

#include <functional>
#include <vector>

#include "CellTypes.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Polyline.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_cells
   * @brief Specification of the query of neighborhood
   *
   * @sa gf::Cells
   */
  enum class CellNeighborQuery {
    Valid               = 0x01,
    Diagonal            = 0x02,
  };


  /**
   * @ingroup core_cells
   * @brief The properties of cells
   *
   * These properties depend on the orientation of the cells in the map:
   * - gf::OrthogonalCells
   * - gf::StaggeredCells
   * - gf::HexagonalCells
   *
   * @sa gf::CellOrientation, gf::Grid, gf::TileLayer
   */
  class GF_CORE_API Cells {
  public:
    virtual ~Cells();

    virtual RectF computeBounds(Vector2i layerSize) const noexcept = 0;

    virtual RectI computeVisibleArea(const RectF& local) const noexcept = 0;

    virtual RectF computeCellBounds(Vector2i coords) const noexcept = 0;

    virtual Vector2i computeCoordinates(Vector2f position) const noexcept = 0;

    virtual Polyline computePolyline(Vector2i coords) const = 0;

    virtual std::vector<Vector2i> computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags = gf::None) const = 0;
  };

  /**
   * @ingroup core_cells
   * @brief Orthogonal cells
   */
  class GF_CORE_API OrthogonalCells : public Cells {
  public:
    OrthogonalCells(Vector2f tileSize)
    : m_tileSize(tileSize)
    {
    }

    RectF computeBounds(Vector2i layerSize) const noexcept override;

    RectI computeVisibleArea(const RectF& local) const noexcept override;

    RectF computeCellBounds(Vector2i coords) const noexcept override;

    Vector2i computeCoordinates(Vector2f position) const noexcept override;

    Polyline computePolyline(Vector2i coords) const override;

    std::vector<Vector2i> computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags = gf::None) const override;

  private:
    Vector2f m_tileSize;
  };


  /**
   * @ingroup core_cells
   * @brief Staggered cells
   */
  class GF_CORE_API StaggeredCells : public Cells {
  public:
    StaggeredCells(Vector2f tileSize, CellAxis axis, CellIndex index)
    : m_tileSize(tileSize)
    , m_axis(axis)
    , m_index(index)
    {
    }

    RectF computeBounds(Vector2i layerSize) const noexcept override;

    RectI computeVisibleArea(const RectF& local) const noexcept override;

    RectF computeCellBounds(Vector2i coords) const noexcept override;

    Vector2i computeCoordinates(Vector2f position) const noexcept override;

    Polyline computePolyline(Vector2i coords) const override;

    std::vector<Vector2i> computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags = gf::None) const override;

  private:
    Vector2f m_tileSize;
    CellAxis m_axis;
    CellIndex m_index;
  };


  /**
   * @ingroup core_cells
   * @brief Hexagonal cells
   */
  class GF_CORE_API HexagonalCells : public Cells {
  public:
    HexagonalCells(Vector2f tileSize, float sideLength, CellAxis axis, CellIndex index)
    : m_tileSize(tileSize)
    , m_sideLength(sideLength)
    , m_axis(axis)
    , m_index(index)
    {
    }

    HexagonalCells(float radius, CellAxis axis, CellIndex index)
    : m_tileSize(computeRegularSize(axis, radius))
    , m_sideLength(radius)
    , m_axis(axis)
    , m_index(index)
    {
    }

    RectF computeBounds(Vector2i layerSize) const noexcept override;

    RectI computeVisibleArea(const RectF& local) const noexcept override;

    RectF computeCellBounds(Vector2i coords) const noexcept override;

    Vector2i computeCoordinates(Vector2f position) const noexcept override;

    Polyline computePolyline(Vector2i coords) const override;

    std::vector<Vector2i> computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags = gf::None) const override;

    /**
     * @brief Get the size of a regular hexagon
     *
     * @param axis The hexagon axis
     * @param radius Radius of hexagon
     *
     * @returns The size of the bounding box of the hexagon
     */
    static Vector2f computeRegularSize(CellAxis axis, float radius);

  private:
    Vector2f m_tileSize;
    float m_sideLength;
    CellAxis m_axis;
    CellIndex m_index;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TILE_PROPERTIES_H
