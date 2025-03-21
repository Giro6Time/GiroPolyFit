#pragma once

#include "box.h"
#include "basic_types.h"
#include "basic.h"
#include "map_cell.h"
#include <memory>

class Map;

namespace GeometryTypes
{
    // 2d Point
    typedef vecng<2, Numeric::int8> Point2d_int8;
    typedef vecng<2, Numeric::int16> Point2d_int16;
    typedef vecng<2, Numeric::int32> Point2d_int32;
    typedef vecng<2, Numeric::int64> Point2d_int64;

    inline Math::vec3 vector(const std::shared_ptr<MapTypes::Halfedge> h)
    {
        return h->vertex()->point() - h->opposite()->vertex()->point();
    }

    Box3d bounding_box(const std::shared_ptr<Map> map);
    Box3d bounding_box(const std::shared_ptr<const Map> map);

    Math::vec3 facet_normal(const std::shared_ptr<MapTypes::Facet> f);
    Math::vec3 vertex_normal(const std::shared_ptr<MapTypes::Vertex> v);
    Math::vec3 triangle_normal(const std::shared_ptr<MapTypes::Facet> f);
};
