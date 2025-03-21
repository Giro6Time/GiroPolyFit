#include "math_types.h"
#include "map.h"
#include "basic.h"
#include "box.h"

namespace GeometryTypes
{

    Box3d bounding_box(const std::shared_ptr<Map> map)
    {
        my_debug_assert(map->num_vertices() > 0);
        Box3d result;
        for (auto it = map->vertices_begin(); it != map->vertices_end(); it++)
        {
            result.add_point((*it)->point());
        }
        return result;
    }

    Box3d bounding_box(const std::shared_ptr<const Map> map)
    {
        my_debug_assert(map->num_vertices() > 0);
        Box3d result;
        for (auto it = map->vertices_begin(); it != map->vertices_end(); it++)
        {
            result.add_point((*it)->point());
        }
        return result;
    }

    Math::vec3 facet_normal(const std::shared_ptr<MapTypes::Facet> f)
    {
        my_debug_assert(f->degree() >= 3);
        Math::vec3 result(0, 0, 0);
        auto it = f->halfedge();
        do
        {
            Math::vec3 v0 = vector(it);
            Math::vec3 v1 = vector(it->prev()->opposite());
            result += cross(v0, v1);
            it = it->next();
        } while (it != f->halfedge());
        return normalize(result);
    }

    Math::vec3 vertex_normal(const std::shared_ptr<MapTypes::Vertex> v)
    {
        my_debug_assert(v->degree() > 0);
        Math::vec3 result(0, 0, 0);
        auto it = v->halfedge();
        do
        {
            result += facet_normal(it->facet());
            it = it->next_around_vertex();
        } while (it != v->halfedge());
        return normalize(result);
    }

    Math::vec3 triangle_normal(const std::shared_ptr<MapTypes::Facet> f)
    {
        my_debug_assert(f->is_triangle());
        Math::vec3 result = (cross(vector(f->halfedge()->next()),
                                   vector(f->halfedge()->opposite()))) +
                            (cross(vector(f->halfedge()),
                                   vector(f->halfedge()->prev()->opposite()))) +
                            (cross(vector(f->halfedge()->next()->next()),
                                   vector(f->halfedge()->next()->opposite())));
        result = normalize(result);
        return result;
    }
};