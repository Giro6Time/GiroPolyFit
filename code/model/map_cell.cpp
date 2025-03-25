#include "map_cell.h"
#include "math_types.h"

namespace MapTypes
{
    unsigned Combel::id_cnt = 0;
    // Vertex
    Vertex::Vertex() {}
    Vertex::Vertex(const vec3 &p) : point_(p) {}
    Vertex::~Vertex() { halfedge_.reset(); }

    bool Vertex::is_valid() const
    {
        return (halfedge()->vertex() == shared_from_this());
    }

    void Vertex::assert_is_valid() const
    {
        my_assert(halfedge()->vertex() == shared_from_this());
    }

    unsigned int Vertex::degree() const
    {
        unsigned int result = 0;
        auto it = halfedge();
        do
        {
            result++;
            it = it->next_around_vertex();
        } while (it != halfedge());
        return result;
    }
    bool Vertex::is_on_border() const
    {
        auto it = halfedge();
        do
        {
            if (it->is_border())
            {
                return true;
            }
            it = it->next_around_vertex();
        } while (it != halfedge());
        return false;
    }
    bool Vertex::is_connected(const std::shared_ptr<Vertex> v) const
    {
        auto it = halfedge();
        do
        {
            if (it->opposite()->vertex() == v)
                return true;

            it = it->next_around_vertex();
        } while (it != halfedge());

        return false;
    }

    // Halfedge
    bool Halfedge::is_valid() const
    {
        auto thisObj = shared_from_this();
        return (opposite()->opposite() == thisObj) &&
               (next()->prev() == thisObj) &&
               (prev()->next() == thisObj);
    }

    void Halfedge::assert_is_valid() const
    {
        my_assert(is_valid());
    }

    // Facet

    int Facet::degree() const
    {
        int result = 0;
        auto it = halfedge();
        do
        {
            result++;
            it = it->next();
        } while (it != halfedge());
        return result;
    }

    bool Facet::is_triangle() const
    {
        return (halfedge()->next()->next()->next() == halfedge());
    }

    bool Facet::is_on_border() const
    {
        auto it = halfedge();
        do
        {
            if (it->opposite()->is_border())
            {
                return true;
            }
            it = it->next();
        } while (it != halfedge());
        return false;
    }

    bool Facet::is_valid() const
    {
        return (
            halfedge()->facet() == shared_from_this() &&
            degree() > 2);
    }

    void Facet::assert_is_valid() const
    {
        my_assert(halfedge()->facet() == shared_from_this());
        my_assert(num_edges() > 2);
    }

}